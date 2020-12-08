
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "proto.h"
#define SERVER_PORT 8888
#define BUFF_LEN 512
#define SERVER_IP "172.0.5.182"


void client_handle_connected(int fd , struct sockaddr* oserver_addr){

    struct procol_packet send_pkt;  //procol 协议
    char send_buf[PACKET_LEN];
    int count ;
    struct sockaddr_in server_addr;
    int len ;
    char buf[MAX_DATA];

    len= sizeof(server_addr);


    while (1) {

        send_data_pack(fd, oserver_addr);
//        memset(send_buf,0,PACKET_LEN);
//        memset(buf, 0  , 1024);
//        printf("请输入数据:\n");
//        scanf(" %s", buf);

//        //create proto package
//        send_pkt.version = 5;
//        send_pkt.type= DATA;
//        send_pkt.data_len= 1024; //3 byte
//        memset(send_pkt.data, 0, MAX_DATA); //set 0
//        memcpy(send_pkt.data,  buf, strlen(buf));
//        //sprintf(send_pkt.data, "%c", 0x00); //需要认证
//        //sprintf(send_pkt.data+1,"%hu", 0x0010); // data len : 16 bytes
//        send_pkt.check_sum = 0; //no need to check
//        memcpy(send_buf, &send_pkt, PACKET_LEN);  //cpy to send_buf
//        sendto(fd,  send_buf, PACKET_LEN, 0, oserver_addr, len);

        sleep(1);
    }
}

void send_data_pack(int fd, struct sockaddr * dst){

    int len = sizeof (*dst);
    struct procol_packet pack;
    char send_buf[PACKET_LEN];
    char buf[MAX_DATA];

    memset(send_buf,0,PACKET_LEN);
    memset(buf, 0  , 1024);

    printf("请输入数据:");
    scanf("%s", buf);

    pack.version = 5;
    pack.type= DATA;
    pack.data_len= 3; //3 byte
    memset(pack.data, 0, MAX_DATA); //set 0
    memcpy(pack.data,  buf, MAX_DATA);
    pack.check_sum = 0; //no need to check
    //cpy to send_buf
    memcpy(send_buf, &pack, PACKET_LEN);
    sendto(fd,  send_buf, PACKET_LEN, 0, dst, len);

    buffer_to_packet(send_buf , &pack);
    packet_print(stdout, &pack);
    /////
}


void send_request_connect_pack(int fd, struct sockaddr * dst){
    /////

    int len = sizeof (*dst);
    struct procol_packet pack;
    char send_buf[PACKET_LEN];
    pack.version = 5;
    pack.type= CONNECT;
    pack.data_len= 3; //3 byte
    memset(pack.data, 0, MAX_DATA); //set 0
    sprintf(pack.data, "%c", 0xff); //需要认证
    sprintf(pack.data+1,"%hu", 0x0010); // data len : 16 bytes
    pack.check_sum = 0; //no need to check
    //cpy to send_buf
    memcpy(send_buf, &pack, PACKET_LEN);
    sendto(fd,  send_buf, PACKET_LEN, 0, dst, len);
    /////
}
//
//
void udp_msg_sender(int fd, struct sockaddr* dst)
{

    char buf[BUFF_LEN];  //缓冲区，1024字节
    char recv_buf[PACKET_LEN]; //包接收缓冲区， 35540字节
    char send_buf[PACKET_LEN]; //包接收缓冲区， 35540字节
    struct procol_packet recv_pkt;  //接收包
    struct procol_packet send_pkt; //发送包
    struct procol_packet pp; //发送包
    int is_cert; //是否需要认证
    int is_connected; //连接是否建立成功
    short data_block_len=16; //数据块长度
    socklen_t len; //socket对应协议的长度
    int count; //接收缓冲区的count
    struct sockaddr_in server_addr;  //clent_addr用于记录发送方的地址信息

    len = sizeof(*dst);
    memset(recv_buf, 0, PACKET_LEN);
    //memset(send_buf, 0, PACKET_LEN);

    //create proto package
    send_pkt.version = 5;
    send_pkt.type= CONNECT;
    send_pkt.data_len= 3; //3 byte
    memset(send_pkt.data, 0, MAX_DATA); //set 0
    sprintf(send_pkt.data, "%c", 0xff); //不需要认证
    sprintf(send_pkt.data+1,"%hu", 0x0010); // data len : 16 bytes
    send_pkt.check_sum = 0; //no need to check


    memcpy(send_buf, &send_pkt, PACKET_LEN);  //cpy to send_buf
    //send_request_connect_pack(fd, dst);
    sendto(fd,  send_buf, PACKET_LEN, 0, dst, len);

    //scanf会影响sendto...????
    printf("正在请求建立连接， 请输入数据块大小:");
    scanf(" %d", &data_block_len);

    //buffer_to_packet(send_buf, &send_pkt);
    buffer_to_packet(send_buf, &pp);
    packet_print(stdout, &pp);
    fprintf(stdout, "sending connect packet to server\n");



    //接收服务器响应， 需要认证时则进行认证，不需要代表连接建立
    count = recvfrom(fd, recv_buf, PACKET_LEN, 0, (struct sockaddr*)&server_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞

    buffer_to_packet(recv_buf, &recv_pkt);

    if(recv_pkt.data[0] ==0){
        printf("服务器不需要进行认证.....\n连接建立...\n");
        is_cert = 0;
    }else{
        printf("服务器需要认证\n");
        is_cert = 1;
    }

    if(is_cert == 0){
        is_connected = 1;
        client_handle_connected(fd, dst);
    }else{

    }



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
    printf("连接: %s:%s\n" , argv[1], argv[2]);
    udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);
    close(client_fd);

    return 0;
}

