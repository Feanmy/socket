#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "common.h"

void read_data(int sockfd)
{
	ssize_t n;
	char buf[1024];

	int time = 0;

	for (;;)
	{
		fprintf(stdout, "block in read\n");
		if ((n = readn(sockfd, buf, 1024)) == 0) {
			return;
		}
		time++;
		fprintf(stdout, "1k read for %d \n", time);
		usleep(60000);
	}
}

int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(12345);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 1024);

	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		read_data(connfd);
		close(connfd);
	}
}
