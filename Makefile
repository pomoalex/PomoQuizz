COMPILER=g++

all: server client

server: server.cpp
	$(COMPILER) server.cpp -o server 

client: client.cpp
	$(COMPILER) client.cpp -o client

clean:
	rm server client