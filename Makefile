all:
	$(CC) -Wall -o checkpw checkpw.c -lpam -lpam_misc

clean:
	rm -f ./checkpw

install:
	cp ./checkpw /usr/bin/
	cp ./checkpw.h /usr/include/

uninstall:
	rm -f /usr/bin/checkpw
	rm -f /usr/include/checkpw.h

