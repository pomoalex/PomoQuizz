#pragma once
#include <string>
using namespace std;

class Player
{
  public:
	Player();
	~Player();
	string username;
	int socket_descriptor;
  private:
};