#include "Definitions.h"
#include "Player.h"
#include <vector>
#include <mutex>
#define WAITING_TIME 30
mutex locker;
extern int errno;

void close_socket(int *socket_descriptor, chrono::high_resolution_clock::time_point start, int seconds)
{
	chrono::high_resolution_clock::time_point finish;
	while (1)
	{
		finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		if (elapsed.count() > seconds)
		{
			shutdown(*socket_descriptor, SHUT_RDWR);
			close(*socket_descriptor);
			break;
		}
	}
}

void login_attempt(vector<Player> &players, int socket_descriptor, chrono::high_resolution_clock::time_point start)
{
	char username[256];
	if (recv_from(socket_descriptor, username))
	{
		Player new_player;
		new_player.username = username;
		new_player.socket_descriptor = socket_descriptor;
		locker.lock();
		players.push_back(new_player);
		locker.unlock();
		char message[256];
		auto current_time = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = current_time - start;
		sprintf(message, "Game starts in %d second !\n", (WAITING_TIME - (int)floor(elapsed.count())));
		send_to(socket_descriptor, message);
	}
	else
		close(socket_descriptor);
}

void PomoQuizz(vector<Player> players)
{
	char message[256];
	sprintf(message,"Game has started !\n");
	for (auto iterator = players.begin(); iterator != players.end(); iterator++)
	{
		send_to(iterator->socket_descriptor, message);
	}
	while (1)
		;
}

int main()
{
	struct sockaddr_in server;
	int server_sd;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	struct sockaddr_in client;
	int client_sd;
	socklen_t length = sizeof(client);
	int nr_client = 0;
	int room = 0;
	while (1)
	{
		room++;
		printf("Room %d started !\n", room);
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
		bzero(&client, sizeof(client));
		fflush(stdout);
		vector<Player> players;
		auto start = chrono::high_resolution_clock::now();
		thread terminate_accept(close_socket, &server_sd, start, WAITING_TIME);
		terminate_accept.detach();
		while ((client_sd = accept(server_sd, (struct sockaddr *)&client, &length)) > 0)
		{
			nr_client++;
			printf("Client %d just connected in room %d !\n", nr_client, room);
			thread login(login_attempt, ref(players), client_sd, start);
			login.join();
		}
		thread match(PomoQuizz, players);
		match.detach();
		players.clear();
	}
}