/*
 * Copyright (c) 2015, SICS Swedish ICT.
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
 */
/**
 * \file
 *         A RPL+TSCH node able to act as either a simple node (6ln),
 *         DAG Root (6dr) or DAG Root with security (6dr-sec)
 *         Press use button at startup to configure.
 *
 * \author Simon Duquennoy <simonduq@sics.se>
 *
 * \file sender 
 * \modify enable application packet on RPL-TSCH by combining
 *  the rpl-tsch example and the simple-udp-rpl example
 * \author Tada Matz
 */

#include "contiki.h"

#define DEBUG DEBUG_FULL
#include "net/ip/uip-debug.h"

#include "node-id.h"
#include "net/rpl/rpl.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/mac/tsch/tsch.h"
#include "net/rpl/rpl-private.h"

#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "simple-udp.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#if WITH_ORCHESTRA
#include "orchestra.h"
#endif /* WITH_ORCHESTRA */
#if WITH_STATIC_SCHEDULER
#include "static-scheduler.h"
#endif /* WITH_STATIC_SCHEDULER */
#ifdef WITH_POWERTRACE
#include "powertrace.h"
#endif //WITH_POWERTRACE
#ifdef WITH_THUNDER
#include "net/rpl/rpl-private.h"
#include "thunder.h"
#endif //WITH_THUNDER
#ifdef WITH_LEAPFROG
#include "leapfrog.h"
#endif //WITH_LEAPFROG


//#define CONFIG_VIA_BUTTON PLATFORM_HAS_BUTTON
//#if CONFIG_VIA_BUTTON
//#include "button-sensor.h"
//#endif /* CONFIG_VIA_BUTTON */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


#define UDP_PORT 1234
#define SERVICE_ID 190

static struct simple_udp_connection unicast_connection;

/*---------------------------------------------------------------------------*/
/*--------------------------- process declaration ---------------------------*/
/*---------------------------------------------------------------------------*/


PROCESS(unicast_sender_process, "Unicast sender example process");
PROCESS(tsch_schedule_print_process, "Schedule process");

//for sender
#ifdef WITH_LEAPFROG
extern char leapfrog_data_counter;
extern char leapfrog_elimination_id_array[LEAPFROG_NUM_NODE];
extern char leapfrog_layer;
#endif //WITH_LEAFPROG

/* ----------------- stable timer start ----------------- */
#ifdef WITH_STABLETIMER
int stable_flag = 0;
PROCESS(stable_timer_process, "Stable timer process");
#endif //WITH_STABLETIMER

#ifdef WITH_STABLETIMER
#ifdef WITH_LEAPFROG
PROCESS(node_process, "RPL Node sender leapfrog");
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &stable_timer_process, &tsch_schedule_print_process);
#else //WITH_LEAPFROG
PROCESS(node_process, "RPL Node sender leapfrog");
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &stable_timer_process, &tsch_schedule_print_process);
#endif //WITH_LEAPFROG
#else //WITH_STABLETIMER
#ifdef WITH_LEAPFROG
PROCESS(node_process, "RPL Node sender leapfrog");
#if CONFIG_VIA_BUTTON
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &tsch_schedule_print_process);
#else /* CONFIG_VIA_BUTTON */
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &tsch_schedule_print_process);
#endif /* CONFIG_VIA_BUTTON */
#else /*WITH_LEAPFROG*/
PROCESS(node_process, "RPL Node sender leapfrog");
#if CONFIG_VIA_BUTTON
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &tsch_schedule_print_process);
#else /* CONFIG_VIA_BUTTON */
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process, &tsch_schedule_print_process);
#endif /* CONFIG_VIA_BUTTON */
#endif /*WITH_LEAPFROG*/
#endif // WITH_STABLETIMER

