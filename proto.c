#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <proto.h>

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

    fprintf(stream, "=============packet info=============\n");
    fprintf(stream , "version:%x\n", send_buf[0]);
    fprintf(stream , "type: %x\n", send_buf[1]);
    fprintf(stream, "data_len:%x bytes\n", * (unsigned short *)(send_buf+2));
    fprintf(stream, "data: %s \n" , send_buf+4);
    fprintf(stream, "check_sum:%x\n" , send_buf[PACKET_LEN -1]);
    fprintf(stream, "=============packet info=============\n");
    fprintf(stream , "\n");
}



void packet_print(FILE * stream , struct procol_packet * pkt){
    fprintf(stream, "=============packet info=============\n");
    fprintf(stream , "version:%x\n", pkt->version);
    fprintf(stream , "type: %x\n", pkt->type);
    fprintf(stream, "data_len:%x bytes\n", pkt->data_len);
    fprintf(stream, "data: %s \n" , pkt->data);
    fprintf(stream, "check_sum:%x\n" , pkt->check_sum);
    fprintf(stream, "=============packet info=============\n");
    fprintf(stream , "\n");
}
void buffer_to_packet(const char * buf , struct  procol_packet * pkt){

    pkt->version = buf[0];
    pkt->type = buf[1];
    pkt->data_len=  *(unsigned short *)(buf+2);
    sprintf(pkt->data , buf+4);
    pkt->check_sum = buf[PACKET_LEN - 1];

}

void gen_packet(char type , short data_len , char * data, struct  procol_packet * pkt){

    pkt->version = VERSION;
    pkt->type = type;
    pkt->data_len= data_len;
    sprintf(pkt->data , data);

    pkt->check_sum = 0; //校验和为0
}



//只取数据长度
int
send_proto_packet(int fd, struct sockaddr_in  * dst,
                             char type , short data_len , char * data,
                             char * send_buf
                             )
{
    struct procol_packet pack;
    int len = sizeof(*dst);

    pack.version = VERSION;
    pack.type= type;
    pack.data_len= data_len; //3 byte
    memset(pack.data, 0, MAX_DATA); //set 0
    memcpy(pack.data,  data, data_len);
    pack.check_sum = 0; //no need to check

    memcpy(send_buf, &pack, PACKET_LEN); //cpy to send_buf
    sendto(fd,  send_buf, PACKET_LEN, 0, dst, len);

    packet_print(stdout, &pack);
}




