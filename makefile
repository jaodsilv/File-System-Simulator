ep3: ep3.o exit.o
	gcc -o ep3 ep3.o exit.o -lreadline -g

ep3.o: source/ep3.c
	gcc -c source/ep3.c -Wall -Wextra -pedantic -g

exit.o: source/commands/exit/exit.c
	gcc -c source/commands/exit/exit.c -Wall -Wextra -pedantic -g

clean:
	rm -rf source/obj/*.o
	rm -rf source/*~
	rm -rf a.out
	rm ep3

all: ep3
	mv ./*.o source/obj/
