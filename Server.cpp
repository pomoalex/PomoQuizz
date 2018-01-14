#include "Definitions.h"
#include "Quizz.h"
#include <vector>
#define WAITING_TIME 1
mutex locker;
extern int errno;

bool login_attempt(Player &player, chrono::high_resolution_clock::time_point &start);

void PomoQuizz(Player &player, bool &adding, bool &server_ok);

void PomoQuizzMatch(Quizz quizz);

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
	Quizz quizz;
	vector<Player> players;
	int players_added = 0;
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
				players_added++;
				printf("%s just connected\n", (char *)player.username.c_str());
			}
			adding = false;
		}
		if (passed_time(start) > WAITING_TIME)
		{
			if (players_added)
			{
				quizz.players = players;
				quizz.number_of_players = players_added;
				players.clear();
				execute = thread(PomoQuizzMatch, quizz);
				execute.detach();
				players_added = 0;
				quizz.Reinitialize();
			}
			start = chrono::high_resolution_clock::now();
			room++;
			printf("Room %d started\n", room);
		}
	}
}

void PomoQuizzMatch(Quizz quizz)
{
	thread execute;
	for (auto iterator = quizz.players.begin(); iterator != quizz.players.end(); iterator++)
	{
		execute = thread(PlayPomoQuizz, ref(quizz), *iterator);
		execute.detach();
	}
	while (!quizz.game_ended)
		;
}

void PlayPomoQuizz(Quizz &quizz, Player player)
{
	vector<string> question;
	int length;
	int given_answer;
	int correct_answer;
	for (int index = 0; index < 10; index++)
	{
		char temp[300];
		correct_answer = quizz.GetQuestion(index, question) + 1;
		sprintf(temp, "%s\n%s\n%s\n%s\n%s\n",
				(char *)question[0].c_str(),
				(char *)question[1].c_str(),
				(char *)question[2].c_str(),
				(char *)question[3].c_str(),
				(char *)question[4].c_str());
		send_to(player.socket_descriptor, temp);
		if (read_from(player.socket_descriptor, &given_answer, (int)sizeof(int)) > 0)
			printf("given answer : %d\n", given_answer);
		else
		{
			printf("%s has disconnected unexpectedly\n", (char *)player.username.c_str());
			break;
		}
		if (given_answer == correct_answer)
		{
			given_answer = 1;
			player.score++;
		}
		else
			given_answer = 0;
		send_to(player.socket_descriptor, &given_answer, (int)sizeof(int));
	}
}