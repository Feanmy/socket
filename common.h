#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define MAXLINE 4096
#define SERV_PORT 12345
#define LISTENQ 5

#define MSG_PING 1
#define MSG_PONG 2
#define MSG_TYPE1 11
#define MSG_TYPE2 21

typedef struct {
	u_int32_t type;
	char data[1024];
} messageObject;


size_t readn(int fd, void *buffer, size_t size)
{
	char *buffer_pointer = buffer;
	int length = size;

	while (length > 0)
	{
		int result = read(fd, buffer_pointer, length);
		if (result < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				return -1;
			}
		}
		else if (result == 0)
		{
			break;
		}

		length -= result; // 剩余未读的字节
		buffer_pointer += result; // 缓冲区指针向后移动
	}

	return (size - length); // 实际已读取的字节数
}
