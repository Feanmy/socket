#include "common.h"

int main(int argc, char **argv)
{

	if (argc != 2)
	{
		error(1, 0, "usage: local_udp_clnt <local_path>");
	}

	int socket_fd;
	socket_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	
	if (socket_fd < 0)
	{
		error(1, errno, "create socket failed");
	}

	struct sockaddr_un server_addr, client_addr;

	bzero(&client_addr, sizeof(client_addr));
	client_addr.sun_family = AF_LOCAL;
	strcpy(client_addr.sun_path, tmpnam(NULL));

	if (bind(socket_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
	{
		error(1, errno, "bind failed");
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sun_family = AF_LOCAL;
	strcpy(server_addr.sun_path, argv[1]);

	char send_line[MAXLINE];
	bzero(send_line, MAXLINE);
	char recv_line[MAXLINE];

	while (fgets(send_line, MAXLINE, stdin) != NULL)
	{
		int i = strlen(send_line);
		if (send_line[i-1] =='\n')
		{
			send_line[i-1] = 0;
		}
		size_t nbytes = strlen(send_line);
		printf("now sending: %s\n", send_line);

		if (sendto(socket_fd, send_line, nbytes, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < nbytes)
		{
			error(1, errno, "sendto error");
		}

		int n = recvfrom(socket_fd, recv_line, MAXLINE, 0, NULL, NULL);
		recv_line[n] = 0;

		fputs(recv_line, stdout);
		fputs("\n", stdout);
	}

	exit(0);
}
