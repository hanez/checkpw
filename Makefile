all:
	$(CC) -Wall -o checkpw checkpw.c -lpam -lpam_misc

clean:
	rm -f ./checkpw

install:
	cp ./checkpw /usr/bin/
	cp ./checkpw.1 /usr/share/man/man1/
	gzip -f /usr/share/man/man1/checkpw.1
	cp ./checkpw.h /usr/include/

uninstall:
	rm -f /usr/bin/checkpw
	rm -f /usr/share/man/man1/checkpw.1.gz
	rm -f /usr/include/checkpw.h

