# checkpw - 1.0!

checkpw is a program that checks the validity of a users password on a Linux/PAM-based system.

## The idea behind:

Exactly a program like this... not more!

# Installation:

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
make
sudo make install
```

# Usage:
# checkpw - 1.0

checkpw is a program that checks the validity of a users password on a Linux/PAM-based system.

## They idea?

Exactly a program like this...

# Installation:

```
git clone https://git.xw3.org/hanez/checkpw.git
cd checkpw
make
sudo make install
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

