#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char data[201];
    //create a socket
    int net_socket, len;
	len = sizeof(struct sockaddr_in);
    net_socket = socket(AF_INET, SOCK_STREAM, 0);

    //connect to a server
    //where we want to connect to
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;

	//htons converts port number to proper format (byte addressing)
    server_address.sin_port = htons(0);

	//inet_addr converts IP in string to proper IP format
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind the socket to port
    bind(net_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	getsockname(net_socket, (struct sockaddr *)&server_address, &len);
	printf("After bind ephemeral prot = %d\n", (int) ntohs(server_address.sin_port));

	//start listen process
	//10 represents size of buffer (queue)
    listen(net_socket, 10);
    char str[200];
    int fd;
    int c;

    while(1) {
		//accept + read/write + close
	    fd=accept(net_socket,(struct sockaddr*)&client_address,&len);
		while (1) {
			int k = recv(fd, str, 199, 0);
			if (k == 0) 
				break;
			str[k] = '\0';
			printf("Received from Client: %s\n",str);
		    write(fd,str,strlen(str));
		}
		close(fd);  
    }

    return(0);
}
