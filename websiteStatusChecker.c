
#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <netdb.h>

#include <string.h>

#include <unistd.h>


void dnsLookup(char *hostname, char *ip) {

	struct hostent *hostEnt;

	struct in_addr **inAddr;

	hostEnt = gethostbyname(hostname);

	if(hostEnt == NULL) {

		perror("gethostbyname");

		exit(0);

	}

	else {

		inAddr = (struct in_addr **)hostEnt->h_addr_list;

		for(int i = 0; inAddr[i] != NULL; i++) {

			strcpy(ip, inet_ntoa(*inAddr[i]));

		}

	}

}


int main(int argc, char *argv[]) {

	int socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if(socketFD > 0) {

		struct timeval timeout;

		timeout.tv_sec = atoi(argv[3]);

		timeout.tv_usec = 0;

		if(setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0 || setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {

			perror("setsockopt");

			exit(0);

		}

                char ip[] = {""};

                dnsLookup(argv[1], ip);

		struct sockaddr_in connectionStruct;

		connectionStruct.sin_family = AF_INET;

		connectionStruct.sin_addr.s_addr = inet_addr(ip);

		connectionStruct.sin_port = htons(atoi(argv[2]));

		int connectionResult = connect(socketFD, (const struct sockaddr *)&connectionStruct, (socklen_t)sizeof(connectionStruct));

		if(connectionResult == 0) {

			printf("%s\n", "Website is up");

		}

		else {

			printf("%s\n", "Website is down");

		}

	}

	else {

		perror("Socket Creation");

		exit(0);

	}


}
