calistir: derle
	./prog

derle:
	gcc main.c -oprog  -lpng -Wall -g  -ggdb

uninstall:
	rm prog
	rm out.png
