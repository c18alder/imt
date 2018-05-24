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

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "client"
#define LOG_LEVEL  LOG_LEVEL_COAP



/* HARDCODED ADDRESS OF THE SERVER */
#define SERVER_EP "coap://[fe80::212:4b00:430:53ef]"

//RESOURCES TO OBSERVE (THROUGH SUSCRIPTION)
#define OBS_RESOURCE_URI_1 "Car_Speed"
#define OBS_RESOURCE_URI_2 "Car_Battery"
#define OBS_RESOURCE_URI_3 "Car_Kilometrage"
#define OBS_RESOURCE_URI_4 "Car_Temp_One"
#define OBS_RESOURCE_URI_5 "Car_Temp_Two"
#define OBS_RESOURCE_URI_6 "Car_Temp_Three"
#define GET_RESOURCE_URI_1 "Car_Cells"

//TIMER IN SECONDS IN BETWEEN REQUESTS OF DIFFERENT RESOURCES
#define TOGGLE_INTERVAL 10
/* Example URIs that can be queried. */
#define NUMBER_OF_URLS 7

PROCESS(er_example_client, "Erbium Example Client");
AUTOSTART_PROCESSES(&er_example_client);




//FOR OBSERVE USAGE
static struct etimer et;
static coap_observee_t *obs;
static short int counter; //To toggle resource

//FOR THE UART VALUES (SAVE HERE THE LAST VALUE OF EACH RESOURCE)- maybe it could also be stored
// the full string value received

static char last_kilometrage_data[MAX_RESOURCE_FORMAT_SIZE];
static char last_battery_data[MAX_RESOURCE_FORMAT_SIZE];
static char last_speed_data[MAX_RESOURCE_FORMAT_SIZE];
static char last_temp_one_data[MAX_RESOURCE_FORMAT_SIZE];
static char last_temp_two_data[MAX_RESOURCE_FORMAT_SIZE];
static char last_temp_three_data[MAX_RESOURCE_FORMAT_SIZE];
static short int last_cells_data[NUMBER_OF_CELLS];

//DECLARE THE URLs YOU WILL REQUEST (DON'T INCLUDE THE OBSERVED ONES)
char *service_urls[NUMBER_OF_URLS] =
{ OBS_RESOURCE_URI_1, OBS_RESOURCE_URI_2, OBS_RESOURCE_URI_3, OBS_RESOURCE_URI_4,OBS_RESOURCE_URI_5,GET_RESOURCE_URI_1, OBS_RESOURCE_URI_6};
static int uri_switch = 5;  //The one with GET ( timer )



//STORE THE VALUE AND/OR FORWARD IT TO THE DISPLAY HANDLER
int handle_observed_resource(const uint8_t *data_payload,const char *uri){

			if(!(strcmp(uri,OBS_RESOURCE_URI_1))){
				strcpy(last_speed_data,(char *)data_payload);
			}else if(!(strcmp(uri,OBS_RESOURCE_URI_2))){
				strcpy(last_battery_data,(char *)data_payload);
			}else if(!(strcmp(uri,OBS_RESOURCE_URI_3))){
				strcpy(last_kilometrage_data,(char *)data_payload);
			}else if(!(strcmp(uri,OBS_RESOURCE_URI_4))){
				strcpy(last_temp_one_data,(char *)data_payload);
			}else if(!(strcmp(uri,OBS_RESOURCE_URI_5))){
				strcpy(last_temp_two_data,(char *)data_payload);
			}else if(!(strcmp(uri,OBS_RESOURCE_URI_6))){
				strcpy(last_temp_three_data,(char *)data_payload);
			}else if(!(strcmp(service_urls[uri_switch],GET_RESOURCE_URI_1))){  //TODO why can't use URI? ? uri=NULL; because piggybagged?

				memcpy(last_cells_data,data_payload,NUMBER_OF_CELLS*2);
			}



	 return 0;

}


