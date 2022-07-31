#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	// create a socket
	int net_socket;
	// AF_INET defines IPV4 
	// SOCK_STREAM defines TCP connection
	
	net_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(5600);
	server_address.sin_addr.s_addr = inet_addr(argv[1]);

	// call connect function
	connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	// recieve data -> recv()
	char data[201];
	int n;
	n = recv(net_socket, data, 200, 0);	

	printf("n = %d\n", n);

	char *token = strtok(data, " ");
	strtok(NULL, " ");
	strtok(NULL, " ");
	token = strtok(NULL, " ");

	printf("%s\n", token);

	char *hr = strtok(token, ":");
	char *min = strtok(NULL, ":");
	char *sec = strtok(NULL, ":");

	int total = atoi(hr) * 3600 + atoi(min) * 60 + atoi(sec);
	printf("%d %d %d \n", atoi(hr), atoi(min), atoi(sec));
	total -= 5 * 3600 + 30 * 60;
	printf("%d\n", total);

	int hour = total / 3600;
	total %= 3600;
	int minute = total / 60;
	total %= 60;
	
	printf("%d : %d : %d\n", hour, minute, total);

	// close the connection
	close(net_socket);
	return 0; 
} 
