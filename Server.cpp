#include "Definitions.h"
#include "Player.h"
#include "Quizz.h"
#include <vector>
#define WAITING_TIME 2
mutex locker;
extern int errno;

void close_socket(int *socket_descriptor, chrono::high_resolution_clock::time_point start, int seconds);

void login_attempt(vector<Player> &players, int socket_descriptor, chrono::high_resolution_clock::time_point start);

void PomoQuizz(vector<Player> players);

void PlayPomoQuizz(Quizz &quizz, Player player);

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
		do
		{
			while ((client_sd = accept(server_sd, (struct sockaddr *)&client, &length)) >= 0)
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
			printf("aici intra\n");
			break;
		}
	}
}

void login_attempt(vector<Player> &players, int socket_descriptor, chrono::high_resolution_clock::time_point start)
{
	char username[256];
	if (recv_from(socket_descriptor, username) > 0)
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
		sprintf(message, "%d\n", (WAITING_TIME - (int)floor(elapsed.count())));
		send_to(socket_descriptor, message);
	}
	else
	{
		shutdown(socket_descriptor, SHUT_RDWR);
		close(socket_descriptor);
	}
}

void PomoQuizz(vector<Player> players)
{
	Quizz quizz;
	thread execute;
	for (auto iterator = players.begin(); iterator != players.end(); iterator++)
	{
		execute = thread(PlayPomoQuizz, ref(quizz), *iterator);
		execute.detach();
	}
}

void PlayPomoQuizz(Quizz &new_quizz, Player player)
{
	Quizz quizz = new_quizz;
	vector<string> question;
	for (int index = 0; index < 10; index++)
	{
		char temp[256];
		quizz.GetQuestion(index, question);
		sprintf(temp, "%s\n", (char *)question[0].c_str());
		if (!send_to(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}
		sprintf(temp, "%s\n", (char *)question[1].c_str());
		if (!send_to(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}
		sprintf(temp, "%s\n", (char *)question[2].c_str());
		if (!send_to(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}
		sprintf(temp, "%s\n", (char *)question[3].c_str());
		if (!send_to(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}
		sprintf(temp, "%s\n", (char *)question[4].c_str());
		if (!send_to(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}
		/*if (!recv_from(player.socket_descriptor, temp))
		{
			printf("Client exited unexpectedly .\n");
			break;
		}*/
	}
}