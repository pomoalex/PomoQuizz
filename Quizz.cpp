#include "Quizz.h"
#include <algorithm>
#include <random>

Quizz::Quizz()
{
	Reinitialize();
}

Quizz::~Quizz()
{
}

void Quizz::Reinitialize()
{
	Database *database;
	database = new Database("Database.sqlite");
	vector<vector<string>> result = database->query("SELECT * FROM Questions  \
		ORDER BY RANDOM() LIMIT 10;");
    srand(time(0));
	for (auto iterator = result.begin(); iterator < result.end(); ++iterator)
	{
		vector<string> row = *iterator;
		Question new_question;
		new_question.question = row.at(0);
		string correct_answer = row.at(1);
		vector<string> answers;
		answers.push_back(row.at(1));
		answers.push_back(row.at(2));
		answers.push_back(row.at(3));
		answers.push_back(row.at(4));
		random_device rd;
		random_shuffle(answers.begin(), answers.end());
		new_question.answers = answers;
		for (int index = 0; index < 4; index++)
			if (answers[index] == correct_answer)
			{
				new_question.correct_answer = index;
				break;
			}
		questions.push_back(new_question);
	}
	database->close();
}

void Quizz::PrintQuestions()
{
	for (int index = 0; index < 10; index++)
	{
		auto temp = questions[index];
		printf("%d) %s\na) %s	b) %s	c) %s	d) %s	correct : %d\n\n", index + 1, (temp.question).c_str(), (temp.answers[0]).c_str(),
			   (temp.answers[1]).c_str(), (temp.answers[2]).c_str(), (temp.answers[3]).c_str(), temp.correct_answer);
	}
}