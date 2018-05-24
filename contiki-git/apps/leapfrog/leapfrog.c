/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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
 *
 */

/**
 * \file
 *         Leapfrog Collaboration
 *
 * \modified by
 * 			Jmdf <jdelfiore2002@hotmail.com>
 * \author
 *          TadaMatz <Telecom Bretange, Keio University>
 */

#include "contiki.h"
#include "leapfrog.h"
#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

char leapfrog_parent_id = 0;
char leapfrog_grand_parent_id = 0;
char leapfrog_alt_parent_id = 0;

char leapfrog_possible_parent_num = 0;
char leapfrog_possible_parent_id_array[LEAPFROG_NUM_NEIGHBOR_NODE] = {0};

char leapfrog_data_counter = 0;
char leapfrog_elimination_id_array[LEAPFROG_NUM_NODE] = {LEAPFROG_DATA_COUNTER_MAX};

extern rpl_instance_t * default_instance;
static struct simple_udp_connection leapfrog_unicast_connection;

PROCESS(leapfrog_beaconing_process, "Leapfrog beaconing process");

// #ifdef WITH_LEAPFROG_TSCH
// // extern struct tsch_slotframe *sf_lfat; //leapfrog alt traffic
// linkaddr_t alt_parent_linkaddr = {{0xc1, 0x0c, 0, 0, 0, 0, 0, 0}};
// #endif /*WITH_LEAPFROG_TSCH*/

