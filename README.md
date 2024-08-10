# checkpw

checkpw is a program that checks the validity of a users password on a Linux/PAM-based system.

Returns 0 on success, 1 otherwise.

# Installation:

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
gcc -lpam -lpam_misc -Werror -o checkpw checkpw.c
```

# Usage:

```
./checkpw -u $USERNAME -p $PASSWORD
echo $?
```

