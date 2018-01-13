#pragma once
#include <string>
using namespace std;

class Player
{
  public:
	Player();
	~Player();
	string username;
	int score = 0;
	int socket_descriptor;

  private:
};