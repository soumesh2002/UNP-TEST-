#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

	printf("%d\n", htonl(5120));
	printf("%d\n", htons(256));

	return 0;	
}
