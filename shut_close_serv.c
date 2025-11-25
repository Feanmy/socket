#include "common.h"

// 全局静态变量
static int count;
// 信号处理函数
static void sig_int(int signo)
{
	printf("\nReceived %d datagrams\n", count);
	exit(0);
}

int main(int argc, char **argv)
{
	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERV_PORT);

	int rt1 = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (rt1 < 0) {
		error(1, errno, "bind failed");
	}

	int rt2 = listen(listenfd, LISTENQ);
	if (rt2 < 0) {
		error(1, errno, "listen failed");
	}

	// 注册信号处理器
	signal(SIGINT, sig_int);
	signal(SIGPIPE, SIG_IGN);

	// 保存客户端地址
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	int connfd;

	if ((connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
		error(1, errno, "accept failed");
	}

	char message[MAXLINE];
	count = 0;

	for (;;)
	{
		int n = read(connfd, message, MAXLINE);
		if (n < 0) {
			error(1, errno, "error read");
		} else if (n == 0) {
			error(1, 0, "client closed \n");
		}

		message[n] = 0;
		printf("received %d bytes: %s\n", n, message);
		count++;

		char send_line[MAXLINE];
		sprintf(send_line, "Hi, %s", message);

		sleep(15);

		int write_nc = send(connfd, send_line, strlen(send_line), 0);
		printf("send bytes: %zu \n", write_nc);

		if (write_nc < 0) {
			error(1, errno, "error write");
		}
	}		
}













































