#include <iostream>
#include "Database.h"
using namespace std;
int main()
{
	Database *database;
	database = new Database("Database.sqlite");
	database->query("DROP TABLE Questions;");
	database->query("CREATE TABLE Questions (\
					Question VARCHAR(100),\
					Answer1 VARCHAR(100),\
					Answer2 VARCHAR(100),\
					Answer3 VARCHAR(100),\
					Answer4 VARCHAR(100),\
					Category INTEGER);");
	database->query("INSERT INTO Questions VALUES(\
					'What is the name of McDonald`s famous clown ?',\
					'Ronald',\
					'Big Mac',\
					'Krusty',\
					'Donald',\
					1);");
	database->query("INSERT INTO Questions VALUES(\
					'Foundue is usually cheese or chocolate served how ?',\
					'Melted',\
					'Frozen',\
					'A la mode',\
					'Overpriced',\
					1);");
	database->query("INSERT INTO Questions VALUES(\
					'What is the essential ingredient of \"sauerkraut\" ?',\
					'Cabbage',\
					'Noodles',\
					'Onion',\
					'Potato',\
					1);");
	database->query("INSERT INTO Questions VALUES(\
					'In which Disney film is Cruella de Vil encountered ?',\
					'101 Dalmations',\
					'Lion King',\
					'Snow White',\
					'The Aristocats',\
					1);");
	database->query("INSERT INTO Questions VALUES(\
					'What is the name of the video game character who ate ghosts and dots ?',\
					'Pac-Man',\
					'Lara Croft',\
					'Super Mario',\
					'Crash Banicoot',\
					1);");
	/*
	vector<vector<string>> result = database->query("SELECT * FROM Questions;");
	for (vector<vector<string>>::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		cout << "Values: Questions=" << row.at(0) << ", an1=" << row.at(1) << ", an2=" << row.at(2) << ", an3=" << 
		row.at(3) << ", an4=" << row.at(4) << ", cat=" << row.at(5) << endl;
	}
	database->close();
	*/
	return 0;
}
