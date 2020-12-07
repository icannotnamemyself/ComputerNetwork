#ifndef PROTO
#define PROTO

#define MAX_DATA 65535//max data length 16bytes
#define PACKET_LEN 65540
#include <stdio.h>

 enum PROCOL_DATATYPE{
	CONNECT = 1,
	CERTIFICATE = 2,
	DATA = 6,
	DISCONNECT = 7
};

 struct procol_packet{
	char version; //version : 5
	char type; // type 1:connect 2:certificate 6:data 7:disconnect
	short   data_len; //data length ,2bytes
	unsigned char  data[MAX_DATA];
	char check_sum;  // check_sum  0: no check_sum
};
//extern const int PACKET_LEN= sizeof(struct procol_packet); //packet length



// buffer transfer to packet
extern void buffer_to_packet(const char * buf , struct procol_packet  * pkt);

/// print function
extern void packet_print(FILE * stream , struct procol_packet * pkt);
extern void packet_buf_print(FILE * stream , char* send_buf);



#endif // PROTO
