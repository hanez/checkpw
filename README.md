# checkpw

checkpw is a program that checks the validity of a users password on a Linux/PAM-based system.

# Installation:

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
gcc -lpam -lpam_misc -Werror -o checkpw checkpw.c
sudo cp ./checkpw /usr/bin/
```

# Usage:

```
./checkpw [-u <username>] [-p <password>] [-i] [-v] [-h]

Options:
  -u <username>  Specify username.
  -p <password>  Specify password.
  -i             Enable interactive mode to prompt for missing username/password.
  -v             Enable verbose mode.
  -h             Show this help
```

Returns 0 on success, 1 otherwise.

```
echo $?
```
