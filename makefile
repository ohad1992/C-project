maman14run: main.o first_pass.o second_pass.o validation.o
	gcc -g -ansi -pedantic -Wall main.o first_pass.o validation.o second_pass.o -lm -o maman14run

main.o: main.c common.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

first_pass.o: first_pass.c common.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

second_pass.o: second_pass.c common.h
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

validation.o: validation.c common.h
	gcc -c -ansi -Wall -pedantic validation.c -o validation.o

