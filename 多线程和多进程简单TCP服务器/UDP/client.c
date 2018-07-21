#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[] )
{	
	if( argc != 3 )
	{
		printf("Usage: %s [server_port] [server_ip\n]", argv[0]);
		return 1;
	}	

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock < 0 )
	{	
		perror("socket");
		return 2;
	}


	char buf[1024];
	while(1)
	{
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(atoi(argv[2]));
		server.sin_addr.s_addr = inet_addr(argv[1]);			//将点分十进制转换成二进制
		
		printf("Please Enter: ");
		ssize_t s = read(0,  buf, sizeof(buf)-1);
		if( s > 0 )
		{
			buf[s] = 0;
			sendto(sock, buf, sizeof(buf), 0, \
				(struct sockaddr*)&server, sizeof(server));
			recvfrom(sock, buf, sizeof(buf) - 1, 0,	NULL, NULL);		//不关心谁给我发的
			printf("server echo# %s", buf);
		}
	}

	
	return 0;
}
