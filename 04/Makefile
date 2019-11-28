
my_prog: main.o methods.o
	g++ main.o methods.o -o my_prog --std=c++14 -O2

test: my_prog
	./my_prog

main.o: main.cpp 
	g++ main.cpp -c --std=c++14 -O2

methods.o: methods.cpp
	g++ methods.cpp -c --std=c++14 -O2

clean:
	rm -rf *.o my_prog
