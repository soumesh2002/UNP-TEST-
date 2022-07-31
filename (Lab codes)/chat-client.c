#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	int net_socket;
	net_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address, client_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3001);
	server_address.sin_addr.s_addr = inet_addr(argv[1]);

	connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	char str[200], data[200];
	int n;

	while (1) {
		scanf("%s", data);
		write(net_socket, data, strlen(data));
//		memset(str, 0, sizeof(str));
		n = recv(net_socket, str, 199, 0);
		str[n] = '\0';
		printf("Received from Server: %s\n", str);
	}

	close(net_socket);

	return 0;	
}
