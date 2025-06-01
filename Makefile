all:
	$(CC) -Wall -o ./chkpw ./chkpw.c -lpam
	gzip -fk ./chkpw.1

clean:
	rm -f ./chkpw
	rm -f ./chkpw.1.gz
	rm -f ./test

install:
	install -g 0 -o 0 -m 0655 ./chkpw /usr/bin/
	install -g 0 -o 0 -m 0644 ./chkpw.h /usr/include/
	install -g 0 -o 0 -m 0644 ./chkpw.1.gz /usr/share/man/man1/

uninstall:
	rm -f /usr/bin/chkpw
	rm -f /usr/include/chkpw.h
	rm -f /usr/share/man/man1/chkpw.1.gz

test:
	$(CC) -Wall -DPAM_DEBUG -o test test.c

