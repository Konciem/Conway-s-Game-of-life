# Replace this with the path you get from `brew info sfml`
SFML_PATH = /Users/konradciemala/Documents/SFML-3.0.0

# Replace "src" with the name of the folder where all your cpp code is
cppFileNames := $(shell find ./src -type f -name "*.cpp")


all: compile

compile:	
	mkdir -p bin
	g++ -std=c++20 $(cppFileNames) -I$(SFML_PATH)/include -o main -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -Wl,-rpath,/Users/konradciemala/Documents/SFML-3.0.0/lib && ./main
