#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


void serviceIO(int sock)		//处理请求
{
	char buf[1024];

	while(1)
	{
		ssize_t s = read(sock, buf, sizeof(buf)-1);

		if( s > 0 )
		{
			buf[s] = 0;
			printf("client# %s\n", buf);
		}
		else if(s == 0)			//代表对端将链接关闭
		{
			printf("client quit.\n");		//读到文件结尾
			break;
		}
		else
		{
			perror("read");
			break;
		}
	}

	close(sock);
}



void *service(void *arg)
{
	int sock = (int)arg;
	serviceIO(sock);
}

//tcp_server ip port
int main(int  argc, char *argv[])
{
	if( argc != 3 )
	{
		printf("Usage:%s [ip] [port]\n", argv[0]);
		return 1;
	}	

	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);		//创建一个监听套接字
	if( listen_sock <0 )
	{
		perror("socket");
		return 2;
	}
	
	struct sockaddr_in local;	//初始化socket API
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);

	if( bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0 )	//给服务器绑定一个固定的ip和port
	{
		perror("bind");
		return 3;		
	}

	if( listen(listen_sock, 5) < 0 )		//监听套接字
	{
		perror("listen");
		return 4;
	}

	for( ; ; )
	{
		struct sockaddr_in client;	
		socklen_t len = sizeof(client);
		int sock = accept(listen_sock, (struct sockaddr*)&client, &len);	//获得新链接

		if( sock < 0 )
		{
			perror("accept");
			continue;
		}
#if 0		
		//多进程
		pid_t id = fork();
		if(id == 0)			//child
		{
			close(listen_sock); 		//子进程不需要listen_sock

			if(fork() > 0)	 //子进程退出，孙子进程提供服务
			{
				exit(0);
			}
	
			serviceIO(sock);
			exit(0);
		}
		else
		{
			close(sock);		//必须关闭，父进程一直运行，父进程
								//可用的文件描述符越来越少，因此必须关闭
			waitpid(id, NULL, 0); 
		}
		
#endif
		printf("get new link[%s][%d]...\n", inet_ntoa(client.sin_addr) \
											, ntohs(client.sin_port));
		//多线程
		pthread_t id;
		pthread_create(&id, NULL, service, (void* )sock);
		pthread_detach(id);


		serviceIO(sock);		//处理请求
	}	
	
	return 0;
}
