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
 *         LinkPeek
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#ifndef __LINKPEEK_H__
#define __LINKPEEK_H__

/********** Includes **********/

#include "contiki.h"
#include "net/rpl/rpl.h"

/******** Configuration *******/

/* Default max parents tried for alternate route */
#ifdef LINKPEEK_DEFAULT_MAX_PARENTS
#define LINKPEEK_MAX_PARENTS LINKPEEK_DEFAULT_MAX_PARENTS
#else
#define LINKPEEK_MAX_PARENTS 2
#endif

void* linkpeek_init_for_curent_packet(mac_callback_t sent, void *ptr);

void linkpeek_notify_tx_result(int, void*meta);

struct linkpeek_meta;
typedef struct linkpeek_meta linkpeek_meta_t;

#endif /* __LINKPEEK_H__ */
