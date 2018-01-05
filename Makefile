COMPILER=g++

all: Server Client Database.sqlite

Server: Server.cpp Player.cpp
	$(COMPILER) Server.cpp Player.cpp -o Server -pthread 

Client: Client.cpp
	$(COMPILER) Client.cpp -o Client

Database.sqlite: Database.cpp CreateDatabase.cpp 
	$(COMPILER) Database.cpp CreateDatabase.cpp -lsqlite3 -o CreateDatabase
	./CreateDatabase

clean:
	rm Server Client 