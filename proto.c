#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "proto.h"

#define SERVER_PORT 8888
#define BUFF_LEN 512
#define SERVER_IP "172.0.5.182"


void packet_buf_print(FILE * stream , char* send_buf){

//    fprintf(stream, "sending packet....");
//    fprintf(stream , "version:%x\n", send_buf[0]);
//    fprintf(stream , "type: %x\n", send_buf[1]);
//    fprintf(stream, "data_len:%x bytes\n", * (unsigned short *)(send_buf+2));
//    fprintf(stream, "data: %s \n" , send_buf+4);
//    fprintf(stream, "check_sum:%x\n" , send_buf[PACKET_LEN -1]);

    fprintf(stream, "sending packet....");
    fprintf(stream , "version:%x\n", send_buf[0]);
    fprintf(stream , "type: %x\n", send_buf[1]);
    fprintf(stream, "data_len:%x bytes\n", * (unsigned short *)(send_buf+2));
    fprintf(stream, "data: %s \n" , send_buf+4);
    fprintf(stream, "check_sum:%x\n" , send_buf[PACKET_LEN -1]);

    fprintf(stream , "\n");
}



void packet_print(FILE * stream , struct procol_packet * pkt){
    fprintf(stream, "sending packet....");
    fprintf(stream , "version:%x\n", pkt->version);
    fprintf(stream , "type: %x\n", pkt->type);
    fprintf(stream, "data_len:%x bytes\n", pkt->data_len);
    fprintf(stream, "data: %s \n" , pkt->data);
    fprintf(stream, "check_sum:%x\n" , pkt->check_sum);
    fprintf(stream , "\n");
}
void buffer_to_packet(const char * buf , struct  procol_packet * pkt){

    pkt->version = buf[0];
    pkt->type = buf[1];
    pkt->data_len=  *(unsigned short *)(buf+2);
    sprintf(pkt->data , buf+4);
    pkt->check_sum = buf[PACKET_LEN - 1];

//    fprintf(stream , "version:%x\n", send_buf[0]);
//    fprintf(stream , "type: %x\n", send_buf[1]);
//    fprintf(stream, "data_len:%x bytes\n", * (unsigned short *)(send_buf+2));
//    fprintf(stream, "data: %s \n" , send_buf+4);
//    fprintf(stream, "check_sum:%x\n" , send_buf[PACKET_LEN -1]);

//    fprintf(stream , "\n");
}






