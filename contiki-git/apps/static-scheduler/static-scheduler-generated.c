#include "contiki.h"
#include "net/packetbuf.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/rpl/rpl-private.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-conf.h"
#include "net/mac/tsch/tsch-schedule.h"
#include "node-id.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

/*---------------------------------------------------------------------------*/
static struct tsch_slotframe * slotframe = NULL;
/*---------------------------------------------------------------------------*/
struct tsch_cell {
  uint16_t from_node;
  uint16_t to_node;
  uint8_t overhearing;
  uint16_t packet;
  uint16_t packet_transmission;
};
/*---------------------------------------------------------------------------*/
typedef struct tsch_cell tsch_cell_t;
/*---------------------------------------------------------------------------*/
static void
print_tsch_cell(tsch_cell_t* cell)
{
  PRINTF("%3u (P%3u/TX%3u) -> %3u %s",
         cell->from_node,
         cell->packet,
         cell->packet_transmission,
         cell->to_node,
         cell->overhearing?"OH":"NO"
         );
}
/*---------------------------------------------------------------------------*/
#if LINKADDR_SIZE == 8
#ifdef CONTIKI_TARGET_Z1
  /* Set node with MAC address c1:0c:00:00:00:00:01 as coordinator,
   * convenient in cooja for regression tests using z1 nodes
   * */
