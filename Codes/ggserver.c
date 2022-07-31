#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int n;
    //char data[201];
    //create a socket
    int net_socket;
    net_socket = socket(AF_INET, SOCK_STREAM, 0);

    //connect to a server
    //where we want to connect to
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;

	//htons converts port number to proper format (byte addressing)
    server_address.sin_port = htons(5600);
	//inet_addr converts IP in string to proper IP format
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

	//bind the socket to port
    bind(net_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	//start listen process
	//10 represents size of buffer (queue)
    listen(net_socket, 10);
    char str[200] = "HelloCSES";;
    int fd;
    int c;

    while(1) {
		//accept + read/write + close
	    fd=accept(net_socket,(struct sockaddr*)&client_address,&c);
	    printf("Accepted ");
        
	    printf("%s",str);
	    write(fd,str,strlen(str));
		close(fd);  
    }

    return(0);
}