/*---------------------------------------------------------------------------*/
static void
print_network_status(void)
{
  int i;
  uint8_t state;
  uip_ds6_defrt_t *default_route;
#if RPL_WITH_STORING
  uip_ds6_route_t *route;
#endif /* RPL_WITH_STORING */
#if RPL_WITH_NON_STORING
  rpl_ns_node_t *link;
#endif /* RPL_WITH_NON_STORING */

  PRINTF("--- Network status ---\n");

  /* Our IPv6 addresses */
  PRINTF("- Server IPv6 addresses:\n");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINTF("-- ");
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
    }
  }

  /* Our default route */
  PRINTF("- Default route:\n");
  default_route = uip_ds6_defrt_lookup(uip_ds6_defrt_choose());
  if(default_route != NULL) {
    PRINTF("-- ");
    PRINT6ADDR(&default_route->ipaddr);
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)default_route->lifetime.interval);
  } else {
    PRINTF("-- None\n");
  }

#if RPL_WITH_STORING
  /* Our routing entries */
  PRINTF("- Routing entries (%u in total):\n", uip_ds6_route_num_routes());
  route = uip_ds6_route_head();
  while(route != NULL) {
    PRINTF("-- ");
    PRINT6ADDR(&route->ipaddr);
    PRINTF(" via ");
    PRINT6ADDR(uip_ds6_route_nexthop(route));
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)route->state.lifetime);
    route = uip_ds6_route_next(route);
  }
#endif

#if RPL_WITH_NON_STORING
  /* Our routing links */
  PRINTF("- Routing links (%u in total):\n", rpl_ns_num_nodes());
  link = rpl_ns_node_head();
  while(link != NULL) {
    uip_ipaddr_t child_ipaddr;
    uip_ipaddr_t parent_ipaddr;
    rpl_ns_get_node_global_addr(&child_ipaddr, link);
    rpl_ns_get_node_global_addr(&parent_ipaddr, link->parent);
    PRINTF("-- ");
    PRINT6ADDR(&child_ipaddr);
    if(link->parent == NULL) {
      memset(&parent_ipaddr, 0, sizeof(parent_ipaddr));
      PRINTF(" --- DODAG root ");
    } else {
      PRINTF(" to ");
      PRINT6ADDR(&parent_ipaddr);
    }
    PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)link->lifetime);
    link = rpl_ns_node_next(link);
  }
