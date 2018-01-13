#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"
#include "Database.h"
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
	void GetQuestion(int question_nr, vector<string> &result);

  private:
	bool concluded = false;
	string winner;
	int score;
	bool first_quizz;
	vector<Question> questions;
};