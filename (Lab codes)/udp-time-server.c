#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	
	int netSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (netSocket < 0)
		perror("socket failed : ");
	else 
		printf("socket created\n");

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(33455);
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	int len = sizeof(struct sockaddr_in);
	int br = bind(netSocket, (struct sockaddr *)&serv_addr, len);
	if (br < 0)
		perror("bind failed : ");
	else {
		printf("bind successful\n");
		len = sizeof(struct sockaddr_in);
		getsockname(netSocket, (struct sockaddr *)&serv_addr, &len);
		printf("server ip : %s\n", inet_ntoa(serv_addr.sin_addr));
		printf("server port : %d\n", ntohs(serv_addr.sin_port));
	}

	int n;
	char buffer[200];
	time_t tick;

	while (1) {
		len = sizeof(struct sockaddr_in);
		n = recvfrom(netSocket, buffer, 200, 0, (struct sockaddr *)&serv_addr, &len);
		if (n < 0) 
			perror("read failed : ");
		tick = time(NULL);
		snprintf(buffer, 200, "%s", ctime(&tick));
		n = sendto(netSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, len);
		if (n < 0)
			perror("write failed : ");
	}

	close (netSocket);

	return 0;
}
