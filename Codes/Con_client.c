#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
// signal(signal_id, function_name)

int net_socket;

void int_handel(int sig)
{
	printf("\nExit from client .....\n");
	close(net_socket);
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, int_handel);
	// create socket
	net_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// connect to server
	// define address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[2]));
	server_address.sin_addr.s_addr = inet_addr(argv[1]);

	// call connect function
	// int connect(int socket, const struct sockaddr *address, socklen_t address_len);
	//  connect() call attempts to establish a connection between two sockets.
	// If successful, connect() returns 0. If unsuccessful, connect() returns -1.
	int con_res = connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	if (con_res < 0) {
		printf("Error on connect\nExiting...\n");
		close(net_socket);
		exit(0);
	}

	//recv data
	char data[200], str[201];
	int n;
	while(1)
	{
		printf("\nEnter : ");
		scanf(" %s", data);
		write(net_socket, data, strlen(data));
		// memset(str, 0, strlen(str));
		n = recv(net_socket, str, 199, 0);
		str[n] = '\0';
		printf("Received from server: %s\n", str);
		printf("IP of server: %s\n", inet_ntoa(server_address.sin_addr));
		printf("Port of server: %d\n", ntohs(server_address.sin_port));
	}
	//close the connection
	close(net_socket);
	return(0);
}
