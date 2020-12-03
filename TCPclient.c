#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(int argc, char * argv[]){
    if(argc < 3){
	printf("请输入目的地址与端口号! \n");
	return 0;
    } 

	//创建套接字
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	//服务器的ip为本地，端口号自由制定
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	//设置目的地址
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	//向服务器发送连接请求
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	//发送并接收数据
	char buffer[40];
	printf("Please write:");
	scanf("%s", buffer);
	while(strcmp(buffer, "end")!=0){
		memset(&buffer, 0, sizeof(buffer));
		//循环给服务器发送a-z
		for(int i= 0 ; i < 26;i++){
			buffer[0] = 'a'+i;
			write(sock, buffer, sizeof(buffer));
			sleep(0.5);  //睡眠0.5s
		}
	
		printf("Please write:");
		scanf("%s", buffer);
	}
	strcpy(buffer, "over");
	write(sock, buffer, sizeof(buffer));
	read(sock, buffer, sizeof(buffer) - 1);
	printf("Server send: %s\n", buffer);

	//断开连接
	close(sock);

	return 0;
}
