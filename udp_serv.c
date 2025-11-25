#include "common.h"

static int count;

static void recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}

int main(int argc, char **argv)
{
	// 创建一个基于IPV4的udp套接字
	int socket_fd;
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	// 初始化服务器地址，转为网络字节序
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定地址
	bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	socklen_t client_len;
	char message[MAXLINE];
	count = 0;

	// 信号捕捉处理函数
	signal(SIGINT, recvfrom_int);

	// 保存对端地址信息
	struct sockaddr_in client_addr;
	client_len = sizeof(client_addr);

	for(;;)
	{
		// 接收数据并保存对端地址信息
		int n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr, &client_len);
		message[n] = 0;
		printf("received %d bytes: %s\n", n, message);

		char send_line[MAXLINE];
		sprintf(send_line, "Hi, %s", message);

		// 向对端发送数据
		sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *)&client_addr, client_len);

		count++;
	}
}
