#include "contiki.h"
#include "net/packetbuf.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/rpl/rpl-private.h"
#include "static-scheduler.h"
#include "static-scheduler-generated.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_child_added(const linkaddr_t *addr)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_child_removed(const linkaddr_t *addr)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_packet_ready(void)
{
  static_scheduler_callback_packet_ready_generated();
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_new_time_source(const struct tsch_neighbor *old, const struct tsch_neighbor *new)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_init(void)
{
  PRINTF("static-scheduler: initializating\n");

  static_scheduler_init_generated();

  PRINTF("static-scheduler: initialized\n");
}
