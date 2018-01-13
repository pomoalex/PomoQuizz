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

int send_to(int file_descriptor, void *message, int size = -1)
{
	int result;
	if (size == -1)
		{
			size = strlen((char *)message);
			send_to(file_descriptor,&size,sizeof(int));
		}
	result = write(file_descriptor, message, size);
	return result;
}

int read_from(int file_descriptor, void *data, int size)
{
	int result;
	result = read(file_descriptor, data, size);
	return result;
}

int recv_from(int file_descriptor, char *message,int size = -1)
{
	int result;
	if(size == -1)
		read_from(file_descriptor,&size,sizeof(int));
	result = read(file_descriptor, message, size);
	if (result > 0)
		if (message[result - 1] == '\n')
			message[result - 1] = '\0';
	return result;
}

double passed_time(chrono::high_resolution_clock::time_point start)
{
	chrono::duration<double> elapsed;
	auto end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	return elapsed.count();
}