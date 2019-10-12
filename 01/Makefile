all: main.o 
	g++ main.o -o my_prog --std=c++14

run: my_prog
	./my_prog ${args}

test: test.py
	python3 test.py

main.o: main.cpp 
	g++ main.cpp -c --std=c++14 -O2

clean:
	rm -rf *.o my_prog
