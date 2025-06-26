all:
	$(CC) -Wall -o ./chkgrp ./chkgrp.c
	$(CC) -Wall -o ./chkgrp-min ./chkgrp-min.c
	$(CC) -Wall -o ./chkgrp-native ./chkgrp-native.c
	$(CC) -Wall -o ./chkgrp-native-min ./chkgrp-native-min.c
	$(CC) -Wall -o ./chkgrp-native-min-extended ./chkgrp-native-min-extended.c
	$(CC) -Wall -o ./chkgrp-ng ./chkgrp-ng.c
	$(CC) -Wall -o ./chkpwd ./chkpwd.c -lpam
	gzip -fk ./chkpwd.1

clean:
	rm -f ./chkgrp
	rm -f ./chkgrp-min
	rm -f ./chkgrp-native
	rm -f ./chkgrp-native-min
	rm -f ./chkgrp-native-min-extended
	rm -f ./chkgrp-ng
	rm -f ./chkpwd
	rm -f ./chkpwd.1.gz
	rm -f ./test

install:
	install -g 0 -o 0 -m 0655 ./chkgrp /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkgrp-min /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkgrp-native /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkgrp-native-min /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkgrp-native-min-extended /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkgrp-ng /usr/bin/
	install -g 0 -o 0 -m 0655 ./chkpwd /usr/bin/
	install -g 0 -o 0 -m 0644 ./chkpwd.h /usr/include/
	install -g 0 -o 0 -m 0644 ./chkpwd.1.gz /usr/share/man/man1/
	install -g 0 -o 0 -m 0655 ./lua/chkgrp-native-min-extended.lua /usr/bin/

uninstall:
	rm -f /usr/bin/chkgrp
	rm -f /usr/bin/chkgrp-min
	rm -f /usr/bin/chkgrp-native
	rm -f /usr/bin/chkgrp-native-min
	rm -f /usr/bin/chkgrp-native-min-extended
	rm -f /usr/bin/chkgrp-native-min-extended.lua
	rm -f /usr/bin/chkgrp-ng
	rm -f /usr/bin/chkpwd
	rm -f /usr/include/chkpwd.h
	rm -f /usr/share/man/man1/chkpwd.1.gz

test:
	$(CC) -Wall -DPAM_DEBUG -o test test.c

