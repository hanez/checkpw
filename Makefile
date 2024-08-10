all:
	$(CC) -Werror -lpam -lpam_misc -o checkpw checkpw.c

clean:
	rm -f ./checkpw

install:
	cp ./checkpw /usr/bin/

