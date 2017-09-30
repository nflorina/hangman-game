CC = gcc

build: 
	${CC} hangman.c -o hangman -lncurses -lmenu
clean:
	rm -f hangman