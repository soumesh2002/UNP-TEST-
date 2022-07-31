#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
		
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (net_socket < 0)
		perror("socket failed : ");
	else
		printf("socket created\n");
	
	struct sockaddr_in serv_addr, sa, ca;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(33455);
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	int len = sizeof(struct sockaddr_in);
	int br = bind(net_socket, (struct sockaddr *)&serv_addr, len);
	if (br < 0)
		perror("bind failed : ");
	else {
		printf("bind completed\n");
		len = sizeof(struct sockaddr_in);
		getsockname(net_socket, (struct sockaddr *)&sa, &len);
		printf("local ip : %s\n", inet_ntoa(sa.sin_addr));
		printf("local port : %d\n", ntohs(sa.sin_port));
	}

	char buffer[200];
	int n;

	while (1) {
//		n = read(net_socket, buffer, 200);
		n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr *)&ca, &len);
		if (n < 0)
			perror("read failed : ");
		buffer[n] = '\0';
//		n = write(net_socket, buffer, strlen(buffer));
		n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&ca, len);
		if (n < 0)
			perror("write failed : ");
	}

	close(net_socket);
	
	return 0;	
}
