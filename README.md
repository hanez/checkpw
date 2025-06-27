# MERGED PROJECT!!! I WILL UPDATE THIS README IN THE NEXT DAYS!!! THE FOLLOWING DOCUMENTATION WILL NOT WORK ACTUALLY!

# chkusr

chkusr is a set of programs to verify information about a user on UNIX based systems.

 * [The idea](#the-idea)
    * [Requirements](#requirements)
    * [Building chkusr](#building-chkusr)
    * [Installation](#installation)
    * [Uninstall](#uninstall)
 * [chkgrp](#chkgrp)
 * [chkpwd](#chkpwd)
    * [Usage](#usage)
       * [Return codes](#return-codes)
       * [Examples](#examples)
          * [Interactive mode asking for a username and a password](#interactive-mode-asking-for-a-username-and-a-password)
          * [Interactive mode only asking for a password](#interactive-mode-only-asking-for-a-password)
          * [None interactive mode with username and password provided as arguments to chkpwd](#none-interactive-mode-with-username-and-password-provided-as-arguments-to-chkpwd)
          * [Request the result from the above commands](#request-the-result-from-the-above-commands)
    * [Custom build](#custom-build)
 * [Links](#links)
 * [License](#license)

## The idea

I needed some programs to verify some information about users and groups on a Linux/UNIX system just returning 0 on success and 1 on error.

Currently chkpwd is only tested on Linux, but it should work on a [AIX](https://en.wikipedia.org/wiki/IBM_AIX), [DragonFly BSD](https://www.dragonflybsd.org/), [FreeBSD](https://www.freebsd.org/), [HP-UX](https://en.wikipedia.org/wiki/HP-UX), [Linux](https://kernel.org/), [macOS](https://en.wikipedia.org/wiki/MacOS), [NetBSD](https://netbsd.org/) and [Solaris](https://en.wikipedia.org/wiki/Oracle_Solaris) operating system too.

### Requirements

You need the PAM development package installed. On Alpine it is named linux-pam-dev, on Debian based systems it is libpam0g-dev.

### Building chkusr

```
git clone https://git.xw3.org/hanez/chkusr.git
cd chkusr
make
```

### Installation

**WARNING:** Install this software with care. chkpwd could easily be used for bruteforcing passwords from local users!

```
sudo make install
```

chkusr programs are installed to /usr/bin/.

chkpwd.h is installed to /usr/include/ for use in other applications.

### Uninstall

```
sudo make uninstall
```

## chkgrp

chkgrp is a program to verify if a user is a member of a group.

More information will follow... Actually there are multiple programs with some different behavior. I will write more about this, but for now I will not.

## chkpwd

chkpwd is a program that checks the validity of a users password on a UNIX/[PAM](https://en.wikipedia.org/wiki/Pluggable_Authentication_Module)-based system.

The code only supports verifying passwords for user id 1000 by default. Look at the file chkpwd.h for some compile time options!

### Usage

```
chkpwd -h
Usage: chkpwd [-u <username>] [-p <password>] [-v] [-V] [-h]

Options:
  -u <username>  Set username.
  -p <password>  Set password.
  -v             Enable verbose mode.
  -V             Print program version.
  -h             Show this help.
```

You can also use chkpwd even without installing by just running the following command:

```
./chkpwd
```

#### Return codes

chk returns 0 on success, 1 otherwise.

#### Examples

##### Interactive mode asking for a username and a password

```
chkpwd
```

##### Interactive mode only asking for a password

```
chkpwd -u hanez
```

##### None interactive mode with username and password provided as arguments to chkpwd

```
chkpwd -u hanez -p password
```

##### Request the result from the above commands

```
echo $?
```

#### Custom build

Set MAX_UID and MIN_UID at compile time:

```
gcc -Wall -DMAX_UID=1000 -DMIN_UID=1000 -o chkpwd chkpwd.c -lpam -lpam_misc
```

## Links

 - [https://git.xw3.org/hanez/chkusr](https://git.xw3.org/hanez/chkusr)
 - [https://github.com/shadow-maint/shadow](https://github.com/shadow-maint/shadow)
 - [https://github.com/linux-pam/linux-pam](https://github.com/linux-pam/linux-pam)
 - [https://www.man7.org/linux/man-pages/man8/unix_chkpwd.8.html](https://www.man7.org/linux/man-pages/man8/unix_chkpwd.8.html)
 - [https://cr.yp.to/checkpwd.html](https://cr.yp.to/checkpwd.html)
 - [https://pamtester.sourceforge.net/](https://pamtester.sourceforge.net/)
 - [https://github.com/AlexanderZhirov/chkpass](https://github.com/AlexanderZhirov/chkpass)
 - [https://github.com/ViKingIX/pam_test](https://github.com/ViKingIX/pam_test)
 - [https://github.com/Dareka826/chk_pw](https://github.com/Dareka826/chk_pw)

## License

chkusr is licensed under the Apache License, Version 2.0.

See LICENSE for details.

