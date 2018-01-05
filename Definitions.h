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
#define PORT 4321
#define SERVER_IP "127.0.0.1"

using namespace std;