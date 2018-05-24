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

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "lib/list.h"
#include "net/link-stats.h"
#include "net/linkaddr.h"
#include "net/packetbuf.h"
#include "net/ipv6/uip-ds6-child.h"

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"


#ifdef UIP_CONF_DS6_LINK_CHILD_CALLBACK
#define LINK_CHILD_CALLBACK(addr, status, numtx) UIP_CONF_DS6_LINK_CHILD_CALLBACK(addr, status, numtx)
void LINK_CHILD_CALLBACK(const linkaddr_t *addr, int status, int numtx);
#else
#define LINK_CHILD_CALLBACK(addr, status, numtx)
#endif /* UIP_CONF_DS6_LINK_NEIGHBOR_CALLBACK */

CHILD_TABLE_GLOBAL(uip_ds6_child_t, ds6_children);

/*---------------------------------------------------------------------------*/
void
uip_ds6_children_init(void)
{
  link_stats_init();
  child_table_register(ds6_children, (child_table_callback *)uip_ds6_child_rm);
}
/*---------------------------------------------------------------------------*/
uip_ds6_child_t *
uip_ds6_child_add(const uip_ipaddr_t *ipaddr, const uip_lladdr_t *lladdr,void *data)
{
  uip_ds6_child_t *child = child_table_add_lladdr(ds6_children, (linkaddr_t*)lladdr,data);
  if(child) {
    uip_ipaddr_copy(&child->ipaddr, ipaddr);
    PRINTF("Adding child with ip addr ");
    PRINT6ADDR(ipaddr);
    PRINTF(" link addr ");
    PRINTLLADDR(lladdr);
    return child;
  } else {
    PRINTF("uip_ds6_child_add drop ip addr ");
    PRINT6ADDR(ipaddr);
    PRINTF(" link addr (%p) ", lladdr);
    PRINTLLADDR(lladdr);
    PRINTF(" state %u\n", state);
    return NULL;
  }
}

/*---------------------------------------------------------------------------*/
int
uip_ds6_child_rm(uip_ds6_child_t *child)
{
  if(child != NULL) {
    return child_table_remove(ds6_children, child);
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
const uip_ipaddr_t *
uip_ds6_child_get_ipaddr(const uip_ds6_child_t *child)
{
  return (child != NULL) ? &child->ipaddr : NULL;
}

/*---------------------------------------------------------------------------*/
const uip_lladdr_t *
uip_ds6_child_get_ll(const uip_ds6_child_t *child)
{
  return (const uip_lladdr_t *)child_table_get_lladdr(ds6_children, child);
}
/*---------------------------------------------------------------------------*/
int
uip_ds6_child_num(void)
{
  uip_ds6_child_t *child;
  int num;

  num = 0;
  for(child = child_table_head(ds6_children);
      child != NULL;
      child = child_table_next(ds6_children, child)) {
    num++;
  }
  return num;
}
/*---------------------------------------------------------------------------*/
uip_ds6_child_t *
uip_ds6_child_lookup(const uip_ipaddr_t *ipaddr)
{
  uip_ds6_child_t *child = child_table_head(ds6_children);
  if(ipaddr != NULL) {
    while(child != NULL) {
      if(uip_ipaddr_cmp(&child->ipaddr, ipaddr)) {
        return child;
      }
      child = child_table_next(ds6_children, child);
    }
  }
  return NULL;
}
/*---------------------------------------------------------------------------*/
uip_ds6_child_t *
uip_ds6_child_ll_lookup(const uip_lladdr_t *lladdr)
{
  return child_table_get_from_lladdr(ds6_children, (linkaddr_t*)lladdr);
}

/*---------------------------------------------------------------------------*/
uip_ipaddr_t *
uip_ds6_child_ipaddr_from_lladdr(const uip_lladdr_t *lladdr)
{
  uip_ds6_child_t *child = uip_ds6_child_ll_lookup(lladdr);
  return child ? &child->ipaddr : NULL;
}

/*---------------------------------------------------------------------------*/
const uip_lladdr_t *
uip_ds6_child_lladdr_from_ipaddr(const uip_ipaddr_t *ipaddr)
{
  uip_ds6_child_t *child = uip_ds6_child_lookup(ipaddr);
  return child ? uip_ds6_child_get_ll(child) : NULL;
}
/*---------------------------------------------------------------------------*/
void
uip_ds6_link_child_callback(int status, int numtx)
{
  const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  if(linkaddr_cmp(dest, &linkaddr_null)) {
    return;
  }

  /* Update child link statistics */
  link_stats_packet_sent(dest, status, numtx);
  /* Call upper-layer callback (e.g. RPL) */
  LINK_CHILD_CALLBACK(dest, status, numtx);

#if UIP_DS6_LL_NUD
  /* From RFC4861, page 72, last paragraph of section 7.3.3:
   *
   *         "In some cases, link-specific information may indicate that a path to
   *         a child has failed (e.g., the resetting of a virtual circuit). In
   *         such cases, link-specific information may be used to purge Neighbor
   *         Cache entries before the Neighbor Unreachability Detection would do
   *         so. However, link-specific information MUST NOT be used to confirm
   *         the reachability of a child; such information does not provide
   *         end-to-end confirmation between childing IP layers."
   *
   * However, we assume that receiving a link layer ack ensures the delivery
   * of the transmitted packed to the IP stack of the neighbour. This is a
   * fair assumption and allows battery powered nodes save some battery by
   * not re-testing the state of a neighbour periodically if it
   * acknowledges link packets. */
  if(status == MAC_TX_OK) {
    uip_ds6_child_t *child;
    child = uip_ds6_child_ll_lookup((uip_lladdr_t *)dest);
    if(child != NULL && child->state != NBR_INCOMPLETE) {
      child->state = NBR_REACHABLE;
      stimer_set(&child->reachable, UIP_ND6_REACHABLE_TIME / 1000);
      PRINTF("uip-ds6-child : received a link layer ACK : ");
      PRINTLLADDR((uip_lladdr_t *)dest);
      PRINTF(" is reachable.\n");
    }
  }
#endif /* UIP_DS6_LL_NUD */

}

/*---------------------------------------------------------------------------*/
/** @} */
