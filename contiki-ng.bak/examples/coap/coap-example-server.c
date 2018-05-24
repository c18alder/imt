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
 *      Erbium (Er) CoAP Engine example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "coap-engine.h"
#include "net/routing/routing.h"

#if PLATFORM_SUPPORTS_BUTTON_HAL
#include "dev/button-hal.h"
#else
#include "dev/button-sensor.h"
#endif


// FOR UART WORKING AND DEBUGING

#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/serial-line.h"
#include <stdint.h>



#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


// PARA LA UART // Testing



extern char battery_value_from_arduino[10];
char battery_value_from_arduino[10];
extern char speed_value_from_arduino[10];
char speed_value_from_arduino[10];
extern char kilometrage_value_from_arduino[10];
char kilometrage_value_from_arduino[10];
/*

*/
/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern coap_resource_t
  res_car_battery,
  res_car_kilometrage,
  res_car_speed,


  res_hello,
  res_mirror,
  res_chunks,
  res_separate,
  //res_push,
  res_event,
  res_sub,
  res_b1_sep_b2;




#if PLATFORM_HAS_LEDS
extern coap_resource_t res_leds, res_toggle;
#endif
#if PLATFORM_HAS_LIGHT
#include "dev/light-sensor.h"
extern coap_resource_t res_light;
#endif
#if PLATFORM_HAS_BATTERY
#include "dev/battery-sensor.h"
extern coap_resource_t res_battery;
#endif
#if PLATFORM_HAS_TEMPERATURE
#include "dev/temperature-sensor.h"
extern coap_resource_t res_temperature;
#endif
/*
extern coap_resource_t res_battery;
#endif
#if PLATFORM_HAS_RADIO
extern coap_resource_t res_radio;
#endif
#if PLATFORM_HAS_SHT11
#include "dev/sht11/sht11-sensor.h"
extern coap_resource_t res_sht11;
#endif
*/

PROCESS(er_example_server, "Erbium Example Server");
AUTOSTART_PROCESSES(&er_example_server);

PROCESS_THREAD(er_example_server, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

  //UART
  char *rxdata;
  char aux[10];


/////////////

  PRINTF("Starting Erbium Example Server\n");

#ifdef RF_CHANNEL
  PRINTF("RF channel: %u\n", RF_CHANNEL);
#endif
#ifdef IEEE802154_PANID
  PRINTF("PAN ID: 0x%04X\n", IEEE802154_PANID);
#endif

  PRINTF("uIP buffer: %u\n", UIP_BUFSIZE);
  PRINTF("LL header: %u\n", UIP_LLH_LEN);
  PRINTF("IP+UDP header: %u\n", UIP_IPUDPH_LEN);
  PRINTF("CoAP max chunk: %u\n", COAP_MAX_CHUNK_SIZE);

  /* Initialize the REST engine. */
  coap_engine_init();

  /*
   * Bind the resources to their Uri-Path.
   * WARNING: Activating twice only means alternate path, not two instances!
   * All static variables are the same for each URI path.
   */
  //UART RESOURCE
  coap_activate_resource(&res_car_speed, "Car_Speed");
  coap_activate_resource(&res_car_kilometrage, "Car_Kilometrage");
  coap_activate_resource(&res_car_battery, "Car_Battery");
  ////

  coap_activate_resource(&res_hello, "test/hello");
 coap_activate_resource(&res_mirror, "debug/mirror");
 coap_activate_resource(&res_chunks, "test/chunks");
 coap_activate_resource(&res_separate, "test/separate");
 //coap_activate_resource(&res_push, "test/push");
#if PLATFORM_HAS_BUTTON
 coap_activate_resource(&res_event, "sensors/button");
#endif /* PLATFORM_HAS_BUTTON */
 coap_activate_resource(&res_sub, "test/sub");
 coap_activate_resource(&res_b1_sep_b2, "test/b1sepb2");
#if PLATFORM_HAS_LEDS
/*  coap_activate_resource(&res_leds, "actuators/leds"); */
  coap_activate_resource(&res_toggle, "actuators/toggle");
#endif
#if PLATFORM_HAS_LIGHT
  coap_activate_resource(&res_light, "sensors/light");
  SENSORS_ACTIVATE(light_sensor);
#endif
#if PLATFORM_HAS_BATTERY
  coap_activate_resource(&res_battery, "sensors/battery");
  SENSORS_ACTIVATE(battery_sensor);
#endif
#if PLATFORM_HAS_TEMPERATURE
  coap_activate_resource(&res_temperature, "sensors/temperature");
  SENSORS_ACTIVATE(temperature_sensor);
#endif
/*
#if PLATFORM_HAS_RADIO
  coap_activate_resource(&res_radio, "sensors/radio");
#endif
#if PLATFORM_HAS_SHT11
  coap_activate_resource(&res_sht11, "sensors/sht11");
  SENSORS_ACTIVATE(sht11_sensor);
#endif
*/



  NETSTACK_ROUTING.root_start(); // CREAT DAG NET , WITH THIS MOTE AS A ROOT



char *pch;


  /* Define application-specific events here. */
  while(1) {
   // PROCESS_WAIT_EVENT();
//#if PLATFORM_HAS_BUTTON
//#if PLATFORM_SUPPORTS_BUTTON_HAL
  //  if(ev == button_hal_release_event) {
//#else
   // if(ev == sensors_event && data == &button_sensor) {
//#endif


	 PROCESS_WAIT_EVENT();

    if(ev == serial_line_event_message) {
    	   leds_toggle(LEDS_RED);
    	   rxdata = data;
    	   strcpy(aux,data);


    	   printf("Resourse received over UART %s\n\n", rxdata);



    	   //CHECK WHICH RESOURCE AND SAVES IT ACCORDINGLY ( FULL FORMAT )
    	       	   pch= strtok (rxdata, "=");
    	       	   if(!(strcmp(pch,"km/h"))){
    	       		   pch=strtok(NULL,"}");
    	       		strcpy(speed_value_from_arduino,aux);
    	            res_car_speed.trigger();  //LLAMA al recurso si está siendo observado.


    	       	   }else if(!(strcmp(pch,"kms"))){
    	       		   pch=strtok(NULL,"}");
    	       		strcpy(kilometrage_value_from_arduino,aux);
    	       	   }else if(!(strcmp(pch,"%bat"))){
    	       		   pch=strtok(NULL,"}");
    	       		strcpy(battery_value_from_arduino,aux);

    	       	   }  //ERROR CALLFUNCTION





    	   //printf("TESTING %s\n", pch);
    	   //printf("Decimal Value %d\n" , atoi(pch));
    	   printf("Last Speed Value %s\n" , speed_value_from_arduino);
    	   printf("Last Battery Value %s\n" , battery_value_from_arduino);
    	   printf("Last Kilometrage Value %s\n\n" , kilometrage_value_from_arduino);
    } // END IF-Event Uart






      /* Call the event_handler for this application-specific event. */
     // res_event.trigger();

      /* Also call the separate response example handler. */
    //  res_separate.resume();
    //}
//#endif /* PLATFORM_HAS_BUTTON */
  }                             /* while (1) */

  PROCESS_END();
}
