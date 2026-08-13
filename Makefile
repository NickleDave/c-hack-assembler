./Assembler : bin/main.o
	gcc -o ./Assembler bin/main.o

bin/main.o : src/main.c
	gcc -Wall -g -c src/main.c -o bin/main.o

clean:
	rm bin/*.o
	rm ./Assembler
