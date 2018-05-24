/*
 * Copyright (c) 2017, Remous-Aris Koutsiamanis.
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
 *         LinkPeek implementation helpers.
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#include "contiki.h"
#include "net/queuebuf.h"
#include "net/linkaddr.h"
#include "net/mac/linkpeek.h"
#include "net/rpl/rpl-private.h"
#include "net/nbr-table.h"
#include "net/mac/tsch/tsch-queue.h"
#include "net/mac/tsch/tsch.h"
#include "lib/data_packet_id.h"

#include <stdlib.h>

static rpl_of_t * linkpeek_of = NULL;

struct linkpeek_meta {
  struct queuebuf *linkpeek_queuebuf;
  rpl_parent_t* linkpeek_parents[NBR_TABLE_MAX_NEIGHBORS];
  int linkpeek_parents_count;
  int linkpeek_parents_current_index;
  rpl_parent_t* linkpeek_parent_default;
  mac_callback_t linkpeek_sent;
  void *linkpeek_ptr;
};

static struct linkpeek_meta linkpeek_metas[NBR_TABLE_MAX_NEIGHBORS] = {0};
/*---------------------------------------------------------------------------*/

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
static void
linkpeek_free_backup_buffer(struct linkpeek_meta*meta){
  PRINTF("linkpeek: linkpeek_free_backup_buffer\n");
  if(meta->linkpeek_queuebuf != NULL){
    queuebuf_free(meta->linkpeek_queuebuf);
    meta->linkpeek_queuebuf = NULL;
  }
}
/*---------------------------------------------------------------------------*/
static void
linkpeek_copy_packetbuf_to_backup_buffer(struct linkpeek_meta*meta){
  PRINTF("linkpeek: linkpeek_copy_packetbuf_to_backup_buffer\n");
  meta->linkpeek_queuebuf = queuebuf_new_from_packetbuf();
}
/*---------------------------------------------------------------------------*/
static void
linkpeek_copy_backup_buffer_to_packetbuf(struct linkpeek_meta*meta){
  PRINTF("linkpeek: linkpeek_copy_backup_buffer_to_packetbuf\n");
  if(meta->linkpeek_queuebuf != NULL){
    queuebuf_to_packetbuf(meta->linkpeek_queuebuf);
  }else{
    PRINTF("linkpeek: linkpeek_copy_backup_buffer_to_packetbuf: NO available buffer\n");
  }
}
/*---------------------------------------------------------------------------*/
static void
linkpeek_reset_parents(struct linkpeek_meta*meta){
  PRINTF("linkpeek: linkpeek_reset_parents\n");
  int i;
  for(i=0; i<NBR_TABLE_MAX_NEIGHBORS;i++)
    meta->linkpeek_parents[i] = NULL;
  meta->linkpeek_parents_count = 0;
  meta->linkpeek_parents_current_index = -1;
  meta->linkpeek_parent_default = NULL;
}
/*---------------------------------------------------------------------------*/
static void
linkpeek_reset_mac(struct linkpeek_meta*meta){
  PRINTF("linkpeek: linkpeek_reset_mac\n");
  meta->linkpeek_sent = NULL;
  meta->linkpeek_ptr = NULL;
}
/*---------------------------------------------------------------------------*/
static rpl_parent_t*
linkpeek_get_next_parent(struct linkpeek_meta*meta){
  if(meta->linkpeek_parents_current_index+1 < meta->linkpeek_parents_count){
    meta->linkpeek_parents_current_index++;
    PRINTF("linkpeek: linkpeek_get_next_parent index %d\n", meta->linkpeek_parents_current_index);
    if(meta->linkpeek_parents[meta->linkpeek_parents_current_index] == meta->linkpeek_parent_default){
      PRINTF("linkpeek: linkpeek_get_next_parent skip default parent at index %d\n", meta->linkpeek_parents_current_index);
      return linkpeek_get_next_parent(meta);
    }else{
      return meta->linkpeek_parents[meta->linkpeek_parents_current_index];
    }
  }else{
    PRINTF("linkpeek: linkpeek_get_next_parent index %d = NULL\n", meta->linkpeek_parents_current_index);
    return NULL;
  }
}
/*---------------------------------------------------------------------------*/
static int
linkpeek_parent_comparison_function(const void * a, const void * b)
{
  rpl_parent_t*best = linkpeek_of->best_parent( (rpl_parent_t*)a, (rpl_parent_t*)b );
  if(best == a) //a better than b
    return -1;
  else if(best == b)
    return 1;
  else
    return 0;
}
/*---------------------------------------------------------------------------*/
static int
linkpeek_record_parents(struct linkpeek_meta*meta)
{
  PRINTF("linkpeek: linkpeek_record_parents\n");
  rpl_instance_t * instance = rpl_get_default_instance();
  if(instance != NULL){
    int fresh_only = 0;
    rpl_dag_t *dag = instance->current_dag;
    rpl_parent_t *p;
    rpl_parent_t *best = NULL;
    ;

    if(dag == NULL || dag->instance == NULL || dag->instance->of == NULL) {
      PRINTF("linkpeek: linkpeek_record_parents: no dag & dag instance & dag instance objective function\n");
      return -1;
    }

    linkpeek_reset_parents(meta);
    meta->linkpeek_parent_default = dag->preferred_parent;
    linkpeek_of = dag->instance->of;
    PRINTF("linkpeek: RPL: Current node has rank %d\n", dag->rank);
    /* Search for the best parent according to the OF */
    for(p = nbr_table_head(rpl_parents); p != NULL; p = nbr_table_next(rpl_parents, p)) {

      const linkaddr_t *p_addr = rpl_get_parent_lladdr(p);
      PRINTF("linkpeek: RPL: Parent %d has rank %d\n", p_addr->u8[LINKADDR_SIZE-1], p->rank);

      /* Exclude parents from other DAGs or announcing an infinite rank. Also exclude parents that have higher rank (including the min hop) than the current node */
      if(p->dag != dag || p->rank == INFINITE_RANK || p->rank < ROOT_RANK(dag->instance) ||
//          p->rank + instance->min_hoprankinc >= dag->rank ||
          linkpeek_of->best_parent( p, p ) == NULL
          )
      {
        if(p->rank < ROOT_RANK(dag->instance)) {
          PRINTF("linkpeek: RPL: Parent %d has invalid rank %d\n", p_addr->u8[LINKADDR_SIZE-1], p->rank);
        }
        continue;
      }

      if(fresh_only && !rpl_parent_is_fresh(p)) {
        /* Filter out non-fresh parents if fresh_only is set */
        continue;
      }

  #if UIP_ND6_SEND_NS
      {
      uip_ds6_nbr_t *nbr = rpl_get_nbr(p);
      /* Exclude links to a neighbor that is not reachable at a NUD level */
      if(nbr == NULL || nbr->state != NBR_REACHABLE) {
        continue;
      }
      }
  #endif /* UIP_ND6_SEND_NS */

      /* Now we have an acceptable parent, add to array */
      meta->linkpeek_parents[meta->linkpeek_parents_count] = p;
      meta->linkpeek_parents_count++;
    }

    // Sort all the parents based on the objective function
    qsort(meta->linkpeek_parents, meta->linkpeek_parents_count, sizeof(rpl_parent_t*), linkpeek_parent_comparison_function);
    //Keep upto LINKPEEK_MAX_PARENTS of the highest preferred parents
    meta->linkpeek_parents_count = MIN(meta->linkpeek_parents_count, LINKPEEK_MAX_PARENTS);
//    meta->linkpeek_parents_count = meta->linkpeek_parents_count < LINKPEEK_MAX_PARENTS? meta->linkpeek_parents_count: LINKPEEK_MAX_PARENTS;

    PRINTF("linkpeek: linkpeek_record_parents OK: parents count: %d\n", meta->linkpeek_parents_count);
    return 0;
  }

  PRINTF("linkpeek: linkpeek_record_parents: no default RPL instance\n");
  return -2;
}
/*---------------------------------------------------------------------------*/
void
linkpeek_notify_tx_result(int tx_ok, void*meta_t){
  PRINTF("linkpeek: linkpeek_notify_tx_result(%i)\n", tx_ok);
  struct linkpeek_meta *meta = (struct linkpeek_meta *)meta_t;
  if(meta != NULL){
    if(tx_ok){
      linkpeek_free_backup_buffer(meta);
      linkpeek_reset_parents(meta);
      linkpeek_reset_mac(meta);
    }else{
      rpl_parent_t*next_best_parent = linkpeek_get_next_parent(meta);
      if(next_best_parent != NULL){
        //add packet to new neighbor queue
        const linkaddr_t *next_best_parent_addr = rpl_get_parent_lladdr(next_best_parent);
        linkpeek_copy_backup_buffer_to_packetbuf(meta);
        PRINTF("linkpeek: linkpeek_notify_tx_result: resending to alternate parent %02d\n", next_best_parent_addr->u8[LINKADDR_SIZE - 1]);
        packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, next_best_parent_addr);
        NETSTACK_MAC.send(meta->linkpeek_sent, meta);
      }else{
        PRINTF("linkpeek: linkpeek_notify_tx_result: failed to resend at alternate parent index %02d\n", meta->linkpeek_parents_current_index);
        linkpeek_free_backup_buffer(meta);
        linkpeek_reset_parents(meta);
        linkpeek_reset_mac(meta);
      }
    }
  }else{
    PRINTF("linkpeek: linkpeek_notify_tx_result(%i) Skip due to meta=NULL\n", tx_ok);
  }

}
/*---------------------------------------------------------------------------*/
void*
linkpeek_init_for_curent_packet(mac_callback_t sent, void *ptr){
  char packet_id[7];
  data_packet_id(packet_id, (char*)packetbuf_dataptr(), packetbuf_datalen());

  PRINTF("linkpeek: linkpeek_init_for_curent_packet data_packet_id: %s\n", packet_id);
  struct linkpeek_meta* meta=(struct linkpeek_meta*)ptr;
  if(meta == NULL){
    PRINTF("linkpeek: linkpeek_init_for_curent_packet is new packet\n");
    int free_meta_index;
    for(free_meta_index=0;free_meta_index<NBR_TABLE_MAX_NEIGHBORS;free_meta_index++){
      if(linkpeek_metas[free_meta_index].linkpeek_queuebuf == NULL)
        break;
    }
    if(free_meta_index<NBR_TABLE_MAX_NEIGHBORS){
      meta = &linkpeek_metas[free_meta_index];
      linkpeek_copy_packetbuf_to_backup_buffer(meta);
      linkpeek_record_parents(meta);
      meta->linkpeek_sent = sent;
      meta->linkpeek_ptr = meta;
      return meta;
    }else{
      PRINTF("linkpeek: linkpeek_init_for_curent_packet did not find a free spot in linkpeek_metas. Error!\n");
      return meta;
    }
  }else{
    PRINTF("linkpeek: linkpeek_init_for_curent_packet is retransmitting packet. Do nothing for linkpeek\n");
    return meta;
  }
}
/*---------------------------------------------------------------------------*/
