COMPILER=g++

all: server client

server: Server.cpp
	$(COMPILER) Server.cpp -o Server 

client: Client.cpp
	$(COMPILER) Client.cpp -o Client

database: Database.cpp CreateDatabase.cpp 
	g++ Database.cpp CreateDatabase.cpp -lsqlite3 -o CreateDatabase
	./CreateDatabase

clean:
	rm Server Client