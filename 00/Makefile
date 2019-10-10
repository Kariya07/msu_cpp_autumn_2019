
all: main.o row.o column.o 
	g++ main.o row.o column.o -o my_prog --std=c++14 -O2

run: my_prog
	./my_prog

main.o: main.cpp 
	g++ main.cpp -c --std=c++14 -O2

row.o: row.cpp
	g++ row.cpp -c --std=c++14 -O2

column.o: column.cpp
	g++ column.cpp -c --std=c++14 -O2

clean:
	rm -rf *.o my_prog
