#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	int netSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (netSocket < 0)
		perror("socket failed : ");
	else 
		printf("socket created\n");

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	int choice, n, len;
	char buffer[200] = "time: ";

	while (1) {
		printf("Enter choice: ");
		scanf(" %d", &choice);
		if (choice == 0) {
			printf("Exiting\n")	;
			break;
		} else if (choice == 1) {
			len = sizeof(struct sockaddr_in);
			n = sendto(netSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, len);
			if (n < 0)
				perror("write failed : ");
			n = recvfrom(netSocket, buffer, 200, 0, (struct sockaddr *)&serv_addr, &len);
			if (n < 0)
				perror("read failed : ");
			buffer[n] = '\0';
			printf("Time: %s\n", buffer);	
		} else 
			printf("Invalid choice\n");
	}

	close (netSocket);

	return 0;
}
