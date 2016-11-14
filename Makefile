all: server client queue_test

server: server.cpp queue.h
	g++ -g -Wall -pedantic server.cpp -std=c++11 -pthread -o server

client: client.cpp queue.h
	g++ -g -Wall -pedantic client.cpp -std=c++11 -pthread -o client

queue_test: queue_test.cpp queue.h
	g++ -g -Wall -pedantic queue_test.cpp -std=c++11 -pthread -o queue_test

test: server client
	./server &
	./client

clean:
	rm server client queue_test
