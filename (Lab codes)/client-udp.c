#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (net_socket < 0)
		perror("socket failed : ");
	else 
		printf("socket created\n");
	
	struct sockaddr_in serv_addr, sa, ca;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	int len = sizeof(struct sockaddr_in);
/*	int cr = connect(net_socket, (struct sockaddr *)&serv_addr, len);
	if (cr < 0)
		perror("connect failed : ");
	else {
		len = sizeof(struct sockaddr_in);
		getsockname(net_socket, (struct sockaddr *)&ca, &len);
		printf("client ip : %s\n", inet_ntoa(ca.sin_addr));
		printf("client port : %d\n", ntohs(ca.sin_port));
		len = sizeof(struct sockaddr_in);
		getpeername(net_socket, (struct sockaddr *)&sa, &len);
		printf("server ip : %s\n", inet_ntoa(sa.sin_addr));
		printf("server port : %d\n", ntohs(sa.sin_port));
	}
*/

	int n;
	char buffer[200];

	while (fgets(buffer, 200, stdin) != NULL) {
//		n = write(net_socket, buffer, strlen(buffer));
		n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, len);
		if (n < 0)
			perror("write failed : ");
//		n = read(net_socket, buffer, 200);
		n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr *)&serv_addr, &len);
		if (n < 0)
			perror("read failed : ");
		buffer[n] = '\0';
		printf("server sent : %s\n", buffer);
	}

	close(net_socket);

	return 0;	
}
