#include "contiki.h"
#include "data_packet_id.h"
#include "string.h"
#include "node-id.h"

char*
data_packet_id(char*buf5, char*ptr, int len)
{
  char * found = NULL;
  int i = 0;
  for(; i < len - (DATA_PACKET_MARKER_LEN + 6); i++) {
    found = strstr (ptr + i, DATA_PACKET_MARKER);
    if(found != NULL) {
      strncpy (buf5, found + DATA_PACKET_MARKER_LEN, 6);
      buf5[6] = 0;
      return buf5;
    }
  }
  strcpy (buf5, "NULL");
  return buf5;
}
