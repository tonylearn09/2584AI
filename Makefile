all: client stdio

client:
	g++ -O3 -std=c++11 *.cpp main/client_main.cpp Fib2584/*.cpp -o ai_client

stdio:
	g++ -O3 -std=c++11 *.cpp main/main.cpp Fib2584/*.cpp -o ai_stdio

old:
	g++ -O3 -std=c++11 *.cpp main/old.cpp Fib2584/*.cpp -o ai_old

train:
	g++ -O3 -std=c++11 *.cpp main/train.cpp Fib2584/*.cpp -o train_game

clean:
	rm ai_client ai_stdio train_game
