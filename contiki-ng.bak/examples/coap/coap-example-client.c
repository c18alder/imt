/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Erbium (Er) CoAP client example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#if PLATFORM_SUPPORTS_BUTTON_HAL
#include "dev/button-hal.h"
#else
#include "dev/button-sensor.h"
#endif

// FOR UART WORKING
#include "dev/uart.h"
#include "dev/serial-line.h"
#include <stdint.h>

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "client"
#define LOG_LEVEL  LOG_LEVEL_COAP

/* FIXME: This server address is hard-coded for Cooja and link-local for unconnected border router. */
#define SERVER_EP "coap://[fe80::212:4b00:430:53ef]"

//TO OBSERVE
#define OBS_RESOURCE_URI_1 "Car_Speed"

#define TOGGLE_INTERVAL 10

PROCESS(er_example_client, "Erbium Example Client");
AUTOSTART_PROCESSES(&er_example_client);

static struct etimer et;


//FOR OBSERVE USAGE
static coap_observee_t *obs;

//FOR THE UART VALUES
static int last_kilometrage_data;
static int last_battery_data;
static int last_speed_data;



/* Example URIs that can be queried. */
#define NUMBER_OF_URLS 3
/* leading and ending slashes only for demo purposes, get cropped automatically when setting the Uri-Path */
char *service_urls[NUMBER_OF_URLS] =
{ "test/hello", "Car_Battery", "Car_Kilometrage" };
#if PLATFORM_HAS_BUTTON
static int uri_switch = 0;
#endif


//FUNCION PARA ADMINISTRAR LAS NOTIFICACIONES DE OBSERVER
static void
notification_callback(coap_observee_t *obs, void *notification,
                      coap_notification_flag_t flag)
{
  int len = 0;
  const uint8_t *payload = NULL;

  printf("Notification handler\n");
  printf("Observee URI: %s\n", obs->url);
  if(notification) {
    len = coap_get_payload(notification, &payload);
  }
  switch(flag) {
  case NOTIFICATION_OK:
    printf("NOTIFICATION OK: %*s\n", len, (char *)payload);
    //SAVE IT IN THE SPEED VARIABLE
    break;
  case OBSERVE_OK: /* server accepeted observation request */
    printf("OBSERVE_OK: %*s\n", len, (char *)payload);
    break;
  case OBSERVE_NOT_SUPPORTED:
    printf("OBSERVE_NOT_SUPPORTED: %*s\n", len, (char *)payload);
    obs = NULL;
    break;
  case ERROR_RESPONSE_CODE:
    printf("ERROR_RESPONSE_CODE: %*s\n", len, (char *)payload);
    obs = NULL;
    break;
  case NO_REPLY_FROM_SERVER:
    printf("NO_REPLY_FROM_SERVER: "
           "removing observe registration with token %x%x\n",
           obs->token[0], obs->token[1]);
    obs = NULL;
    break;
  }
}
//////////////////////////
//SOLICITAR OBSERVAR o DEJAR DE OBSERVAR CON EL BOTON DE LA MOTE
void
toggle_observation(coap_endpoint_t server_ep)
{

	  //printf("coap_endpoint_log \n");//DEBUGGING
	 // coap_endpoint_log(&server_ep);  //DEBUGGING

  if(obs) {
    printf("Stopping observation\n");
    coap_obs_remove_observee(obs);
    obs = NULL;
  } else {
    printf("Starting observation\n");
    obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_1, notification_callback, NULL);

  }
}
////////////////////////////////////////////




/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(coap_message_t *response)
{
	char *pch;
  const uint8_t *chunk;

  int len = coap_get_payload(response, &chunk);

  printf("|%.*s", len, (char *)chunk);
 //SAVES THE INTEGER DATA IN last_speed_data, etc..
  pch= strtok ((char *)chunk, "=");
              	       	   if(!(strcmp(pch,"km/h"))){
              	       		   pch=strtok(NULL,"}");
              	       		   last_speed_data=atoi(pch);

              	       	   }else if(!(strcmp(pch,"kms"))){
              	       		   pch=strtok(NULL,"}");
              	       		   last_kilometrage_data=atoi(pch);
              	       	   }else if(!(strcmp(pch,"%bat"))){
              	       		   pch=strtok(NULL,"}");
              	       		   last_battery_data=atoi(pch);

              	       	   }
            	printf("\nLast Speed Value %d \n", last_speed_data);
            	printf("\nLast Kilometrage Value %d \n", last_kilometrage_data);
            	printf("\nLast Battery Value %d \n", last_battery_data);

 ////


 printf("\n Longitud %d",len);
}
PROCESS_THREAD(er_example_client, ev, data)
{
  static coap_endpoint_t server_ep;
  PROCESS_BEGIN();

  static coap_message_t request[1];      /* This way the packet can be treated as pointer as usual. */

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  /* receives all CoAP messages */
  coap_engine_init();

  etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);

#if PLATFORM_HAS_BUTTON
#if !PLATFORM_SUPPORTS_BUTTON_HAL
  SENSORS_ACTIVATE(button_sensor);
#endif
  //printf("Press a button to request %s\n", service_urls[uri_switch]);
  printf("Press a button to start/stop observation of remote resource\n");
#endif /* PLATFORM_HAS_BUTTON */

  //TO THE PARSE ( UART EVENT)


  while(1) {
    PROCESS_YIELD();

    //if(etimer_expired(&et)) {
    //  printf("--Toggle timer--\n");

      /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
      /* coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, service_urls[1]);

      const char msg[] = "Toggle!";

      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      LOG_INFO_COAP_EP(&server_ep);
      LOG_INFO_("\n");

      COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

      printf("\n--Done--\n");

      etimer_reset(&et); */             // TIMER




#if PLATFORM_HAS_BUTTON
#if PLATFORM_SUPPORTS_BUTTON_HAL
    	if(ev == button_hal_release_event) {
#else
    	if(ev == sensors_event && data == &button_sensor) {
#endif


    toggle_observation(server_ep);

         printf("\n--Done--\n");
    	}
#endif



    	if(etimer_expired(&et)) {

      /* send a request to notify the end of the process */

      coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
      coap_set_header_uri_path(request, service_urls[uri_switch]);

      printf("--Requesting %s--\n", service_urls[uri_switch]);

      LOG_INFO_COAP_EP(&server_ep);
      LOG_INFO_("\n");

      COAP_BLOCKING_REQUEST(&server_ep, request,
                            client_chunk_handler);



      printf("\n--Done--\n");

      uri_switch = (uri_switch + 1) % NUMBER_OF_URLS;
      etimer_reset(&et);
    	}
//#endif /* PLATFORM_HAS_BUTTON */

  }

  PROCESS_END();
}





