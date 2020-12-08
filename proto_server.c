#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "proto.h"


#define SERVER_PORT 8888
#define BUFF_LEN 1024

//双方约定的口令
static char * TOKEN ="ABCDEFG";

void server_handle_connected(int fd ,struct sockaddr_in *oclent_addr){

     struct procol_packet recv_pkt;  //procol 协议
     struct procol_packet pack;
     char recv_buf[PACKET_LEN];
     char send_buf[PACKET_LEN];
     int count ;
     struct sockaddr_in clent_addr;
     int len = sizeof(clent_addr);
     int is_disconnect;
     do{
         //接收客户端数据
         printf("wait for data...\n");
         count = recvfrom(fd, recv_buf, PACKET_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞

         buffer_to_packet(recv_buf , &recv_pkt);
         packet_print(stdout, &recv_pkt);


         if(recv_pkt.data_len ==0){
             printf("客户端想要断开连接，是否同意?(0/1):");
             scanf(" %d", &is_disconnect);


             send_proto_packet(fd , oclent_addr,is_disconnect?DISCONNECT:CONNECT
                               ,0,"",send_buf
                               );


             if(is_disconnect){
                 printf("断开连接\n");
                 return ;
             }else{
                 printf("不同意断开，请求客户端继续发送..\n");
             }


         }




         fflush(stdout);
     }while (1) ;
}


void handle_udp_msg(int fd)
{
    char buf[BUFF_LEN];  //缓冲区，1024字节
    char recv_buf[PACKET_LEN]; //包接收缓冲区， 35540字节
    char send_buf[PACKET_LEN]; //包接收缓冲区， 35540字节
    struct procol_packet recv_pkt;  //接收包
    struct procol_packet send_pkt; //发送包
    int is_cert; //是否需要认证
    int is_connected; //连接是否建立成功
    short data_block_len=16; //数据块长度
    socklen_t len; //socket对应协议的长度
    int count; //接收缓冲区的count
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息


    len = sizeof(clent_addr);
    memset(recv_buf, 0, PACKET_LEN);
    memset(send_buf, 0, PACKET_LEN);

    //客户端发起连接请求
    printf("wating for client\n");
    count = recvfrom(fd, recv_buf, PACKET_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
    if(count == -1)
    {
        printf("recieve data fail!\n");
        return;
    }
    printf("recieve data success!\n");
    fflush(stdout);
    fflush(stdin);
    buffer_to_packet(recv_buf, &recv_pkt);
    if(recv_pkt.type == CONNECT)
        printf("数据单元类型为1 ");
    else {
        printf("数据单元类型不正确, 退出程序\n");
        return ;
    }
    printf("收到客户端连接请求...，包信息为:\n");
    buffer_to_packet(recv_buf , &recv_pkt);
    packet_print(stdout, &recv_pkt);

    data_block_len = recv_pkt.data_len;
    printf("是否要求客户端进行认证?(输入1或0):");
    scanf(" %d", &is_cert);



    if(is_cert){
        //需要进行认证
        sprintf(buf ,"%c", (char)0xff);

        send_proto_packet(fd , (struct sockaddr*)&clent_addr,CERTIFICATE
                          ,1,buf,send_buf
                          );


        //接收认证口令
        count = recvfrom(fd, recv_buf, PACKET_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        buffer_to_packet(recv_buf ,  &recv_pkt);
        if(strcmp(TOKEN, recv_pkt.data) ==0){
            printf("口令正确， 认证成功....\n建立连接...\n");

            send_proto_packet(fd , (struct sockaddr*)&clent_addr,CONNECT
                              ,2,"\0\0",send_buf
                              );
            is_connected=1;
        }else{
            printf("口令错误，认证失败....断开连接!\n");
            send_proto_packet(fd , (struct sockaddr*)&clent_addr,DISCONNECT
                              ,2,"\0\0",send_buf
                              );
            return ;
        }


    }else {
        send_proto_packet(fd , (struct sockaddr*)&clent_addr,CONNECT
                          ,2,"\0\0",send_buf
                          );

        printf("建立连接......\n");
        //连接成功建立
        is_connected=1;
    }

    if(is_connected){
        //连接建立， 进入处理状态
        server_handle_connected(fd , &clent_addr);
    }else{


    }

}


/*
    server:
            socket-->bind-->recvfrom-->sendto-->close
*/

int main(int argc, char* argv[])
{
    int server_fd, ret;
    struct sockaddr_in ser_addr;

    server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return -1;
    }

    handle_udp_msg(server_fd);   //处理接收到的数据

    close(server_fd);
    return 0;
}
