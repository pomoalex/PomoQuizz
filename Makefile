COMPILER=g++

all: Server Client Database.sqlite

Server: Server.cpp Player.cpp Player.h Definitions.h
	$(COMPILER) Server.cpp Player.cpp -o Server -pthread 

Client: Client.cpp Definitions.h
	$(COMPILER) Client.cpp -o Client -lsfml-graphics -lsfml-window -lsfml-system

Database.sqlite: Database.cpp CreateDatabase.cpp Database.h
	$(COMPILER) Database.cpp CreateDatabase.cpp -lsqlite3 -o CreateDatabase
	./CreateDatabase

clean:
	rm Server Client 