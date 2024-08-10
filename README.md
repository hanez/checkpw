# checkpw

checkpw is a program to check the validity of a users password on a Linux/PAM-based system.

# Installation:

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
gcc -lpam -lpam_misc -o checkpw checkpw.c
```

# Usage:

```
./checkpw -u $USERNAME -p $PASSWORD
echo $?
```

