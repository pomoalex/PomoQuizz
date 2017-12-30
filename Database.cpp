#include "Database.h"
#include <iostream>

Database::Database(char const *filename)
{
	database = NULL;
	open(filename);
}

Database::~Database()
{
}

bool Database::open(char const *filename)
{
	if (sqlite3_open(filename, &database) == SQLITE_OK)
		return true;
	return false;
}

vector<vector<string>> Database::query(char const *query)
{
	sqlite3_stmt *statement;
	vector<vector<string>> results;
	if (sqlite3_prepare_v2(database, query, -1, &statement, NULL) == SQLITE_OK)
	{
		int nr_of_columns = sqlite3_column_count(statement);
		int result = 0;
		while ((result = sqlite3_step(statement)) == SQLITE_ROW)
		{
			vector<string> values;
			for (int column = 0; column < nr_of_columns; column++)
			{
				std::string val;
				char *ptr = (char *)sqlite3_column_text(statement, column);
				if (ptr)
				{
					val = ptr;
				}
				else
					val = "";
				values.push_back(val);
			}
			results.push_back(values);
		}
		sqlite3_finalize(statement);
	}
	string error = sqlite3_errmsg(database);
	if (error != "not an error")
		cout << query << " " << error << endl;
	return results;
}

void Database::close()
{
	sqlite3_close(database);
}