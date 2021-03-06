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
 *
 * \modified by
 * 			Jmdf <jdelfiore2002@hotmail.com>
 *
 * \author
 * 			TadaMatz
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

//modified by TadaMatz 1/July/2016
//#define IGNORE_TSCH_RESEND //ignore TSCH resending in tsch-slot-operation
//#define TSCH_CONF_MAC_MAX_FRAME_RETRIES 1//here can control the number of re-transmission in TSCH
#define TSCH_SCHEDULE_CONF_MAX_LINKS 64 // To allow upto 64 slots to be used in the slotframe ...Added by Jmdf

#define DATA_PACKET_MARKER "SxKkf0KzxcQ0B"
#define DATA_PACKET_MARKER_LEN 13

#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1
//#define WITH_LEAPFROG
//#define WITH_LEAPFROG_BEACON_SLOT //Store timelots dedicated for Leapfrog beacon. This can be only used with Thunder as a scheduler
//#define WITH_OVERHEARING //Enable Overhearing feature inluding both timeslot and slot-operation. This can be only used with Thunder as a scheduler

//#define WITH_THUNDER //do not use Orchestra. Fixed TSCH schedule
//#define WITH_THUNDER_ADAPTIVE_EB_SLOT //make EB slot can be changed based on network

//#define WITH_POWERTRACE // for power trace
//#define WITH_STABLETIMER // for waiting application traffic until network stabilized


//#define WITH_EXTRA_INFORMATION // for printing more information

//#define WITH_ORCHESTRA 1 //if 1, enable Orchestra
//#define ORCHESTRA_CONF_UNICAST_SENDER_BASED 1
//#define ORCHESTRA_CONF_COLLISION_FREE_HASH 1
//#define WITH_LEAPFROG_TSCH //if defined, make the new timeslot for alt-parent
//#define CONDUCT_ORCHESTRA //Scheduling based on Orchestra idea. Each slotframe has different lengths
//#define SEPARATE_ORCHESTRA //Schduling based on Orchestra idea. All slotframes have same slotframe lengths
//#define WITH_DATA_SLOT //make timeslot dedicated for application traffic. must be used with SEPARATE_ORCHESTRA

//udp data
//#define DATA_SEND_SLIDE_TIME (30 * CLOCK_SECOND)
#define DATA_SEND_INTERVAL   (DATA_SEND_INTERVAL_TIME_IN_S * CLOCK_SECOND) //60 is nice for simulation
//#define DATA_SEND_TIME   (random_rand() % (DATA_SEND_INTERVAL))
//#define DATA_SEND_TIME   (DATA_SEND_INTERVAL) //make it periodical
#define TSCH_SCHEDULE_PRINT_INTERVAL (TSCH_SCHEDULE_PRINT_INTERVAL_IN_S* CLOCK_SECOND) //every 10 minutes


#ifdef WITH_POWERTRACE
#define POWERTRACE_INTERVAL (60 * CLOCK_SECOND)
#endif //WITH_POWERTRACE

#ifdef WITH_STABLETIMER
#define STABLE_TIME (DATA_SEND_START_TIME_IN_S * CLOCK_SECOND) //1800 is nice for simulation
#endif //WITH_STABLETIMER

/* Set to run orchestra */
//#ifndef WITH_ORCHESTRA
//#define WITH_ORCHESTRA 0
//#endif /* WITH_ORCHESTRA */

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 0
#endif /* WITH_SECURITY */

/*******************************************************/
/********************* Enable TSCH *********************/
/*******************************************************/

/* Netstack layers */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     tschmac_driver
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nordc_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154

/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

/* TSCH and RPL callbacks */
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network

#if CONTIKI_TARGET_CC2538DK
/* Needed for CC2538 platforms only */
/* For TSCH we have to use the more accurate crystal oscillator
 * by default the RC oscillator is activated */
#undef SYS_CTRL_CONF_OSC32K_USE_XTAL
#define SYS_CTRL_CONF_OSC32K_USE_XTAL 1
#endif /* CONTIKI_TARGET_CC2538DK */

