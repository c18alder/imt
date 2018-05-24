/*
 * Copyright (c) 2013, Swedish Institute of Computer Science
 * Copyright (c) 2010, Vrije Universiteit Brussel
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
 * Authors: Simon Duquennoy <simonduq@sics.se>
 *          Joris Borms <joris.borms@vub.ac.be>
 */

#ifndef child_table_H_
#define child_table_H_

#include "contiki.h"
#include "net/linkaddr.h"
#include "net/netstack.h"

/* Neighbor table size */
#ifdef CHILD_TABLE_CONF_MAX_NEIGHBORS
#define CHILD_TABLE_MAX_NEIGHBORS CHILD_TABLE_CONF_MAX_NEIGHBORS
#else /* child_table_CONF_MAX_NEIGHBORS */
#define CHILD_TABLE_MAX_NEIGHBORS 4
#endif /* child_table_CONF_MAX_NEIGHBORS */

/* An item in a neighbor table */
typedef void child_table_item_t;

/* Callback function, called when removing an item from a table */
typedef void(child_table_callback)(child_table_item_t *item);

/* A neighbor table */
typedef struct child_table {
  int index;
  int item_size;
  child_table_callback *callback;
  child_table_item_t *data;
} child_table_t;

/** \brief A static neighbor table. To be initialized through child_table_register(name) */
#define child_table(type, name) \
  static type _##name##_mem[CHILD_TABLE_MAX_NEIGHBORS]; \
  static child_table_t name##_struct = { 0, sizeof(type), NULL, (child_table_item_t *)_##name##_mem }; \
  static child_table_t *name = &name##_struct \

/** \brief A non-static neighbor table. To be initialized through child_table_register(name) */
#define CHILD_TABLE_GLOBAL(type, name) \
  static type _##name##_mem[CHILD_TABLE_MAX_NEIGHBORS]; \
  static child_table_t name##_struct = { 0, sizeof(type), NULL, (child_table_item_t *)_##name##_mem }; \
  child_table_t *name = &name##_struct \

/** \brief Declaration of non-static neighbor tables */
#define CHILD_TABLE_DECLARE(name) extern child_table_t *name

/** \name Neighbor tables: register and loop through table elements */
/** @{ */
int child_table_register(child_table_t *table, child_table_callback *callback);
child_table_item_t *child_table_head(child_table_t *table);
child_table_item_t *child_table_next(child_table_t *table, child_table_item_t *item);
/** @} */

/** \name Neighbor tables: add and get data */
/** @{ */
child_table_item_t *child_table_add_lladdr(child_table_t *table, const linkaddr_t *lladdr, void *data);
child_table_item_t *child_table_get_from_lladdr(child_table_t *table, const linkaddr_t *lladdr);
/** @} */

/** \name Neighbor tables: set flags (unused, locked, unlocked) */
/** @{ */
int child_table_remove(child_table_t *table, child_table_item_t *item);
int child_table_lock(child_table_t *table, child_table_item_t *item);
int child_table_unlock(child_table_t *table, child_table_item_t *item);
/** @} */

/** \name Neighbor tables: address manipulation */
/** @{ */
linkaddr_t *child_table_get_lladdr(child_table_t *table, const child_table_item_t *item);
int child_table_update_lladdr(const linkaddr_t *old_addr, const linkaddr_t *new_addr, int remove_if_duplicate);
/** @} */

#endif /* child_table_H_ */
