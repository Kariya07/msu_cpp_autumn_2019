all: main.o 
	g++ main.o -pthread -o my_test

test: my_test
	./my_test

main.o: main.cpp
	g++ main.cpp -pthread -c --std=c++14

my_test: main.o 
	g++ main.o -pthread -o my_test

clean:
	rm -rf *.o my_test