#define INT_TO_LINKADDR(i) ((linkaddr_t){ { 0xc1, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, i } })
#elif CONTIKI_TARGET_COOJA
#define INT_TO_LINKADDR(i) ((linkaddr_t){ { 0, i, 0, i, 0, i, 0, i } })
#endif
#endif /*LINKADDR_SIZE == 8*/
/*---------------------------------------------------------------------------*/
#if defined(TSCH_CONF_MAC_MAX_FRAME_RETRIES)
#define EB_TIMESLOT_COUNT (TSCH_CONF_MAC_MAX_FRAME_RETRIES + 1)
#else
#endif /* defined(TSCH_CONF_MAC_MAX_FRAME_RETRIES) */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* START GENERATED */
/*---------------------------------------------------------------------------*/
#if defined(TSCH_CONF_MAC_MAX_FRAME_RETRIES) & TSCH_CONF_MAC_MAX_FRAME_RETRIES == 0
#define CHANNEL_COUNT 1
#define TIMESLOT_COUNT 22
#define CELL_ENTRIES 1
static struct tsch_cell schedule[CHANNEL_COUNT][TIMESLOT_COUNT][CELL_ENTRIES] =
{
  {
    {{8, 4, 0, 1, 1}},
    {{8, 5, 0, 1, 1}},
    {{4, 2, 0, 1, 1}},
    {{5, 2, 0, 1, 1}},
    {{2, 1, 0, 1, 1}},
    {{5, 3, 0, 1, 1}},
    {{3, 1, 0, 1, 1}},
    {{9, 5, 0, 2, 1}},
    {{9, 6, 0, 2, 1}},
    {{5, 2, 0, 2, 1}},
    {{5, 3, 0, 2, 1}},
    {{6, 2, 0, 2, 1}},
    {{2, 1, 0, 2, 1}},
    {{6, 3, 0, 2, 1}},
    {{3, 1, 0, 2, 1}},
    {{10, 6, 0, 3, 1}},
    {{10, 7, 0, 3, 1}},
    {{6, 2, 0, 3, 1}},
    {{6, 3, 0, 3, 1}},
    {{2, 1, 0, 3, 1}},
    {{7, 3, 0, 3, 1}},
    {{3, 1, 0, 3, 1}},
  },
};
#define NODE_IDS_COUNT 10
static uint8_t node_ids[NODE_IDS_COUNT] =
{
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10
};
#elif defined(TSCH_CONF_MAC_MAX_FRAME_RETRIES) & TSCH_CONF_MAC_MAX_FRAME_RETRIES == 2
#define CHANNEL_COUNT 1
#define TIMESLOT_COUNT 66
#define CELL_ENTRIES 1
static struct tsch_cell schedule[CHANNEL_COUNT][TIMESLOT_COUNT][CELL_ENTRIES] =
{
  {
    {{8, 4, 0, 1, 3}},
    {{8, 5, 0, 1, 3}},
    {{8, 4, 0, 1, 2}},
    {{8, 5, 0, 1, 1}},
    {{8, 5, 0, 1, 2}},
    {{8, 4, 0, 1, 1}},
    {{9, 6, 0, 2, 1}},
    {{9, 5, 0, 2, 3}},
    {{9, 6, 0, 2, 2}},
    {{10, 6, 0, 3, 3}},
    {{4, 2, 0, 1, 2}},
    {{9, 6, 0, 2, 3}},
    {{5, 2, 0, 1, 3}},
    {{6, 2, 0, 2, 1}},
    {{9, 5, 0, 2, 2}},
    {{10, 6, 0, 3, 1}},
    {{5, 3, 0, 1, 1}},
    {{5, 3, 0, 1, 3}},
    {{10, 6, 0, 3, 2}},
    {{10, 7, 0, 3, 3}},
    {{6, 2, 0, 3, 1}},
    {{6, 2, 0, 3, 3}},
    {{6, 3, 0, 2, 3}},
    {{6, 2, 0, 3, 2}},
    {{2, 1, 0, 3, 1}},
    {{5, 2, 0, 1, 1}},
    {{10, 7, 0, 3, 2}},
    {{9, 5, 0, 2, 1}},
    {{6, 2, 0, 2, 2}},
    {{5, 2, 0, 2, 2}},
    {{5, 2, 0, 2, 3}},
    {{5, 2, 0, 1, 2}},
    {{6, 3, 0, 3, 2}},
    {{6, 3, 0, 2, 1}},
    {{6, 3, 0, 3, 1}},
    {{5, 2, 0, 2, 1}},
    {{5, 3, 0, 1, 2}},
    {{3, 1, 0, 1, 3}},
    {{3, 1, 0, 1, 1}},
    {{4, 2, 0, 1, 1}},
    {{6, 3, 0, 2, 2}},
    {{6, 2, 0, 2, 3}},
    {{10, 7, 0, 3, 1}},
    {{7, 3, 0, 3, 3}},
    {{7, 3, 0, 3, 2}},
    {{6, 3, 0, 3, 3}},
    {{4, 2, 0, 1, 3}},
    {{5, 3, 0, 2, 2}},
    {{7, 3, 0, 3, 1}},
    {{2, 1, 0, 3, 3}},
    {{5, 3, 0, 2, 1}},
    {{3, 1, 0, 3, 3}},
    {{2, 1, 0, 2, 2}},
    {{2, 1, 0, 1, 1}},
    {{2, 1, 0, 3, 2}},
    {{3, 1, 0, 3, 1}},
    {{5, 3, 0, 2, 3}},
    {{3, 1, 0, 2, 1}},
    {{2, 1, 0, 1, 3}},
    {{2, 1, 0, 1, 2}},
    {{3, 1, 0, 3, 2}},
    {{3, 1, 0, 2, 2}},
    {{2, 1, 0, 2, 3}},
    {{2, 1, 0, 2, 1}},
    {{3, 1, 0, 1, 2}},
    {{3, 1, 0, 2, 3}},
  },
};
#define NODE_IDS_COUNT 10
static uint8_t node_ids[NODE_IDS_COUNT] =
{
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10
};

