#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
// pid_t wait(int *sat_loc);
//#include<time.h>
#include<string.h>

int l_socket;

void do_nothing(int sig)
{
	pid_t child_id;
	// pid_t wait(int *wstatus);
	// A call to wait() blocks the calling process until one of its child processes exits or a signal is received. After child process terminates, parent continues its execution after wait system call instruction. 
	child_id = wait(NULL);
	printf("The child terminated is : %d\n", child_id);
}

void handle_int(int sig)
{
	printf("\nExit from server .....\n");
	close(l_socket);
	exit(0);
}

int main(int argc, char *argv[])
{
	// void (*signal(int sig, void (*func)(int)))(int)
	// signal function sets a function to handle signal i.e. a signal handler with signal number sig.
	// This function returns the previous value of the signal handler, or SIG_ERR on error.
	signal(SIGCHLD, do_nothing);
	signal(SIGINT, handle_int);

	// listening socket
	// int socket(int *family, int type, int protocol);
	// If successful, socket() returns a nonnegative socket descriptor.
	// If unsuccessful, socket() returns -1 on error
	l_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (l_socket < 0) {
		printf("Socket creation failed!!\nExiting...\n");
		exit(0);
	}

	// store server details
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;

	// in_port_t htons(in_port_t hostshort);
	// The htons() function translates a short integer from host byte order to network byte order.
	server_addr.sin_port = htons(0);

	// in_addr_t htonl(in_addr_t hostlong);
	// The htonl() function translates a long integer from host byte order to network byte order.
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;//inet_addr(argv[1])i;

	// srever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// inet_aton(char *, struct in_adrr *)
	// inet_aton("127.0.0.1", &server_addr.sin_addr);
	// char * inet_ntoa(struct in_addr );
	// char ip[30] = net_ntoa(server_addr.sin_addr);

	// sizeof() is used to calculate the size (in bytes)that a data type occupies in ​the computer’s memory
	int len = sizeof(struct sockaddr_in);

	// bind the socket to port
	// The bind() function shall assign a local socket address address to a socket identified by descriptor socket that has no local socket address assigned.
	// int bind(int socket, const struct sockaddr *address, socklen_t address_len);
	// On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error

	int bind_res = bind(l_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (bind_res < 0) {
		printf("Error on bind!!\nExiting...\n");
		close(l_socket);
		exit(0);
	}

	// int getsockname(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
	// returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr
	// On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error
	int sock_name = getsockname(l_socket, (struct sockaddr *)&server_addr, &len);
	if (sock_name < 0) {
		printf("Error on getting socket details!!\nExiting...\n");
		close(l_socket);
		exit(0);
	}

	// in_port_t ntohs(in_port_t netshort);
	// The ntohs() function translates a short integer from network byte order to host byte order.
	printf("Port number is : %d\n", (int)ntohs(server_addr.sin_port));

	// start listen process
	// int listen(int sockfd, int backlog);
	// listen() marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests
	// On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error
	int listen_res = listen(l_socket, 10);
	if (listen_res < 0) {
		printf("Error in listen function!!\nExiting...\n");
		close(l_socket);
		exit(0);
	}

	int s_socket;
	struct sockaddr_in client_addr;
	int client_sz= sizeof(struct sockaddr);
	char data[200], str[201];
	int n, c = 1;
	//time_t tick;
	while(1)
	{
		// accept + (read/ write) + close_new_socket
		// int accept(int socket, struct sockaddr *__restrict__ address, socklen_t *__restrict__address_len);
		// The accept() call is used by a server to accept a connection request from a client.
		// On success, these system calls return a file descriptor for the accepted socket (a nonnegative integer).  On error, -1 is returned, errno is set to indicate the error, and addrlen is left unchanged.
		s_socket = accept( l_socket, (struct sockaddr *) &client_addr, &client_sz);
		if (s_socket < 0) {
			printf("Error in accept!!\nExiting...\n");
			close(l_socket);
			exit(0);
		}

		// Fork system call is used for creating a new process, which is called child process, which runs concurrently with the process that makes the fork() call (parent process)
		if (fork()==0)
		{
			close(l_socket);
			while(1)
			{
				// ssize_t recv(int socket, void *buffer, size_t length, int flags);
				// The recv() function receives data on a socket with descriptor socket and stores it in a buffer.
				n = recv(s_socket, data, 199, 0);
				if(n == 0)
						break;
				data[n] = '\0';
				printf("Received from client %d: %s\n", c, data);
				printf("IP of client %d: %s\n", c, inet_ntoa(client_addr.sin_addr));
				printf("Port of client %d: %d\n", c, ntohs(client_addr.sin_port));

				// int printf(const char *format, ...)
				// returns the total number of characters written is returned.
				printf("Enter : ");

				// int scanf(const char *format, ...)
				// returns the number of items of the argument list successfully read.
				scanf("%s", str);

				// ssize_t write(int fd, const void *buf, size_t count);
				// write() writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd.
				write(s_socket, str, strlen(str));
			}

			// int getpeername(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
			// getpeername() returns the address of the peer connected to the socket sockfd, in the buffer pointed to by addr.
			// On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.
			int peer_name = getpeername(s_socket, (struct sockaddr *) &client_addr, &client_sz);
			if (peer_name < 0) {
				printf("Error in geeting peer detail!!\nExiting...\n");
				close(l_socket);
				exit(0);
			}

			// char *inet_ntoa(struct in_addr in);
			// inet_ntoa() accepts an Internet address expressed as a 32-bit quantity in network byte order and returns a string expressed in dotted-decimal notation.
			printf("%s is the ip exiting... \n", inet_ntoa(client_addr.sin_addr));

			// in_port_t ntohs(in_port_t netshort);
			// The ntohs() function translates a short integer from network byte order to host byte order.
			printf("%d is the port of client \n", ntohs(client_addr.sin_port));

			// sockaddr_in -> sin_family
			// sockaddr_in -> sin_port
			// sockaddr_in -> sin_addr -> s_addr

			// int close(int fd);
			// close() closes a file descriptor, so that it no longer refers to any file and may be reused.
            close(s_socket);

			// void exit(int status)
			// exit() terminates the calling process immediately. Any open file descriptors belonging to the process are closed and any children of the process are inherited by process 1, init, and the process parent is sent a SIGCHLD signal.
			exit(0);
		}
		else
		{
			c++;
			close(s_socket);
		}
	}
	return(0);
}
