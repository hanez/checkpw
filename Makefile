all:
	$(CC) -Wall -o ./chkpwd ./chkpwd.c -lpam
	gzip -fk ./chkpwd.1

clean:
	rm -f ./chkpwd
	rm -f ./chkpwd.1.gz
	rm -f ./test

install:
	install -g 0 -o 0 -m 0655 ./chkpwd /usr/bin/
	install -g 0 -o 0 -m 0644 ./chkpwd.h /usr/include/
	install -g 0 -o 0 -m 0644 ./chkpwd.1.gz /usr/share/man/man1/

uninstall:
	rm -f /usr/bin/chkpwd
	rm -f /usr/include/chkpwd.h
	rm -f /usr/share/man/man1/chkpwd.1.gz

test:
	$(CC) -Wall -DPAM_DEBUG -o test test.c