char leapfrog_layer = 0; //default for 0. sender should be 1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void
leapfrog_receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
//#ifdef WITH_LEAPFROG //for beaconing
  if(datalen > 0 && data[0] == LEAPFROG_BEACON_HEADER){
    char temp_sid = 0; //sender id of packet
    char temp_pid = 0; //sender's parent id
//  char temp_gid = 0; //sender's grand parent id
    char temp_aid = 0; //sender's alt parent id
    char temp_layer = 0; //sender's layer
    temp_sid = sender_addr->u8[15]; //get most least byte. must be modified to store whole address
    temp_pid = data[2] - LEAPFROG_BEACON_OFFSET;
//    temp_gid = data[4] - LEAPFROG_BEACON_OFFSET;
    temp_aid = data[6] - LEAPFROG_BEACON_OFFSET;
    temp_layer = data[8] - LEAPFROG_BEACON_OFFSET;
    char temp_pps_num;
    char temp_pps_str[LEAPFROG_NUM_NEIGHBOR_NODE];
    int temp_pps_itr;
    temp_pps_num = data[10] - LEAPFROG_BEACON_OFFSET;
    for(temp_pps_itr = 0; temp_pps_itr < (int)temp_pps_num; temp_pps_itr++){ //do nothing if temp_pps_num = 0
      temp_pps_str[temp_pps_itr] = data[10 + 1 + temp_pps_itr];
    }
    // temp_pps_str[temp_pps_itr] = '\0';

    //printf("LEAPFROG: receive beacon S%dP%dGP%dAP%d#%dPPs%s\n", temp_sid, temp_pid, temp_gid, temp_aid, temp_pps_num, temp_pps_str);
    // printf("LEAPFROG: receive beacon '%s'\n", data);
    
    //preparing own informaiton
    char my_id = node_id; // my ID
    char my_pid = 0; // my parent ID
    uip_ipaddr_t * addr = 0;
    if (default_instance != NULL && default_instance->current_dag != NULL &&
        default_instance->current_dag->preferred_parent != NULL)
      addr = rpl_get_parent_ipaddr(default_instance->current_dag->preferred_parent);
    if(addr != NULL){
      my_pid = addr->u8[15];
    }


    /* First, we need to understand that:
     * 		A) LP is our old DP
     * 		B) DP is our new DP
     *
     * Then, we check if our DP has changed, meaning that LP != DP. If this is the case...
     *		1) We don't know any longer our DGP, so we reset it.
     *		2) We have to update LP, the old DP. But, we want to avoid the case where LP = DP = AP.
     *  	   So, if AP = DP, we are just going to flip the old DP with the AP. For this, we update
     *  	   AP making AP = LP. As we still have not updated the old DP, then for the moment we have LP = AP != DP.
     * 		3) Finally, we can now make LP = DP.
     */

    if(leapfrog_parent_id != my_pid){ //new parent and reset P, GP, AP
    		leapfrog_grand_parent_id = 0;
    		if (leapfrog_alt_parent_id != 0 && leapfrog_alt_parent_id == my_pid)
    			leapfrog_alt_parent_id = leapfrog_parent_id;
    		leapfrog_parent_id = my_pid;
        }

    /* As now LP = DP, we can use either to refer to the updated parent
     *
     * Getting Grand Parent
     * If temp_sid is our DP, then if he has a temp_pid, this temp_pid is our DGP.
     */

  	if(temp_sid == leapfrog_parent_id && temp_pid > 0) // Condition removed: temp_pid != my_pid ..why would it be here?
  		leapfrog_grand_parent_id = temp_pid;

   /* Getting Alternate Parent
    * If we have a DGP and the sender has his own DP. If the sender's DP is our DGP, he could be chosen as our AP,
    * unless he is in fact our DP.
    *
    * If the sender's temp_sid is not our DP, then we check among his PPs if our DGP is included. If that is
    * the case, then he can be our AP.
    *
    * Questions for the future...
    *	1) Why do we change the AP every time a new temp_sid could be a possible AP?
    * 	2) temp_pid is one of the PPs of temp_sid, so why don't we check the possible PPs at once and that's it?
    */

    if(leapfrog_grand_parent_id > 0 && temp_pid > 0 && leapfrog_grand_parent_id == temp_pid && leapfrog_parent_id != temp_sid)
    	leapfrog_alt_parent_id = temp_sid;
    else if(leapfrog_parent_id != temp_sid){
    	  for(temp_pps_itr = 0; temp_pps_itr < (int)temp_pps_num; temp_pps_itr++){
    		  if(leapfrog_grand_parent_id == temp_pps_str[temp_pps_itr] - LEAPFROG_BEACON_OFFSET){
    			  leapfrog_alt_parent_id = temp_sid;
    			  break;
    		  }
    	  }
    }

    /*
     * Getting Layer case we are DP of sender
     *
     * If temp_sid has a DP called temp_pid, and I am that his parent,
     * then I am one step above his temp_layer.
     *
     * Also, if our child has an AP, as we are his DP, we have to overhear that communication.
     *
     */

    if(temp_pid != 0 && my_id == temp_pid){
    	if(temp_layer > 0){
    		if(leapfrog_layer == 0 || leapfrog_layer > temp_layer + 1)
    			leapfrog_layer = temp_layer + 1;
        }
#ifdef WITH_OVERHEARING
    if(temp_aid != 0)
    	thunder_add_link(temp_sid, temp_aid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
#endif //WITH_OVERHEARING
    }   

    /*
     * Getting Layer case we are AP of sender
     *
     * If temp_sid has an AP called temp_aid, and I am this node,
     * then I am one step above his temp_layer.
     *
     * Also, as we are the AP, we have to overhear that communication with the DP.
     *
     */

    if(temp_aid != 0 && my_id == temp_aid){
    	if(temp_layer > 0){
    		if(leapfrog_layer == 0 || leapfrog_layer > temp_layer + 1)
    			leapfrog_layer = temp_layer + 1;
        }
#ifdef WITH_OVERHEARING
	thunder_add_link(temp_sid, temp_pid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
#endif //WITH_OVERHEARING
    }   

    /*
     * The only thing left is checking if the sender is one of our siblings.
     *
     * If sender's DP is the same as my DP, then we are siblings, so I have to
     * overhear the communication with his DP, and with his AP, if he has.
     *
     * If the sender's temp_pid is not our DP, then we check among his PPs if our DP is included. If that is
     * the case, then he is our sibling.
     *
    * Questions for the future...
    *	1) temp_pid is one of the PPs of temp_sid, so why don't we check the possible PPs at once and that's it?
    */

    if(temp_pid > 0 && temp_pid == leapfrog_parent_id){
#ifdef WITH_OVERHEARING
    thunder_add_link(temp_sid, temp_pid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
    if(temp_aid > 0)
    	thunder_add_link(temp_sid, temp_aid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
#endif //WITH_OVERHEARING
    }
    else if(temp_pid > 0 && temp_pps_num > 0){
      for(temp_pps_itr = 0; temp_pps_itr < (int)temp_pps_num; temp_pps_itr++){
        if(temp_pps_str[temp_pps_itr] - LEAPFROG_BEACON_OFFSET == leapfrog_parent_id){
#ifdef WITH_OVERHEARING
	thunder_add_link(temp_sid, temp_pid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
    if(temp_aid > 0)
    	thunder_add_link(temp_sid, temp_aid, LINK_OPTION_RX | LINK_OPTION_OVERHEARING);
#endif //WITH_OVERHEARING
          break;
        }
      }
    }

    //end of judging process by beacon.
    //print calculated information
    for(temp_pps_itr = 0; temp_pps_itr < leapfrog_possible_parent_num; temp_pps_itr++){
       temp_pps_str[temp_pps_itr] = leapfrog_possible_parent_id_array[temp_pps_itr] + LEAPFROG_BEACON_OFFSET;
    }
    temp_pps_str[temp_pps_itr] = '\0';
    printf("LEAPFROG: own P%d GP%d AP%d PPs%d:%s L%d\n", leapfrog_parent_id, leapfrog_grand_parent_id, leapfrog_alt_parent_id, leapfrog_possible_parent_num, temp_pps_str, leapfrog_layer);

  } //if(data[0] == LEAPFROG_BEACON_HEADER)
//#endif /*WITH_LEAPFROG*/
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(leapfrog_beaconing_process, ev, data)
{
  static struct etimer lf_beacon_periodic_timer;

  PROCESS_BEGIN();

  simple_udp_register(&leapfrog_unicast_connection, LEAPFROG_UDP_PORT, NULL, LEAPFROG_UDP_PORT, leapfrog_receiver);

  //slide timer added 5/Sep/2016 to avoid collision with network print and application data
  etimer_set(&lf_beacon_periodic_timer, LEAPFROG_SEND_SLIDE_TIME);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&lf_beacon_periodic_timer));
  etimer_set(&lf_beacon_periodic_timer, LEAPFROG_SEND_INTERVAL);

  while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&lf_beacon_periodic_timer));
      etimer_reset(&lf_beacon_periodic_timer);
      
      if(tsch_is_associated){

    	  uip_ipaddr_t temp_ipaddr;
          uip_create_linklocal_rplnodes_mcast(&temp_ipaddr);

          static unsigned int message_number;
          char buf[20];
          char possible_parent_str[1 + LEAPFROG_NUM_NEIGHBOR_NODE];

          possible_parent_str[0] = leapfrog_possible_parent_num + LEAPFROG_BEACON_OFFSET;
          int i;
          for(i = 0; i < leapfrog_possible_parent_num; i++){
              possible_parent_str[1 + i] = leapfrog_possible_parent_id_array[i] + LEAPFROG_BEACON_OFFSET;
          }

          sprintf(buf, "%cP%cG%cA%cL%cC%sN%d",
             LEAPFROG_BEACON_HEADER,
             leapfrog_parent_id + LEAPFROG_BEACON_OFFSET,
             leapfrog_grand_parent_id + LEAPFROG_BEACON_OFFSET,
             leapfrog_alt_parent_id + LEAPFROG_BEACON_OFFSET,
             leapfrog_layer + LEAPFROG_BEACON_OFFSET, //for layer
             possible_parent_str, //C for candidate
             message_number);
         // printf("LEAPFROG: Sending beacon to ");
         // uip_debug_ipaddr_print(addr);
         printf("LEAPFROG: Beacon ");
         uip_debug_ipaddr_print(&temp_ipaddr);
         printf(" '");
         printf(buf);
         printf("'\n");
         message_number++;
         // simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
         simple_udp_sendto(&leapfrog_unicast_connection, buf, strlen(buf) + 1, &temp_ipaddr);
         //simple_udp_sendto(&unicast_connection, buf, cnt, addr);
       } //if tsch_is_associated
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void
leapfrog_init()
{
  int initialize_elimination_itr = 0;
  for(initialize_elimination_itr = 0; initialize_elimination_itr < LEAPFROG_NUM_NODE; initialize_elimination_itr++){
    leapfrog_elimination_id_array[initialize_elimination_itr] = LEAPFROG_DATA_COUNTER_MAX; //Do not forget the initialization
  }

  process_start(&leapfrog_beaconing_process, NULL);
}
