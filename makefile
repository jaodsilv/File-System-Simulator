ep3: ep3.o exit.o mount.o umount.o
	gcc -o ep3 ep3.o exit.o mount.o umount.o -lreadline -g

ep3.o: source/ep3.c
	gcc -c source/ep3.c -Wall -Wextra -pedantic -g

exit.o: source/commands/exit/exit.c
	gcc -c source/commands/exit/exit.c -Wall -Wextra -pedantic -g

mount.o: source/commands/mount/mount.c
	gcc -c source/commands/mount/mount.c -Wall -Wextra -pedantic -g

umount.o: source/commands/umount/umount.c
	gcc -c source/commands/umount/umount.c -Wall -Wextra -pedantic -g

clean:
	rm -rf source/obj/*.o
	rm -rf source/*~
	rm -rf a.out
	rm ep3

all: ep3
	mv ./*.o source/obj/
