#include "Definitions.h"

extern int errno;

int main(int argc, char *argv[])
{
	int client_sd;			   // socked descriptor
	struct sockaddr_in server; // structure used to connect to server
	char msg[256];			   // sent message
	// socket creation
	if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("error at socket().\n");
		return errno;
	}
	// filling server structure
	// socket family
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_port = htons(PORT);
	// connection attempt
	if (connect(client_sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("error at connect().\n");
		return errno;
	}
	bzero(msg, 256);
	fflush(stdout);
	int res;
	while ((res = read(0, msg, 100)) > 0)
	{
		msg[res - 1] = 0;
		if (write(client_sd, msg, 256) <= 0)
		{
			perror("error at write()\n");
			return errno;
		}
	}
	close(client_sd);
}