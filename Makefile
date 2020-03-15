all:
	gcc -o main main.c -Wall -Wextra
	gcc -o construction functions.c construction.c
	gcc -o painter functions.c painter.c
	gcc -o checkinglevel functions.c checkinglevel.c
	gcc -o combine functions.c combine.c List.c
