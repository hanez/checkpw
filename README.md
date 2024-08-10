# checkpw

checkpw is a program to check the validity of a users password on a Linux/PAM-based system.

# Installation:

gcc -lpam -lpam_misc -o checkpw checkpw.c

# Usage:

./checkpw -u $USERNAME -p $PASSWORD

echo $?

