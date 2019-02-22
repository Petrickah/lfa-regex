all: regex.exe

regex.exe: src\nfa_helper.cpp src\nfa.cpp src\Regex.cpp src\regexp.cpp
	g++ -g -c -std=c++17 -Wreorder -o bin\nfa.o src\nfa.cpp
	g++ -g -c -std=c++17 -Wreorder -o bin\Regex.o src\Regex.cpp
	g++ -g -c -std=c++17 -Wreorder -o bin\regexp.o src\regexp.cpp
	g++ -g -c -std=c++17 -Wreorder -o bin\nfa_helper.o src\nfa_helper.cpp
	g++ -g -std=c++17 -Wreorder -o regex.exe bin\nfa_helper.o bin\nfa.o bin\Regex.o bin\regexp.o