#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

//./tcp_client 127.0.0.1 8080
int main (int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [ip] [port]\n", argv[0]);
		return 1;
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

//	int re = connect(sock, (struct sockaddr*)&server, sizeof(server));
//	if(re < 0 )
	if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("connect");
		return 3;
	}

	char buf[1024];
	while(1)
	{
		printf("Please Enter: ");
		scanf("%s", buf);
		write(sock, buf, sizeof(buf)-1);
	}	

	return 0;
}
