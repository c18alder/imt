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
#include "net/routing/routing.h" //INCLUDE TO SET UP A FLOATING DODAG
#include "net/mac/tsch/tsch.h"   //INCLUDE TO TURN ON MAC LAYER



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


//max size for the home_made format: km/h=120}  size 8 EXCEPT CELLS Voltage



//TODO: ACK ARDUINO
//#define ARDUINO_ACK "ACKNOWLEDGED"
//#define ARDUINO_ERROR "WRONG_PACKET"  //GOT A UART PACKET BUT NOT THE CORRECT FORMAT (i.e not  kms=10)
//#define ARDUINO_ACK_SIZE ((sizeof(ARDUINO_ACK))-1)

// HERE IS GOING TO BE STORED THE LAST VALUE OF EACH RESOURCE (TODO: probably create a struct
//or vector of pointers, etc to store more values)

extern char battery_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
char battery_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
extern char speed_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
char speed_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
extern char kilometrage_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
char kilometrage_value_from_arduino[MAX_RESOURCE_FORMAT_SIZE];
extern short int cells_voltage_from_arduino[NUMBER_OF_CELLS];
short int cells_voltage_from_arduino[NUMBER_OF_CELLS];
extern char temp_one_from_arduino[NUMBER_OF_CELLS];
char temp_one_from_arduino[NUMBER_OF_CELLS];
extern char temp_two_from_arduino[NUMBER_OF_CELLS];
char temp_two_from_arduino[NUMBER_OF_CELLS];
extern char temp_three_from_arduino[NUMBER_OF_CELLS];
char temp_three_from_arduino[NUMBER_OF_CELLS];



/*
 * Resources to be activated need to be imported through the extern keyword.
 * The build system automatically compiles the resources in the corresponding sub-directory.
 */
extern coap_resource_t
  res_car_battery,
  res_car_kilometrage,
  res_car_speed,
  res_hello,
  res_car_cells,
  res_car_temp_one,
  res_car_temp_two,
  res_car_temp_three;


//UART SENT FUNCTION
unsigned int
uart1_send_bytes( char *s, unsigned int len) //JUST TO SEND ACK (to send packets use const unsigned char *s)
{
  unsigned int i = 0;

  while(s && *s != 0) {
    if(i >= len) {
      break;
    }
    uart_write_byte(1, *s++);
    i++;
  }
  return i;
}

//CHECK WHICH RESOURCE IS (depending of the KEY) AND SAVES IT ACCORDINGLY ( FULL FORMAT )
//carefull! strtok modifies rxdata
int handle_incoming_resource(char *in_data){

  	        	   char *pch;
  	        	   //char *aux2;
  	        	   char aux[MAX_RESOURCE_FORMAT_SIZE];
  	        	   int aux_atoi;

  	        	   strcpy(aux,in_data);
  	        	   pch= strtok (aux, "=.");   // "." is for cells

  	        	   	   if(!(strcmp(pch,"km/h"))){
  	        	   		   strcpy(speed_value_from_arduino,in_data);
  	        	   		   res_car_speed.trigger();  //CALL THE RESOURCE BEING OBSERVED (TODO: put a condition if "no observated resources", dont do this)
  	        	   	   }else if(!(strcmp(pch,"%bat"))){
  	        	   		   strcpy(battery_value_from_arduino,in_data);
  	        	   		   res_car_battery.trigger();
  	        	   	   }else if(!(strcmp(pch,"kms"))){
  	        	   		   strcpy(kilometrage_value_from_arduino,in_data);
  	        	   		   res_car_kilometrage.trigger();

  	        	   	   }else if(!(strcmp(pch,"ta"))){
  	        	   	  	   strcpy(temp_one_from_arduino,in_data);
  	          	   		   res_car_temp_one.trigger();
  	        	   	   }else if(!(strcmp(pch,"tb"))){
  	        	   		   strcpy(temp_two_from_arduino,in_data);
  	        	   		   res_car_temp_one.trigger();
  	        	   	   }else if(!(strcmp(pch,"tc"))){
  	        	   		   strcpy(temp_three_from_arduino,in_data);
  	        	   		   res_car_temp_one.trigger();

  	    	       	   }else if(!(strcmp(pch,"cs"))){
  	    	       		pch = strtok (NULL, "=.");
  	    	       		aux_atoi= atoi(pch);
  	    	       		pch = strtok (NULL, "=.");
  	    	       		cells_voltage_from_arduino[aux_atoi]=atoi(pch);
  	    	       	   }else{

  	        	   	 //IMPLEMENT ERROR CONTROl
  	    	       	   }

  	        	  return 0;
  	           }


PROCESS(er_example_server, "Erbium Example Server");
AUTOSTART_PROCESSES(&er_example_server);

PROCESS_THREAD(er_example_server, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

//UART RECEIVE
 char *rxdata;

/////////////
//DEBUG AT RESTART
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
  coap_activate_resource(&res_car_speed, "Car_Speed");
  coap_activate_resource(&res_car_kilometrage, "Car_Kilometrage");
  coap_activate_resource(&res_car_battery, "Car_Battery");
  coap_activate_resource(&res_car_cells, "Car_Cells");
  coap_activate_resource(&res_car_temp_one, "Car_Temp_One");
  coap_activate_resource(&res_car_temp_two, "Car_Temp_Two");
  coap_activate_resource(&res_car_temp_three, "Car_Temp_Three");



  //coap_activate_resource(&res_hello, "test/hello");


  // CREATes A FLOTING DODAG NET , WITH THIS MOTE AS A ROOT (Following  6TiSCH/simple-node/node.c example )
  NETSTACK_ROUTING.root_start();
  NETSTACK_MAC.on();

  /* Define application-specific events here. */
  while(1) {

	 PROCESS_WAIT_EVENT();
//INCOMING UART DATA
    if(ev == serial_line_event_message) {
    	   //lights toggle if some data is read at UART
    		leds_toggle(LEDS_RED);
    		rxdata = data;
    		PRINTF("Resourse received over UART %s\n\n", rxdata);

    		handle_incoming_resource(rxdata);

    		PRINTF("Last Speed Value %s\n" , strtok (speed_value_from_arduino, "}"));
    		PRINTF("Last Battery Value %s\n" , strtok (battery_value_from_arduino, "}"));
    		PRINTF("Last Kilometrage Value %s\n\n" , strtok (kilometrage_value_from_arduino, "}"));
    		PRINTF("Last Temps:  %s  %s  %s  \n\n" , strtok (temp_one_from_arduino, "}"),strtok (temp_two_from_arduino, "}"),strtok (temp_three_from_arduino, "}"));
    	 	for (int i=0 ; i<24; i++){
    	            	PRINTF(" %u , ", cells_voltage_from_arduino[i]);
    	              	}

    } // END INCOMING UART DATA

  }                             /* while (1) */

  PROCESS_END();
}
