all:
	$(CC) -Wall -o checkpw checkpw.c -lpam -lpam_misc

clean:
	rm -f ./checkpw

install:
	cp ./checkpw /usr/bin/

uninstall:
	rm -f /usr/bin/checkpw

