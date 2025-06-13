FLAGS = -Wall -Werror -Wextra -std=c++11
all: gen 

gen: gen.o nist.o stats.o 
	g++ $(FLAGS) -o gen  gen.o nist.o stats.o 

gen.o:  gen.o 
	g++ $(FLAGS) -c gen.cpp

nist.o: nist.cpp
	g++ $(FLAGS) -c nist.cpp

stats.o: stats.cpp
	g++ $(FLAGS) -c stats.cpp
	

clean:
	rm -rf *.o gen 

rebuild: clean all