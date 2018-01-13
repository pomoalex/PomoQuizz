#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <chrono>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#define PORT 4321
#define SERVER_IP "127.0.0.1"
#define TIME_PER_ANSWER 30

using namespace std;

int send_to(int file_descriptor, char *message)
{
	int result;
	result = write(file_descriptor, message, strlen(message));
	if (result <= 0)
		return result;
}

int recv_from(int file_descriptor, char *message)
{
	int result;
	result = read(file_descriptor, message, 256);
	if (result > 0)
		if (message[result - 1] == '\n')
			message[result - 1] = '\0';
	return result;
}

bool time_passed(chrono::high_resolution_clock::time_point start, int time)
{
	chrono::duration<double> elapsed;
	auto end = chrono::high_resolution_clock::now();
	elapsed = start - end;
	if (floor(elapsed.count()) > time)
		return true;
	else
		return false;
}