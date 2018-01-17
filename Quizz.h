#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"
#include "Database.h"
#include "Player.h"
using namespace std;
struct Question
{
	string question;
	vector<string> answers;
	short int correct_answer;
};

class Quizz
{
  public:
	Quizz();
	~Quizz();
	void Reinitialize();
	void PrintQuestions();
	int GetQuestion(int question_nr, vector<string> &result);
	vector<Player> players;
	int highscore;
	int players_ended_game;
	int players_done;
	Player winner ;

  private:
	bool first_quizz;
	vector<Question> questions;
};