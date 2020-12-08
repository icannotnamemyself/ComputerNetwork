#ifndef PROTO
#define PROTO


#include <stdio.h>
//#define MAX_DATA 65535//max data length 16bytes
//#define PACKET_LEN 65540

#define MAX_DATA 35535//max data length 16bytes
#define PACKET_LEN 35540
#define VERSION 5 //协议版本号


 enum PROCOL_DATATYPE{
    CONNECT = 1,   //建立连接
    CERTIFICATE = 2,  //认证
    DATA = 6,  //数据
    DISCONNECT = 7  //断开连接
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

/**
 * @brief gen_packet 版本号固定为5 ， 校验和由函数计算
 * @param type 数据类型
 * @param data_len 协议数据长度
 * @param data  协议数据
 * @param pkt 输出参数
 */
extern void gen_packet(char type , short data_len , char * data, struct  procol_packet * pkt);

#endif // PROTO