#if CONTIKI_TARGET_Z1
/* Needed for cc2420 platforms only */
/* Disable DCO calibration (uses timerB) */
#undef DCOSYNCH_CONF_ENABLED
#define DCOSYNCH_CONF_ENABLED 0
/* Enable SFD timestamps (uses timerB) */
#undef CC2420_CONF_SFD_TIMESTAMPS
#define CC2420_CONF_SFD_TIMESTAMPS 1
#endif /* CONTIKI_TARGET_Z1 */

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

/* As we are having trouble with timing and clock drifts,
 * for the moment the solution is to configure the nodes
 * to autoselect their time source instead of just listening
 * to their RLP parent. The other solution could be sending
 * EB's more oftenly.*/
//#define TSCH_CONF_EB_PERIOD (1 * CLOCK_SECOND) 		// Added by Jmdf
//#define TSCH_CONF_AUTOSELECT_TIME_SOURCE 0 			// Added by Jmdf

/* TSCH logging. 0: disabled. 1: basic log. 2: with delayed
 * log messages from interrupt */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 1

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xabcd

/* Do not start TSCH at init, wait for NETSTACK_MAC.on() */
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 0

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3

#if WITH_SECURITY

/* Enable security */
#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED 1
/* TSCH uses explicit keys to identify k1 and k2 */
#undef LLSEC802154_CONF_USES_EXPLICIT_KEYS
#define LLSEC802154_CONF_USES_EXPLICIT_KEYS 1
/* TSCH uses the ASN rather than frame counter to construct the Nonce */
#undef LLSEC802154_CONF_USES_FRAME_COUNTER
#define LLSEC802154_CONF_USES_FRAME_COUNTER 0

#endif /* WITH_SECURITY */

#if WITH_ORCHESTRA

/* See apps/orchestra/README.md for more Orchestra configuration options */
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0 /* No 6TiSCH minimal schedule */
#define TSCH_CONF_WITH_LINK_SELECTOR 1 /* Orchestra requires per-packet link selection */
/* Orchestra callbacks */
#define TSCH_CALLBACK_NEW_TIME_SOURCE orchestra_callback_new_time_source
#define TSCH_CALLBACK_PACKET_READY orchestra_callback_packet_ready
#define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK orchestra_callback_child_added
#define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK orchestra_callback_child_removed

#endif /* WITH_ORCHESTRA */

//added by TadaMatz 2016/Sep/1
#ifdef WITH_THUNDER

#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0 /* No 6TiSCH minimal schedule */
#define TSCH_CONF_WITH_LINK_SELECTOR 1 /* requires per-packet link selection */
/* Thunder callbacks */
#define TSCH_CALLBACK_PACKET_READY thunder_callback_packet_ready //set timeslot and frame to packefbuf attributes

#ifdef WITH_THUNDER_ADAPTIVE_EB_SLOT
#define TSCH_CALLBACK_NEW_TIME_SOURCE thunder_callback_new_time_source //only for eb
#endif //WITH_THUNDER_ADAPTIVE_EB_SLOT

#endif //WITH_THUNDER
/*******************************************************/
/************* Other system configuration **************/
/*******************************************************/

#if CONTIKI_TARGET_Z1
/* Save some space to fit the limited RAM of the z1 */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP 0
#undef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM 3
#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM  8
#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8
#undef UIP_CONF_ND6_SEND_NS
#define UIP_CONF_ND6_SEND_NS 0
#undef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG 0

#if WITH_SECURITY
/* Note: on sky or z1 in cooja, crypto operations are done in S/W and
 * cannot be accommodated in normal slots. Use 65ms slots instead, and
 * a very short 6TiSCH minimal schedule length */
#undef TSCH_CONF_DEFAULT_TIMESLOT_LENGTH
#define TSCH_CONF_DEFAULT_TIMESLOT_LENGTH 65000
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 2
/* Reduce log level to make space for security on z1 */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 0
#endif /* WITH_SECURITY */

#endif /* CONTIKI_TARGET_Z1 */

#if CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL || \
  CONTIKI_TARGET_OPENMOTE_CC2538
#define TSCH_CONF_HW_FRAME_FILTERING    0
#endif /* CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL \
       || CONTIKI_TARGET_OPENMOTE_CC2538 */

#if CONTIKI_TARGET_COOJA
#define COOJA_CONF_SIMULATE_TURNAROUND 0
#endif /* CONTIKI_TARGET_COOJA */

#endif /* __PROJECT_CONF_H__ */
