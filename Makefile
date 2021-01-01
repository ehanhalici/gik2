calistir: derle
	./prog

derle:
	gcc main.c -oprog  -lpng -Wall -g

uninstall:
	rm prog
	rm out.png
