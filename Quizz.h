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
private:
	vector<Question> questions;
};