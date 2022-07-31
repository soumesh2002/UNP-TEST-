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
        getsockname(socket, (struct sockaddr*)&ca, &len);
        printf("local ip : %s\n", inet_ntoa(ca.sin_addr));
        printf("local port : %d\n", ntohs(ca.sin_port));
        len = sizeof(struct sockaddr_in);
        getpeername(socket, (struct sockaddr *)&sa, &len);
        printf("remote ip : %s\n", inet_ntoa(sa.sin_addr));
        printf("remote port : %d\n", ntohs(sa.sin_port));
}
int main(int argc, char *argv[])
{
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);

	char buffer[200];
	int n, l , len =sizeof(struct sockaddr_in);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	struct sockaddr_in sa, ca;
	int l1, l2;

	while(fgets(buffer, 200, stdin)!=NULL)
	{
		l1= len;
		sa = servaddr;
		n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&sa, l1);
		print_socket_details(net_socket);
		if(n<0)
			perror("write error : ");
		l2=len;
HERE:
		n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr *)&ca, &l2);
		if (n<0)
			perror("read error :");
		if(l1==l2 && memcmp(&sa, &ca, sizeof(struct sockaddr_in))==0)
		{
			buffer[n]='\0';
			printf("recv from server : %s\n", buffer);
		}
		else
		{
			buffer[n]='\0';
			printf("fake message %s rejected from %s:%d\n", buffer, inet_ntoa(ca.sin_addr),ntohs(ca.sin_port));
			goto HERE;
		}
			
	}
	close(net_socket);
	return(0);
}

