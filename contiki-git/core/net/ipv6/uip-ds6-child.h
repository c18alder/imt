/*
 * Copyright (c) 2013, Swedish Institute of Computer Science.
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
 */

/**
 * \addtogroup uip6
 * @{
 */

/**
 * \file
 *    IPv6 Neighbor cache (link-layer/IPv6 address mapping)
 * \author Mathilde Durvy <mdurvy@cisco.com>
 * \author Julien Abeille <jabeille@cisco.com>
 * \author Simon Duquennoy <simonduq@sics.se>
 *
 */

#ifndef UIP_DS6_CHILD_H_
#define UIP_DS6_CHILD_H_

#include "net/ip/uip.h"
#include "net/child-table.h"
#include "sys/stimer.h"
#include "net/ipv6/uip-ds6.h"
#if UIP_CONF_IPV6_QUEUE_PKT
#include "net/ip/uip-packetqueue.h"
#endif                          /*UIP_CONF_QUEUE_PKT */

/*--------------------------------------------------*/
/** \brief Possible states for the child cache entries */

CHILD_TABLE_DECLARE(ds6_children);

/** \brief An entry in the child cache */
typedef struct uip_ds6_child {
  uip_ipaddr_t ipaddr;
} uip_ds6_child_t;

void uip_ds6_children_init(void);

/** \brief Neighbor Cache basic routines */
uip_ds6_child_t *uip_ds6_child_add(const uip_ipaddr_t *ipaddr,
                                 const uip_lladdr_t *lladdr,
                                 void *data);
int uip_ds6_child_rm(uip_ds6_child_t *child);
const uip_lladdr_t *uip_ds6_child_get_ll(const uip_ds6_child_t *child);
const uip_ipaddr_t *uip_ds6_child_get_ipaddr(const uip_ds6_child_t *child);
uip_ds6_child_t *uip_ds6_child_lookup(const uip_ipaddr_t *ipaddr);
uip_ds6_child_t *uip_ds6_child_ll_lookup(const uip_lladdr_t *lladdr);
uip_ipaddr_t *uip_ds6_child_ipaddr_from_lladdr(const uip_lladdr_t *lladdr);
const uip_lladdr_t *uip_ds6_child_lladdr_from_ipaddr(const uip_ipaddr_t *ipaddr);
void uip_ds6_link_child_callback(int status, int numtx);
void uip_ds6_child_periodic(void);
int uip_ds6_child_num(void);

#if UIP_ND6_SEND_NS
/**
 * \brief Refresh the reachable state of a neighbor. This function
 * may be called when a node receives an IPv6 message that confirms the
 * reachability of a neighbor.
 * \param ipaddr pointer to the IPv6 address whose neighbor reachability state
 * should be refreshed.
 */
void uip_ds6_child_refresh_reachable_state(const uip_ipaddr_t *ipaddr);
#endif /* UIP_ND6_SEND_NS */

/**
 * \brief
 *    This searches inside the neighbor table for the neighbor that is about to
 *    expire the next.
 *
 * \return
 *    A reference to the neighbor about to expire the next or NULL if
 *    table is empty.
 */
uip_ds6_child_t *uip_ds6_get_least_lifetime_child(void);

#endif /* UIP_DS6_NEIGHBOR_H_ */
/** @} */
