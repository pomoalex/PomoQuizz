#include "Definitions.h"

extern int errno;

int main()
{
	struct sockaddr_in server;
	struct sockaddr_in from;
	char msg[100];
	char msgrasp[100] = " ";
	int server_sd; // server socket descriptor
	//server socket description
	if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("error at socket().\n");
		return errno;
	}
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));
	// filling server structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	if (bind(server_sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("error at bind().\n");
		return errno;
	}
	if (listen(server_sd, 1) == -1)
	{
		perror("error at listen().\n");
		return errno;
	}
	int child_pid;
	int nr_client = 0;
	int client;
	socklen_t length = sizeof(from);
	//
	auto start = chrono::high_resolution_clock::now();
	int last = 0;
	//
	while (1)
	{
		fflush(stdout);
		client = accept(server_sd, (struct sockaddr *)&from, &length);
		nr_client++;

		//
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		if (floor(elapsed.count()) != last)
		{
			printf("%d\n", (int)elapsed.count());
			last = floor(elapsed.count());
		}
		//

		if (client < 0)
		{
			perror("error at accept().\n");
			return errno;
		}
		else
			printf("Client %d just connected !\n", nr_client);
		if ((child_pid = fork()) < 0)
		{
			perror("error at fork()");
			exit(1);
		}
		if (!child_pid)
		{
			bzero(msg, 100);
			fflush(stdout);
			int res;
			while ((res = read(client, msg, 256)) > 0)
			{
				msg[res] = 0;
				printf("[client %d] : %s\n", nr_client, msg);
			}
			if (res < 0)
			{
				perror("error at read() \n");
				close(client);
			}
			if (res == 0)
			{
				printf("Client %d disconnected !\n", nr_client);
				close(client);
			}
			close(client);
		}
		close(client);
	}
}