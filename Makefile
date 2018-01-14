COMPILER=g++

all: Server Client Database.sqlite

Server: Server.cpp Quizz.o Player.o Database.o Definitions.h
	$(COMPILER) Server.cpp Quizz.o Player.o Database.o -o Server -pthread -lsqlite3

Quizz.o: Quizz.cpp Quizz.h 
	$(COMPILER) Quizz.cpp -c

Player.o: Player.cpp Player.h
	$(COMPILER) Player.cpp -c 

Database.o: Database.h Database.cpp 
	$(COMPILER) Database.cpp -c 

Client: Client.cpp Definitions.h
	$(COMPILER) Client.cpp -o Client -lsfml-graphics -lsfml-window -lsfml-system -pthread

Database.sqlite: Database.o CreateDatabase.cpp Database.h
	$(COMPILER) Database.o CreateDatabase.cpp -lsqlite3 -o CreateDatabase
	./CreateDatabase

clean:
	rm Server Client Player.o Quizz.o Database.o 