#endif

  PRINTF("----------------------\n");
}
/*---------------------------------------------------------------------------*/
static void
net_init(uip_ipaddr_t *br_prefix)
{
  uip_ipaddr_t global_ipaddr;

  if(br_prefix) { /* We are RPL root. Will be set automatically
                     as TSCH pan coordinator via the tsch-rpl module */
    memcpy(&global_ipaddr, br_prefix, 16);
    uip_ds6_set_addr_iid(&global_ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&global_ipaddr, 0, ADDR_AUTOCONF);
    rpl_set_root(RPL_DEFAULT_INSTANCE, &global_ipaddr);
    rpl_set_prefix(rpl_get_any_dag(), br_prefix, 64);
    rpl_repair_root(RPL_DEFAULT_INSTANCE);
  }

  NETSTACK_MAC.on();
}
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  PRINTF("sender: received from ");
//  uip_debug_ipaddr_print(sender_addr);
//  PRINTF(" on port %d from port %d with length %d: '%s'\n", receiver_port, sender_port, datalen, data);
  PRINTF("ID:%d l:%d '%s'\n", sender_addr->u8[15], datalen, data); //make it shorter
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(node_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  /* 3 possible roles:
   * - role_6ln: simple node, will join any network, secured or not
   * - role_6dr: DAG root, will advertise (unsecured) beacons
   * - role_6dr_sec: DAG root, will advertise secured beacons
   * */
  static int is_coordinator = 0;
  static enum { role_6ln, role_6dr, role_6dr_sec } node_role;
  node_role = role_6ln;

  int coordinator_candidate = 0;

#ifdef CONTIKI_TARGET_Z1
  /* Set node with MAC address c1:0c:00:00:00:00:01 as coordinator,
   * convenient in cooja for regression tests using z1 nodes
   * */
  extern unsigned char node_mac[8];
  unsigned char coordinator_mac[8] = { 0xc1, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

  coordinator_candidate = (memcmp(node_mac, coordinator_mac, 8) == 0);
#elif CONTIKI_TARGET_COOJA
  coordinator_candidate = (node_id == 1);
#endif

  if(coordinator_candidate) {
    if(LLSEC802154_ENABLED) {
      node_role = role_6dr_sec;
    } else {
      node_role = role_6dr;
    }
  } else {
    node_role = role_6ln;
  }

#if CONFIG_VIA_BUTTON
  {
#define CONFIG_WAIT_TIME 5

    SENSORS_ACTIVATE(button_sensor);

    while(!etimer_expired(&et)) {
      printf("Init: current role: %s. Will start in %u seconds. Press user button to toggle mode.\n",
             node_role == role_6ln ? "6ln" : (node_role == role_6dr) ? "6dr" : "6dr-sec",
             CONFIG_WAIT_TIME);
      PROCESS_WAIT_EVENT_UNTIL(((ev == sensors_event) &&
                                (data == &button_sensor) && button_sensor.value(0) > 0)
                               || etimer_expired(&et));
      if(ev == sensors_event && data == &button_sensor && button_sensor.value(0) > 0) {
        node_role = (node_role + 1) % 3;
        if(LLSEC802154_ENABLED == 0 && node_role == role_6dr_sec) {
          node_role = (node_role + 1) % 3;
        }
        etimer_restart(&et);
      }
    }
  }

#endif /* CONFIG_VIA_BUTTON */

  printf("Init: node starting with role %s\n",
         node_role == role_6ln ? "6ln" : (node_role == role_6dr) ? "6dr" : "6dr-sec");

  tsch_set_pan_secured(LLSEC802154_ENABLED && (node_role == role_6dr_sec));
  is_coordinator = node_role > role_6ln;

  if(is_coordinator) {
    uip_ipaddr_t prefix;
    uip_ip6addr(&prefix, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    net_init(&prefix);
  } else {
    net_init(NULL);
  }

#if WITH_ORCHESTRA
  orchestra_init();
#endif /* WITH_ORCHESTRA */

#if WITH_STATIC_SCHEDULER
  static_scheduler_init();
#endif /* WITH_STATIC_SCHEDULER */

#ifdef WITH_THUNDER //added by TadaMatz 1/Sep/2016
  thunder_init();
#endif //WITH_THUNDER

#ifdef WITH_POWERTRACE
  powertrace_start(POWERTRACE_INTERVAL);
#endif //WITH_POWERTRACE

#ifdef WITH_LEAPFROG
  leapfrog_init();
  leapfrog_layer = 1; //if sender, this is necessary.
#endif //WITHLEAPFROG

  /* Print out routing tables every minute */
  etimer_set(&et, CLOCK_SECOND * 60);
  while(1) {
    print_network_status();
    PROCESS_YIELD_UNTIL(etimer_expired(&et));
    etimer_reset(&et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_sender_process, ev, data)
{
  static struct etimer data_periodic_timer_offset, data_periodic_timer;
  //static struct etimer data_send_timer;
  uip_ipaddr_t *addr;


  PROCESS_BEGIN();

  simple_udp_register(&unicast_connection, UDP_PORT, NULL, UDP_PORT, receiver);
  
  //slide timer added 31/Aug/2016 avoid collision with network print
//  etimer_set(&data_periodic_timer, DATA_SEND_SLIDE_TIME);
//  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&data_periodic_timer));

  // etimer_set(&data_periodic_timer_offset, (13*CLOCK_SECOND)*(node_id-8));
  // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&data_periodic_timer_offset));
if(node_id==2){
	etimer_set(&data_periodic_timer, DATA_SEND_INTERVAL);
}else if(node_id == 8){
	etimer_set(&data_periodic_timer, 2*DATA_SEND_INTERVAL);
}else{
	etimer_set(&data_periodic_timer, DATA_SEND_INTERVAL*node_id);
}

  while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&data_periodic_timer));
      etimer_reset(&data_periodic_timer);
#ifdef WITH_STABLETIMER    
    if(stable_flag){
#else //WITH_STABLETIMER
    if(tsch_is_associated){
#endif //WITH_STABLETIMER
      /*--- target address decision ---*/
      /*-- to registered target with servreg_hack --*/
      //addr = servreg_hack_lookup(SERVICE_ID);
      /*-- to default route --*/
      //uip_ds6_defrt_t *default_route;
      //default_route = uip_ds6_defrt_lookup(uip_ds6_defrt_choose());
      //if(default_route != NULL) addr = &default_route->ipaddr;
      //else addr = NULL;
      /*-- decide by address directory--*/
      uip_ipaddr_t temp_ipaddr;

      uip_ip6addr(&temp_ipaddr,0xfd00,0,0,0,0x0201,1,1,1);
      addr = &temp_ipaddr;
      /*-- linklocal rplnodes mcast --*/
      //uip_ipaddr_t temp_ipaddr;
      //uip_ip6addr(&temp_ipaddr, 0xff02,0,0,0,0,0,0,0x001a);
      //addr = &temp_ipaddr;
      /*-- to default parent --*/
      //addr = rpl_get_parent_ipaddr(default_instance->current_dag->preferred_parent); 

//      tsch_schedule_print();

      /*--- sending ---*/ 
      if(addr != NULL) {
        static unsigned int message_number;
        char buf[20];
//        PRINTF("XXXXXX\n");
//        __asm__("int $3");

#ifdef WITH_LEAPFROG
        sprintf(buf, "%c%c%s%04d", LEAPFROG_DATA_HEADER, leapfrog_data_counter + LEAPFROG_BEACON_OFFSET, DATA_PACKET_MARKER, message_number);
        //uip_ipaddr_t * my_addr;
        //my_addr = &uip_ds6_if.addr_list[2].ipaddr; //get own ID. [2] seems to be default
        //if(my_addr != NULL){
        //  leapfrog_elimination_id_array[(int)addr->u8[15]] = leapfrog_data_counter;
        //}
        leapfrog_elimination_id_array[node_id - 1] = leapfrog_data_counter; //to do elimination
        leapfrog_data_counter++;
        if(leapfrog_data_counter >= LEAPFROG_DATA_COUNTER_MAX) leapfrog_data_counter = 0; //leapfrog_data_counter does NOT reach LEAPFROG_DATA_COUNTER_MAX
#else
        sprintf(buf, "%s%04d%u", DATA_PACKET_MARKER, message_number,node_id);
#endif
        PRINTF("sender: Sending '%s' data_packet_id: %04d at %u\n", buf, message_number,clock_seconds());
        //uip_debug_ipaddr_print(addr);
//        PRINTF(" '");
//        PRINTF(buf);
//        PRINTF("'\n");
        message_number++;
        simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, addr);
      } else {
        PRINTF("DATA: addr is NULL!!");
      }
    } //if(tsch_is_associated)
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
#ifdef WITH_STABLETIMER
/* ----------------- stable_timer process start ----------------- */
PROCESS_THREAD(stable_timer_process, ev, data)
{
  static struct etimer stable_timer;

  PROCESS_BEGIN();
  etimer_set(&stable_timer, STABLE_TIME); //15min
  PRINTF("Set Stable timer\n");
  
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&stable_timer));
  etimer_stop(&stable_timer);
   
  stable_flag = 1;
  PRINTF("Stable timer expired!! Start to send application traffic\n");
  PROCESS_EXIT();

  PROCESS_END();
}
/* ----------------- stable_timer process end ----------------- */
#endif //WITH_STABLE_TIMER
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tsch_schedule_print_process, ev, data)
{
  static struct etimer data_periodic_timer;
  uip_ipaddr_t *addr;

  PROCESS_BEGIN();

  etimer_set(&data_periodic_timer, TSCH_SCHEDULE_PRINT_INTERVAL);
  while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&data_periodic_timer));
      etimer_reset(&data_periodic_timer);
      tsch_schedule_print();
  }

  PROCESS_END();
}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
