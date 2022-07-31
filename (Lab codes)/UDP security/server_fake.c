#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

void print_socket_details(int socket)
{
        struct sockaddr_in sa, ca;
        int len = sizeof(struct sockaddr_in);
        getsockname(socket, (struct sockaddr*)&sa, &len);
        printf("local ip : %s\n", inet_ntoa(sa.sin_addr));
        printf("local port : %d\n", ntohs(sa.sin_port));
        len = sizeof(struct sockaddr_in);
        getpeername(socket, (struct sockaddr *)&ca, &len);
        printf("remote ip : %s\n", inet_ntoa(ca.sin_addr));
        printf("remote port : %d\n", ntohs(ca.sin_port));
}
int main()
{
	while(1)
	{
		char ip[20];
		int port;
		printf("enter the ip :");
		scanf("%s", ip);
		printf("enetr the port : ");
		scanf("%d", &port);

		int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;

		sendto(net_socket, "Bye...", 6, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
		close(net_socket);
	}
}
