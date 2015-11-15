ep3: ep3.o bitmap.o fat.o fildir.o cat.o cp.o df.o exit.o find.o mkdir.o rm.o rmdir.o ls.o mount.o touch.o umount.o
	gcc -o ep3 ep3.o bitmap.o fat.o fildir.o cat.o cp.o df.o exit.o find.o mkdir.o rm.o rmdir.o ls.o mount.o touch.o umount.o -lreadline -g

ep3.o: source/ep3.c
	gcc -c source/ep3.c -Wall -Wextra -pedantic -g

bitmap.o: source/bitmap.c
	gcc -c source/bitmap.c -Wall -Wextra -pedantic -g

fat.o: source/fat.c
	gcc -c source/fat.c -Wall -Wextra -pedantic -g

fildir.o: source/fildir.c
	gcc -c source/fildir.c -Wall -Wextra -pedantic -g

cat.o: source/commands/cat/cat.c
	gcc -c source/commands/cat/cat.c -Wall -Wextra -pedantic -g

cp.o: source/commands/cp/cp.c
	gcc -c source/commands/cp/cp.c -Wall -Wextra -pedantic -g

df.o: source/commands/df/df.c
	gcc -c source/commands/df/df.c -Wall -Wextra -pedantic -g

exit.o: source/commands/exit/exit.c
	gcc -c source/commands/exit/exit.c -Wall -Wextra -pedantic -g

mkdir.o: source/commands/mkdir/mkdir.c
	gcc -c source/commands/mkdir/mkdir.c -Wall -Wextra -pedantic -g

mount.o: source/commands/mount/mount.c
	gcc -c source/commands/mount/mount.c -Wall -Wextra -pedantic -g

rm.o: source/commands/rm/rm.c
	gcc -c source/commands/rm/rm.c -Wall -Wextra -pedantic -g

rmdir.o: source/commands/rmdir/rmdir.c
	gcc -c source/commands/rmdir/rmdir.c -Wall -Wextra -pedantic -g

ls.o: source/commands/ls/ls.c
	gcc -c source/commands/ls/ls.c -Wall -Wextra -pedantic -g

find.o: source/commands/find/find.c
	gcc -c source/commands/find/find.c -Wall -Wextra -pedantic -g

touch.o: source/commands/touch/touch.c
	gcc -c source/commands/touch/touch.c -Wall -Wextra -pedantic -g

umount.o: source/commands/umount/umount.c
	gcc -c source/commands/umount/umount.c -Wall -Wextra -pedantic -g

clean:
	rm -rf source/obj/*.o
	rm -rf source/*~
	rm -rf a.out
	rm ep3

all: ep3
	mv ./*.o source/obj/
