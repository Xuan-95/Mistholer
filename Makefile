CC = gcc

mistholer: mistholer.c 
	${CC} mistholer.c -o mistholer -Wall -Wextra -pedantic

