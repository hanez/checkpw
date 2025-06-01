# chkpw

chkpw is a program that checks the validity of a users password on a UNIX/[PAM](https://en.wikipedia.org/wiki/Pluggable_Authentication_Module)-based system.

Currently chkpw is only tested on Linux, but it should work on a [AIX](https://en.wikipedia.org/wiki/IBM_AIX), [DragonFly BSD](https://www.dragonflybsd.org/), [FreeBSD](https://www.freebsd.org/), [HP-UX](https://en.wikipedia.org/wiki/HP-UX), [Linux](https://kernel.org/), [macOS](https://en.wikipedia.org/wiki/MacOS), [NetBSD](https://netbsd.org/) and [Solaris](https://en.wikipedia.org/wiki/Oracle_Solaris) operating system too.

## The idea

I needed a program to verify passwords of users on Linux/UNIX systems using PAM that just returns 0 on success and 1 on error.

## Requirements

You need the PAM development package installed. On Alpine it is named linux-pam-dev, on Debian based systems it is libpam0g-dev.

## Building chkpw

```
git clone https://git.xw3.org/hanez/chkpw.git
cd chkpw
make
```

The code only supports verifying passwords for user id 1000 by default. Look at the file chkpw.h for some compile time options!

### Custom build example

Set MAX_UID and MIN_UID at compile time:

```
gcc -Wall -DMAX_UID=1000 -DMIN_UID=1000 -o chkpw chkpw.c -lpam -lpam_misc
```

## Installation

**WARNING:** Install this software with care. chkpw could easily be used for bruteforcing passwords from local users!

```
sudo make install
```

chkpw is installed to /usr/bin/.

chkpw.h is installed to /usr/include/ for use in other applications.

## Uninstall

```
sudo make uninstall
```

## Usage

```
chkpw -h
Usage: chkpw [-u <username>] [-p <password>] [-v] [-V] [-h]

Options:
  -u <username>  Set username.
  -p <password>  Set password.
  -v             Enable verbose mode.
  -V             Print program version.
  -h             Show this help.
```

You can also use chkpw even without installing by just running the following command:

```
./chkpw
```

### Return codes

chkpw returns 0 on success, 1 otherwise.

### Examples


#### Interactive mode asking for a username and a password

```
chkpw
```

#### Interactive mode only asking for a password

```
chkpw -u hanez
```

#### None interactive mode with username and password provided as arguments to chkpw

```
chkpw -u hanez -p password
```

#### Request the result from the above commands

```
echo $?
```

## License

chkpw is licensed under the Apache License, Version 2.0.

See LICENSE for details.

## Links

 - [https://git.xw3.org/hanez/chkpw](https://git.xw3.org/hanez/chkpw)
 - [https://cr.yp.to/checkpwd.html](https://cr.yp.to/checkpwd.html)
 - [https://pamtester.sourceforge.net/](https://pamtester.sourceforge.net/)

