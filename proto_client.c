
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "proto.h"
#define SERVER_PORT 8888
#define BUFF_LEN 512
#define SERVER_IP "172.0.5.182"




//
//
void udp_msg_sender(int fd, struct sockaddr* dst)
{

    socklen_t len;
    struct sockaddr_in src;
    int i = 0;


    char send_buf[PACKET_LEN];
    char buf[BUFF_LEN];
    printf("handle sendimsg!!!\n");
    //certificate pack
    struct procol_packet pack;
    pack.version = 5;
    pack.type= CONNECT;
    pack.data_len= 3; //3 byte
    memset(pack.data, 0, MAX_DATA); //set 0
    sprintf(pack.data, "%c", 0xff); //需要认证
    sprintf(pack.data+1,"%hu", 0x0010); // data len : 16 bytes
    pack.check_sum = 0; //no need to check

    memcpy(send_buf, &pack, PACKET_LEN);
    printf("sending connect packet to\n");
    fprintf(stdout, "sending connect packet to\n");


    while(1)
    {
//        printf("sdfsd");
        struct procol_packet pp;
        buffer_to_packet(send_buf, &pp);
//        buffer_to_packet(send_buf , &pp);
//        packet_print(stdout, &pp);
//    packet_buf_print(stdout, );
    //发送认证包
    sendto(fd,  send_buf, PACKET_LEN, 0, dst, len);


#if 0
    //char buf[BUFF_LEN] = "TEST UDP MSG!\n";
memset(buf, 0, BUFF_LEN);
buf[0] = ('a' + i++%26);         //ascii 97
len = sizeof(*dst);
    printf("client send:%s\n",buf);  //打印自己发送的信息
    sendto(fd, buf, BUFF_LEN, 0, dst, len);
    memset(buf, 0, BUFF_LEN);
    recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&src, &len);  //接收来自server的信息
    printf("server send back:%s\n",buf);
  #endif // 0
        sleep(1);  //一秒发送一次消息
    }



}

/*
    client:
            socket-->sendto-->revcfrom-->close
*/

int main(int argc, char* argv[])
{
    if(argc < 3){
	printf("请输入目的地址与端口号! \n");
	return 0;
    }


    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //注意网络序转换
    //ser_addr.sin_port = htons(SERVER_PORT);  //注意网络序转换
    ser_addr.sin_port =htons(atoi(argv[2]));  //注意网络序转换

    udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);
    close(client_fd);

    return 0;
}