#elif defined(TSCH_CONF_MAC_MAX_FRAME_RETRIES) & TSCH_CONF_MAC_MAX_FRAME_RETRIES == 4
#define CHANNEL_COUNT 1
#define TIMESLOT_COUNT 110
#define CELL_ENTRIES 1
static struct tsch_cell schedule[CHANNEL_COUNT][TIMESLOT_COUNT][CELL_ENTRIES] =
{
  {
    {{10, 6, 0, 3, 5}},
    {{9, 5, 0, 2, 2}},
    {{10, 6, 0, 3, 2}},
    {{8, 4, 0, 1, 3}},
    {{8, 5, 0, 1, 2}},
    {{10, 6, 0, 3, 1}},
    {{8, 5, 0, 1, 5}},
    {{9, 6, 0, 2, 2}},
    {{8, 5, 0, 1, 3}},
    {{9, 6, 0, 2, 1}},
    {{9, 6, 0, 2, 4}},
    {{9, 5, 0, 2, 3}},
    {{9, 6, 0, 2, 3}},
    {{9, 5, 0, 2, 4}},
    {{9, 5, 0, 2, 1}},
    {{8, 5, 0, 1, 1}},
    {{10, 6, 0, 3, 4}},
    {{10, 6, 0, 3, 3}},
    {{8, 4, 0, 1, 4}},
    {{9, 6, 0, 2, 5}},
    {{10, 7, 0, 3, 1}},
    {{6, 3, 0, 2, 2}},
    {{6, 2, 0, 3, 2}},
    {{8, 4, 0, 1, 2}},
    {{8, 4, 0, 1, 1}},
    {{9, 5, 0, 2, 5}},
    {{10, 7, 0, 3, 3}},
    {{5, 2, 0, 2, 4}},
    {{6, 3, 0, 2, 3}},
    {{6, 2, 0, 2, 2}},
    {{5, 2, 0, 2, 3}},
    {{5, 3, 0, 2, 2}},
    {{6, 3, 0, 2, 5}},
    {{5, 3, 0, 2, 5}},
    {{6, 3, 0, 3, 4}},
    {{5, 3, 0, 2, 3}},
    {{5, 2, 0, 2, 2}},
    {{10, 7, 0, 3, 5}},
    {{6, 2, 0, 2, 1}},
    {{6, 2, 0, 2, 4}},
    {{5, 2, 0, 2, 5}},
    {{5, 3, 0, 2, 4}},
    {{6, 3, 0, 2, 1}},
    {{5, 3, 0, 2, 1}},
    {{6, 2, 0, 3, 4}},
    {{6, 2, 0, 3, 5}},
    {{6, 2, 0, 3, 3}},
    {{6, 3, 0, 3, 2}},
    {{5, 2, 0, 2, 1}},
    {{6, 2, 0, 3, 1}},
    {{8, 5, 0, 1, 4}},
    {{5, 3, 0, 1, 3}},
    {{5, 3, 0, 1, 1}},
    {{5, 2, 0, 1, 1}},
    {{6, 3, 0, 2, 4}},
    {{5, 3, 0, 1, 4}},
    {{5, 3, 0, 1, 5}},
    {{8, 4, 0, 1, 5}},
    {{5, 2, 0, 1, 5}},
    {{5, 2, 0, 1, 3}},
    {{2, 1, 0, 3, 1}},
    {{2, 1, 0, 3, 5}},
    {{10, 7, 0, 3, 4}},
    {{6, 2, 0, 2, 5}},
    {{6, 2, 0, 2, 3}},
    {{2, 1, 0, 2, 2}},
    {{2, 1, 0, 3, 2}},
    {{2, 1, 0, 2, 5}},
    {{5, 2, 0, 1, 2}},
    {{4, 2, 0, 1, 1}},
    {{5, 2, 0, 1, 4}},
    {{6, 3, 0, 3, 1}},
    {{4, 2, 0, 1, 3}},
    {{5, 3, 0, 1, 2}},
    {{3, 1, 0, 1, 2}},
    {{3, 1, 0, 1, 1}},
    {{2, 1, 0, 3, 3}},
    {{2, 1, 0, 2, 1}},
    {{4, 2, 0, 1, 4}},
    {{3, 1, 0, 2, 1}},
    {{3, 1, 0, 2, 3}},
    {{3, 1, 0, 2, 5}},
    {{3, 1, 0, 1, 4}},
    {{3, 1, 0, 1, 3}},
    {{6, 3, 0, 3, 5}},
    {{3, 1, 0, 1, 5}},
    {{6, 3, 0, 3, 3}},
    {{4, 2, 0, 1, 5}},
    {{4, 2, 0, 1, 2}},
    {{2, 1, 0, 1, 5}},
    {{10, 7, 0, 3, 2}},
    {{7, 3, 0, 3, 2}},
    {{7, 3, 0, 3, 1}},
    {{7, 3, 0, 3, 5}},
    {{7, 3, 0, 3, 4}},
    {{7, 3, 0, 3, 3}},
    {{3, 1, 0, 3, 2}},
    {{2, 1, 0, 3, 4}},
    {{2, 1, 0, 2, 4}},
    {{2, 1, 0, 2, 3}},
    {{3, 1, 0, 3, 1}},
    {{3, 1, 0, 3, 4}},
    {{3, 1, 0, 2, 2}},
    {{2, 1, 0, 1, 2}},
    {{3, 1, 0, 2, 4}},
    {{2, 1, 0, 1, 1}},
    {{3, 1, 0, 3, 5}},
    {{2, 1, 0, 1, 4}},
    {{3, 1, 0, 3, 3}},
    {{2, 1, 0, 1, 3}},
  },
};
#define NODE_IDS_COUNT 10
static uint8_t node_ids[NODE_IDS_COUNT] =
{
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10
};
#endif
/*---------------------------------------------------------------------------*/
/* END GENERATED */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
static int
select_packet_unicast(uint16_t *out_slotframe_handle, uint16_t *out_timeslot)
{
  /* Select data packets we have a unicast link to */
  const linkaddr_t *packet_dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  const linkaddr_t *packet_src = packetbuf_addr(PACKETBUF_ADDR_SENDER);
  if(packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE) == FRAME802154_DATAFRAME
     && !linkaddr_cmp(packet_dest, &linkaddr_null)) {
    if(out_slotframe_handle != NULL) {
      *out_slotframe_handle = 0;
    }
    if(out_timeslot != NULL) {
      uint16_t channel, generated_timeslot, cell_entry = 0;

      /* Find the *first* timeslot in the slotframe for this TX. It is implied that this is for the first TX.
       * For the next retransmissions, if there are any, the time slot will be decided in another place.
       */
      for(generated_timeslot = 0; generated_timeslot < TIMESLOT_COUNT; generated_timeslot++) {
        for(channel = 0; channel < CHANNEL_COUNT; channel++) {
          const linkaddr_t cell_src = INT_TO_LINKADDR(schedule[channel][generated_timeslot][cell_entry].from_node);
          const linkaddr_t cell_dest = INT_TO_LINKADDR(schedule[channel][generated_timeslot][cell_entry].to_node);

          if(linkaddr_cmp(packet_src, &cell_src) && linkaddr_cmp(packet_dest, &cell_dest)){
            *out_timeslot = EB_TIMESLOT_COUNT + NODE_IDS_COUNT + generated_timeslot;
            PRINTF("select_packet_unicast: selected TSCH cell: ");
            print_tsch_cell(&schedule[channel][generated_timeslot][cell_entry]);
            PRINTF("\n");
            break;
          }
        }
      }
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
select_packet_node_broadcast(uint16_t *out_slotframe_handle, uint16_t *out_timeslot)
{
  /* Select data packets which are to be broadcast */
  const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  const linkaddr_t *packet_src = packetbuf_addr(PACKETBUF_ADDR_SENDER);
  if(packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE) == FRAME802154_DATAFRAME
     && linkaddr_cmp(dest, &linkaddr_null)) {
    if(out_slotframe_handle != NULL) {
      *out_slotframe_handle = 0;
    }
    if(out_timeslot != NULL) {
      uint16_t channel = 0, node_broadcast_timeslot;

      /* Find the *first* timeslot in the slotframe for this TX. It is implied that this is for the first TX.
       * For the next retransmissions, if there are any, the time slot will be decided in another place.
       */
      for(node_broadcast_timeslot = 0; node_broadcast_timeslot < NODE_IDS_COUNT; node_broadcast_timeslot++) {
        const linkaddr_t cell_src = INT_TO_LINKADDR(node_ids[node_broadcast_timeslot]);

        if(linkaddr_cmp(packet_src, &cell_src)){
          *out_timeslot = EB_TIMESLOT_COUNT + node_broadcast_timeslot;
          PRINTF("select_packet_node_broadcast: selected node_id: %u\n", node_ids[node_broadcast_timeslot]);
          break;
        }
      }

    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
select_packet_eb(uint16_t *out_slotframe_handle, uint16_t *out_timeslot)
{
  /* Select EBs only */
  if(packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE) == FRAME802154_BEACONFRAME) {
    if(out_slotframe_handle != NULL) {
      *out_slotframe_handle = 0;
    }
    if(out_timeslot != NULL) {
      *out_timeslot = 0; /* Always the same */
    }
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
select_packet(uint16_t *out_slotframe_handle, uint16_t *out_timeslot)
{
  int select_packet_result_unicast, select_packet_result_node_broadcast, select_packet_result_eb;
  select_packet_result_unicast = select_packet_unicast(out_slotframe_handle, out_timeslot);
  if(!select_packet_result_unicast){
    select_packet_result_node_broadcast = select_packet_node_broadcast(out_slotframe_handle, out_timeslot);
    if(!select_packet_result_node_broadcast)
      select_packet_result_eb = select_packet_eb(out_slotframe_handle, out_timeslot);
  }
  return select_packet_result_unicast || select_packet_result_node_broadcast || select_packet_result_eb;
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_packet_ready_generated(void)
{
  PRINTF("static-scheduler: generated callback_packet_ready\n");
  /* By default, use any timeslot */
  uint16_t slotframe_handle = 0;
  uint16_t timeslot = 0xffff;

  const linkaddr_t *packet_dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  const linkaddr_t *packet_src = packetbuf_addr(PACKETBUF_ADDR_SENDER);
  PRINTF("static-scheduler: deciding TSCH cell for packet from ");
  PRINTLLADDR(packet_src);
  PRINTF(" to ");
  PRINTLLADDR(packet_dest);
  PRINTF(" type ");
  if(packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE) == FRAME802154_BEACONFRAME)
    PRINTF("EB");
  else if(packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE) == FRAME802154_DATAFRAME)
    PRINTF("DATA");
  else
    PRINTF("OTHER(%u)", packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE));

  PRINTF("\n");
  select_packet(&slotframe_handle, &timeslot);
  PRINTF("static-scheduler: selected cell: sf %u ts %u\n", slotframe_handle, timeslot);

  packetbuf_set_attr(PACKETBUF_ATTR_TSCH_SLOTFRAME, slotframe_handle);
  packetbuf_set_attr(PACKETBUF_ATTR_TSCH_TIMESLOT, timeslot);
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_init_generated(void)
{
  PRINTF("static-scheduler: generated init\n");
  /* Slotframe for all transmissions, size is eb_timeslot_count (for EB) + NODE_IDS_COUNT (for DIO/ 1 node broadcasts others listen) + generated ones */
  slotframe = tsch_schedule_add_slotframe(0, EB_TIMESLOT_COUNT + NODE_IDS_COUNT + TIMESLOT_COUNT);

  /* EB links: all on channel 0 at timeslot 0 */
  uint16_t eb_timeslot_index;
  for(eb_timeslot_index = 0; eb_timeslot_index < EB_TIMESLOT_COUNT; eb_timeslot_index++)
    tsch_schedule_add_link(slotframe,
                           LINK_OPTION_SHARED | LINK_OPTION_TX | LINK_OPTION_RX,
                           LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                           eb_timeslot_index, 0);

  /* Node broadcast, e.g. for DIO. One node sends, all other listen */
  uint16_t node_broadcast_timeslot_index;
  for(node_broadcast_timeslot_index = 0; node_broadcast_timeslot_index < NODE_IDS_COUNT; node_broadcast_timeslot_index++){
    uint16_t channel = 0;

    if(node_ids[node_broadcast_timeslot_index] == node_id){ //TX cell
      tsch_schedule_add_link(slotframe,
          LINK_OPTION_SHARED | LINK_OPTION_TX,
          LINK_TYPE_NORMAL, &tsch_broadcast_address,
          EB_TIMESLOT_COUNT + node_broadcast_timeslot_index, channel);
      printf("At cell: ch %i, ts %i, node %u TX towards ALL\n",
             channel, EB_TIMESLOT_COUNT + node_broadcast_timeslot_index, node_id
             );
    }else{ // RX cell
      linkaddr_t from_node_linkaddr = INT_TO_LINKADDR(node_ids[node_broadcast_timeslot_index]);
      tsch_schedule_add_link(slotframe,
          LINK_OPTION_SHARED | LINK_OPTION_RX,
          LINK_TYPE_NORMAL, &from_node_linkaddr,
          EB_TIMESLOT_COUNT + node_broadcast_timeslot_index, channel);
      printf("At cell: ch %i, ts %i, node %u RX from node %i\n",
             channel, EB_TIMESLOT_COUNT + node_broadcast_timeslot_index,
             node_id, node_ids[node_broadcast_timeslot_index]
             );
    }
  }

//  printf("Current MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
//         linkaddr_node_addr[0], linkaddr_node_addr[1], linkaddr_node_addr[2], linkaddr_node_addr[3],
//         linkaddr_node_addr[4], linkaddr_node_addr[5], linkaddr_node_addr[6], linkaddr_node_addr[7]);
  printf("Current MAC ");
  PRINTLLADDR((const uip_lladdr_t *)&linkaddr_node_addr);
  printf("\n");

  uint16_t channel, generated_timeslot, cell_entry = 0;
  for(channel = 0; channel < CHANNEL_COUNT; channel++) {
    for(generated_timeslot = 0; generated_timeslot < TIMESLOT_COUNT; generated_timeslot++) {

#if (DEBUG) & DEBUG_PRINT
      PRINTF("Generated C%3u-T%3u: ", channel, generated_timeslot);
      print_tsch_cell(&schedule[channel][generated_timeslot][cell_entry]);
      PRINTF("\n");
#endif /* (DEBUG) & DEBUG_PRINT */
      linkaddr_t from_node_linkaddr = INT_TO_LINKADDR(schedule[channel][generated_timeslot][cell_entry].from_node);
      linkaddr_t to_node_linkaddr = INT_TO_LINKADDR(schedule[channel][generated_timeslot][cell_entry].to_node);
      uint16_t schedule_timeslot = EB_TIMESLOT_COUNT + NODE_IDS_COUNT + generated_timeslot;

//      printf("%i %i From MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
//             channel, schedule_timeslot,
//             from_node_linkaddr[0], from_node_linkaddr[1], from_node_linkaddr[2], from_node_linkaddr[3],
//             from_node_linkaddr[4], from_node_linkaddr[5], from_node_linkaddr[6], from_node_linkaddr[7]);
      printf("%i %i From MAC ", channel, schedule_timeslot);
      PRINTLLADDR((const uip_lladdr_t *)&from_node_linkaddr);
      printf("\n");

//      printf("%i %i To   MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
//             channel, schedule_timeslot,
//             to_node_linkaddr[0], to_node_linkaddr[1], to_node_linkaddr[2], to_node_linkaddr[3],
//             to_node_linkaddr[4], to_node_linkaddr[5], to_node_linkaddr[6], to_node_linkaddr[7]);
      printf("%i %i To   MAC ", channel, schedule_timeslot);
      PRINTLLADDR((const uip_lladdr_t *)&to_node_linkaddr);
      printf("\n");

      /* TX */
      if(linkaddr_cmp(&from_node_linkaddr, &linkaddr_node_addr)) {
        tsch_schedule_add_link(slotframe,
            LINK_OPTION_TX,
            LINK_TYPE_NORMAL, &to_node_linkaddr,
            schedule_timeslot, channel);
        printf("At cell: ch %i, ts %i, node %u TX towards node %u\n",
               channel, schedule_timeslot,
               schedule[channel][generated_timeslot][cell_entry].from_node,
               schedule[channel][generated_timeslot][cell_entry].to_node
               );
      }
      /* RX */
      else if(linkaddr_cmp(&to_node_linkaddr, &linkaddr_node_addr)) {
        tsch_schedule_add_link(slotframe,
            LINK_OPTION_RX,
            LINK_TYPE_NORMAL, &tsch_broadcast_address,
            schedule_timeslot, channel);
        printf("At cell: ch %i, ts %i, node %u RX from node %u\n",
               channel, schedule_timeslot,
               schedule[channel][generated_timeslot][cell_entry].to_node,
               schedule[channel][generated_timeslot][cell_entry].from_node
               );
      }
    }
  }

}

