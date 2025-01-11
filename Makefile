all:
	$(CC) -Wall -o ./checkpw ./checkpw.c -lpam
	gzip -fk ./checkpw.1

clean:
	rm -f ./checkpw
	rm -f ./checkpw.1.gz
	rm -f ./test

install:
	install -g 0 -o 0 -m 0655 ./checkpw /usr/bin/
	install -g 0 -o 0 -m 0644 ./checkpw.h /usr/include/
	install -g 0 -o 0 -m 0644 ./checkpw.1.gz /usr/share/man/man1/

uninstall:
	rm -f /usr/bin/checkpw
	rm -f /usr/include/checkpw.h
	rm -f /usr/share/man/man1/checkpw.1.gz

test:
	$(CC) -Wall -DPAM_DEBUG -o test test.c