//FUNCTION TO HANDLE THE CLIENT'S RESPONSE TO THE OBSERVE REQUEST
//AND FOLLOWING NOTIFICATIONS
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
   // store_integer_value(payload);   //stores the new resource
    handle_observed_resource(payload,obs->url);
    break;
  case OBSERVE_OK: /* server accepeted observation request */
    printf("OBSERVE_OK: %*s\n", len, (char *)payload);
   // store_integer_value(payload);   //stores the new resource also the first time it suscribes
    handle_observed_resource(payload,obs->url);
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
//MAKES A NEW OBSERVE REQUEST OR STOPs THE CURRENT ONE
void
toggle_observation(coap_endpoint_t server_ep)   //FIXME: very bad coded
{
	if(counter==0){

    printf("Starting observation\n");
    obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_1, notification_callback, NULL);
	}else if(counter==1){
		printf("Starting observation\n");
		obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_2, notification_callback, NULL);
  } else if(counter==2){
    printf("Starting observation\n");
    obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_3, notification_callback, NULL);
  } else if(counter==3){
      printf("Starting observation\n");
      obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_4, notification_callback, NULL);
  } else if(counter==4){
      printf("Starting observation\n");
      obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_5, notification_callback, NULL);
  } else if(counter==5){
      printf("Starting observation\n");
      obs = coap_obs_request_registration(&server_ep, OBS_RESOURCE_URI_6, notification_callback, NULL);
  }
	counter++;
	if (counter==6){
	counter=0;
	}

}

////////////////////////////////////////////

/* This function will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(coap_message_t *response)
{
	//char *pch;
  const uint8_t *chunk;
  // int len =  coap_get_payload(response, &chunk);
 coap_get_payload(response, &chunk);
  //printf("|%.*s\n", len, (char *)chunk);


  //store_integer_value(chunk); //careful!, it changes chunk  (TODO: fix it or make sure you don't need chunk anymore)
  handle_observed_resource(chunk, response->uri_path);

  	  	  	  	PRINTF("\nLast Speed Value %s \n", last_speed_data);
              	PRINTF("\nLast Kilometrage Value %s \n", last_kilometrage_data);
              	PRINTF("\nLast Battery Value %s \n", last_battery_data);
              	PRINTF("\nLast Temp :  %s  %s  %s\n", last_temp_one_data, last_temp_two_data, last_temp_three_data);
              	PRINTF("Cells: ");
              	//Print like this not to store a Float valiable.
              	for (int i=0 ; i<24; i++){
              	PRINTF("c%d=%d.%d , ", i+1,last_cells_data[i]/100,last_cells_data[i]-((last_cells_data[i]/100)*100));
              	}
              	PRINTF(" \n\n ");

}


PROCESS_THREAD(er_example_client, ev, data)
{
  static coap_endpoint_t server_ep;
  PROCESS_BEGIN();

  static coap_message_t request[1];      /* This way the packet can be treated as pointer as usual. */
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  /* receives all CoAP messages */
  coap_engine_init();


  NETSTACK_MAC.on();//


  etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);

//ACTIVATE BUTTON TO MAKE OBSERVE REQUEST -
#if PLATFORM_HAS_BUTTON
#if !PLATFORM_SUPPORTS_BUTTON_HAL
  SENSORS_ACTIVATE(button_sensor);
#endif
  printf("Press a button to start/stop observation of remote resource\n");
#endif /* PLATFORM_HAS_BUTTON */

counter=0; // Initialize counter for new observed resource
  while(1) {
    PROCESS_YIELD();

    //PRESS BUTTON TO MAKE OBSERVE REQUEST OR STOP (EVENT BUTTON)
		#if PLATFORM_HAS_BUTTON
		#if PLATFORM_SUPPORTS_BUTTON_HAL
   			if(ev == button_hal_release_event) {
		#else
   			if(ev == sensors_event && data == &button_sensor) {
		#endif

   			toggle_observation(server_ep);

   			printf("\n--Done--\n");
   			}
   		#endif /* end platform_has_button*/

   	 //(EVENT TIMER - REQUEST NEXT RESOURCE)
    	if(etimer_expired(&et)) {

    		/* PREPARES THE REQUEST */
    		coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
    		coap_set_header_uri_path(request, service_urls[uri_switch]);

    		printf("--Requesting %s--\n", service_urls[uri_switch]);

    		/* MAKES A BLOCKING REQUEST */
    		COAP_BLOCKING_REQUEST(&server_ep, request,client_chunk_handler); //then answer is piggybagged (?)
    		printf("\n--Done--\n");
    		//SWITCHs TO NEXT GET RESOURCE
    		uri_switch = 5;  // loop if there are more than one GET resource
    		etimer_reset(&et);
    	}
    //#endif /* PLATFORM_HAS_BUTTON */

   			}

  PROCESS_END();
}





