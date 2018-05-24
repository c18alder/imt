#ifndef __STATIC_SCHEDULER_H__
#define __STATIC_SCHEDULER_H__

#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-conf.h"
#include "net/mac/tsch/tsch-schedule.h"
#include "static-scheduler-conf.h"

/* Call from application to start static-scheduler */
void static_scheduler_init(void);
/* Callbacks required for static-scheduler to operate */
/* Set with #define TSCH_CALLBACK_PACKET_READY static_scheduler_callback_packet_ready */
void static_scheduler_callback_packet_ready(void);
/* Set with #define TSCH_CALLBACK_NEW_TIME_SOURCE static_scheduler_callback_new_time_source */
void static_scheduler_callback_new_time_source(const struct tsch_neighbor *old, const struct tsch_neighbor *new);
/* Set with #define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK static_scheduler_callback_child_added */
void static_scheduler_callback_child_added(const linkaddr_t *addr);
/* Set with #define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK static_scheduler_callback_child_removed */
void static_scheduler_callback_child_removed(const linkaddr_t *addr);

#endif /* __STATIC_SCHEDULER_H__ */
