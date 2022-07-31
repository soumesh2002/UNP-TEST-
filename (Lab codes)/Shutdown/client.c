#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/select.h>
int main(int argc, char *argv[])
{
	int net_socket;
	net_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_addr.s_addr = inet_addr(argv[1]);
	//inet_aton(argv[1], &server_address.sin_addr);
	//inet_pton(AF_INET, agrv[1], &server_address.sin_addr);
	//server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(atoi(argv[2]));
	server_address.sin_family = AF_INET;
	int sz = sizeof(struct sockaddr_in);
	connect(net_socket, (struct sockaddr *)&server_address, sz);

	FILE *fp;
	fp  = fopen("file.txt", "r");
	int fd = fileno(fp);
	char data[201];
	int n;
	int maxfd;
	fd_set read;
	FD_ZERO(&read);
	int flag =0;

	while(1)
	{
		if (flag==0)
		{
			FD_SET(fd, &read);
			FD_SET(net_socket, &read);
		}
		else
			FD_SET(net_socket, &read);
		if(flag==0)
			maxfd = (net_socket > fd ? net_socket : fd)+1;
		else
			maxfd  = net_socket +1;
		select (maxfd, &read, NULL, NULL, NULL);
		if (FD_ISSET(fd, &read))
		{
			if(fgets(data,200, fp)!=NULL)
			{
				write(net_socket, data, strlen(data));
			}
			else
			{
				printf("Data sent completed ...\n");
				shutdown(net_socket, SHUT_WR);
				flag =1;
			}
		}
		if(FD_ISSET(net_socket, &read))
		{
			n = recv(net_socket, data,  200, 0);
			if(n==0)
			{
				printf("Data recv completed...");
				break;
			}
			else
				printf("%s\n",data);
		}
	}
	printf("ddd`\n");
	shutdown(net_socket, SHUT_RD);
	return(0);
}
