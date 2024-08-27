# checkpw - 1.0.1!

checkpw is a program that checks the validity of a users password on a Linux/PAM-based system.

## The idea behind:

I needed a program to verify passwords of users on Linux based systems using PAM.

Exactly a program like this... not more!

## Installation:

**WARNING:** Install this software with care. checkpw could easily be used for bruteforcing passwords from local users!

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
make
sudo make install
```

The code only supports verifying passwords for user id 1000 by default. Look a the code for some compile time options!

Set MAX_UID and MIN_UID in the code or you can compile checkpw without editing the code using the following command and install it manually:

```
gcc -Werror -DMAX_UID=1000 -DMIN_UID=1000 -o checkpw checkpw.c -lpam -lpam_misc
sudo cp ./checkpw /usr/bin/
```

## Uninstall:

```
sudo make uninstall
```

## Usage:

```
checkpw -h
Usage: checkpw [-u <username>] [-p <password>] [-i] [-v] [-h]

Options:
  -u <username>  Specify username.
  -p <password>  Specify password.
  -i             Enable interactive mode to prompt for missing username/password.
  -v             Enable verbose mode.
  -h             Show this help.
```

Returns 0 on success, 1 otherwise.

### Examples:

#### Interactive mode:

```
checkpw -i
```

#### Interactive mode only asking for a password:

```
checkpw -u hanez -i
```

#### None interactive mode with username and password provided as arguments to checkpw:

```
checkpw -u hanez -p password
```

#### Request the result from the above commands:

```
echo $?
```

## Links:

 - [https://cr.yp.to/checkpwd.html](https://cr.yp.to/checkpwd.html)

