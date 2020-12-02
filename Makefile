all: src/main.cpp src/main.o
	g++ -std=c++17 -Wall -Wextra -g -Iinclude -c src/main.cpp  -o src/main.o
	g++ -std=c++17 -Wall -Wextra -g -Iinclude -o AlgoTowers src/main.o  -Llib
clean:
	$(RM) AlgoTowers
