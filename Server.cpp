#include "Definitions.h"
#include "Player.h"
#include "Quizz.h"
#include <vector>
#define WAITING_TIME 15
mutex locker;
extern int errno;

bool login_attempt(Player &player, chrono::high_resolution_clock::time_point &start);

void PomoQuizz(Player &player, bool &adding, bool &server_ok);

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
	while (1)
	{
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
		Player player;
		bool adding = false;
		bool server_ok = true;
		thread execute(PomoQuizz, ref(player), ref(adding), ref(server_ok));
		execute.detach();
		while ((client_sd = accept(server_sd, (struct sockaddr *)&client, &length)) > 0)
		{
			while (adding)
				;
			player.socket_descriptor = client_sd;
			adding = true;
		}
		server_ok = false;
	}
}

bool login_attempt(Player &player, chrono::high_resolution_clock::time_point &start)
{
	char username[256];
	if (recv_from(player.socket_descriptor, username) > 0)
	{
		char message[256];
		sprintf(message, "%d\n", (WAITING_TIME + 3 - (int)passed_time(start)));
		player.username = username;
		if (send_to(player.socket_descriptor, message) > 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

void PomoQuizz(Player &player, bool &adding, bool &server_ok)
{
	srand(time(NULL));
	Quizz quizz;
	vector<Player> players;
	bool players_added = false;
	auto start = chrono::high_resolution_clock::now();
	int room = 1;
	thread execute;
	printf("Room %d started\n", room);
	while (server_ok)
	{
		if (adding)
		{
			if (login_attempt(player, start))
			{
				players.push_back(player);
				players_added = true;
				printf("%s just connected\n", (char *)player.username.c_str());
			}
			adding = false;
		}
		if (passed_time(start) > WAITING_TIME)
		{
			if (players_added)
			{
				quizz.Reinitialize();
				for (auto iterator = players.begin(); iterator != players.end(); iterator++)
				{
					execute = thread(PlayPomoQuizz, ref(quizz), *iterator);
					execute.detach();
				}
				players.clear();
				players_added = false;
			}
			start = chrono::high_resolution_clock::now();
			room++;
			printf("Room %d started\n", room);
		}
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