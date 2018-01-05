#include "Definitions.h"
#include "Player.h"

extern int errno;

void close_socket(int *socket_descriptor, int seconds)
{
	auto start = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point finish;
	while (1)
	{
		finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		if (elapsed.count() > seconds)
		{
			shutdown(*socket_descriptor,SHUT_RDWR );
			break;
		}
	}
}

int main()
{
	struct sockaddr_in server;
	int server_sd; // server socket descriptor
	bzero(&server, sizeof(server));
	// filling server structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	int nr_client = 0;
	int room = 0;
	while (1)
	{
		room++;
		printf("Room %d started !\n",room);
		if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("error at socket().\n");
			return errno;
		}
		int on = 1;
		setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on));
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
		struct sockaddr_in client;
		char msg[100];
		char msgrasp[100] = " ";
		bzero(&client, sizeof(client));
		int child_pid;
		int client_sd;
		socklen_t length = sizeof(client);
		fflush(stdout);
		auto start = chrono::high_resolution_clock::now();
		thread terminate_accept(close_socket, &server_sd, 5);
		terminate_accept.detach();
		while (1)
		{
			client_sd = accept(server_sd, (struct sockaddr *)&client, &length);
			if (client_sd < 0)
				break;
			else
			{
				nr_client++;
				printf("Client %d just connected in room %d !\n", nr_client, room);
				close(client_sd);
			}
		}
	}
}