#ifndef DATA_PACKET_ID_H_
#define DATA_PACKET_ID_H_

#ifndef DATA_PACKET_MARKER
#define DATA_PACKET_MARKER "SxKkf0KzxcQ0B"
#endif

#ifndef DATA_PACKET_MARKER_LEN
#define DATA_PACKET_MARKER_LEN (sizeof(DATA_PACKET_MARKER)-1)
#endif


char*
data_packet_id(char*buf5, char*ptr, int len);

#endif /* DATA_PACKET_ID_H_ */
