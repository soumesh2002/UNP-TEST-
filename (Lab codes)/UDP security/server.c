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
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(net_socket< 0)
		perror("socket failed : ");
	else
		printf("socket created...\n");
	printf("\n::: socket details before bind :::\n");
	print_socket_details(net_socket);


	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =htons(33456);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int br = bind(net_socket, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
	if(br<0)
		perror("bin failed :");
	else
		printf("bind completed \n");
	printf("\n::: after bind :::: \n");
	print_socket_details(net_socket);

	char buffer[200];
	struct sockaddr_in ca;
	int n , len=sizeof(struct sockaddr_in);

	while(1)
	{
		int l = len;
		n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr*)&ca, &l);
		if(n<0)
			perror("read error :");
		sleep(20);
		buffer[n] = '\0';
		l = len;
		n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&ca, l);
		if(n<0)
			perror("write erroe :");

	}	
	close(net_socket);
	return(0);


}
