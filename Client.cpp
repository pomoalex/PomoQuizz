#include "Definitions.h"

extern int errno;

int main()
{
	int client_sd;
	struct sockaddr_in server;
	char username[256];
	recv_from(0, username);
	if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("error at socket().\n");
		return errno;
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_port = htons(PORT);
	if (connect(client_sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("error at connect().\n");
		return errno;
	}
	fflush(stdout);
	send_to(client_sd, username);
	char message[256];
	recv_from(client_sd, message);
	printf("%s\n", message);
	recv_from(client_sd, message);
	printf("%s\n", message);
	while (1)
		;
	close(client_sd);
}