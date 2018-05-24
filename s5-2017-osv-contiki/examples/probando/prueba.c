#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-engine.h"
#include "dev/button-sensor.h"

#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/serial-line.h"
#include <stdint.h>

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 0x1)      /* server addr*/

#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

#define TOGGLE_INTERVAL 2
/*---------------------------------------------------------------------------*/
PROCESS(er_example_client, "Coap Client OSV");
AUTOSTART_PROCESSES(&er_example_client);
/*---------------------------------------------------------------------------*/
uip_ipaddr_t server_ipaddr;
static struct etimer et;
/*---------------------------------------------------------------------------*/
/* Example URIs that can be queried. */
#define NUMBER_OF_URLS 3
/* leading and ending slashes only for demo purposes, get cropped automatically when setting the Uri-Path */
char *service_urls[NUMBER_OF_URLS] =
{"speed", "kms", "soc"};
uint16_t port_service_urls[NUMBER_OF_URLS] =
{30007, 30007, 30008};
#if PLATFORM_HAS_BUTTON
static int uri_switch = 0;
#endif
/*---------------------------------------------------------------------------*/
/*function to be used to send via uart*/
unsigned int
uart1_send_bytes(const unsigned char *s, unsigned int len)
{
  unsigned int i = 0;

  while(s && *s != 0) {
    if(i >= len) {
      break;
    }
    uart_write_byte(1, *s++);
    i++;
  }
  return i;
}
/*---------------------------------------------------------------------------*/
/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void
client_chunk_handler(void *response)
{
  const uint8_t *chunk;
  char *pch;
  char arduino_str[30];

  int len = coap_get_payload(response, &chunk);
  /*we will need to define the string to send ex: kms=12.857 \n */
  pch=strtok((char *)chunk,":");
  pch=strtok(NULL,"}");
  //Concatenate the strings
  strcat(arduino_str,service_urls[uri_switch]);
  strcat(arduino_str,"=");
  strcat(arduino_str,pch);
  strcat(arduino_str,"\n");
  uart1_send_bytes((uint8_t*)arduino_str,sizeof(arduino_str)-1);
  printf("%.*s \n", sizeof(arduino_str), arduino_str);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(er_example_client, ev, data)
{
  char *rxdata;    
  PROCESS_BEGIN();
  static coap_packet_t request[1];      /* This way the packet can be treated as pointer as usual. */
  SERVER_NODE(&server_ipaddr);

  /* receives all CoAP messages */
  coap_init_engine();
  uart_set_input(1, serial_line_input_byte);
  etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);
  leds_toggle(LEDS_GREEN);


  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&et)) {

      /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
      coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
      coap_set_header_uri_path(request, service_urls[uri_switch]);
      printf("request service is %s \n",service_urls[uri_switch]);

      PRINT6ADDR(&server_ipaddr);
      PRINTF(" : %u\n", port_service_urls[uri_switch]);
      COAP_BLOCKING_REQUEST(&server_ipaddr, UIP_HTONS(port_service_urls[uri_switch]), request,
                            client_chunk_handler);

      uri_switch = (uri_switch + 1) % NUMBER_OF_URLS;
      //printf("the new url switch is %i \n", uri_switch);
      //printf("the new ressource is %s \n", service_urls[uri_switch]);

      etimer_reset(&et);
    } else if(ev == serial_line_event_message) {

      leds_toggle(LEDS_RED);
      rxdata = data;
      printf("Data received over UART1 %s\n", rxdata);
    }

  }

  PROCESS_END();
}

