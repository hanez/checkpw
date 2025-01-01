/**
 * checkpw is a program that checks the validity of a users password on a
 * UNIX/PAM-based system.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2024
 * License: Apache-2.0 (see LICENSE)
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "checkpw.h"

// Function to prompt user for input, optionally hiding input
void prompt_for_input(char *buffer, size_t size, const char *prompt,
                      bool hide_input)
{
    printf("%s", prompt);
    fflush(stdout);

    if (hide_input) {
        struct termios oldt, newt;
        if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
            perror("tcgetattr");
            exit(1);
        }
        newt = oldt;
        newt.c_lflag &= ~(ECHO);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
            perror("tcsetattr");
            exit(1);
        }
    }

    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "\nError reading input.\n");
        if (hide_input) {
            // Restore terminal settings
            struct termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            oldt.c_lflag |= ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
        exit(1);
    }

    // Remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';

    if (hide_input) {
        // Restore terminal settings
        struct termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        oldt.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        printf("\n"); // Move to the next line after password input
    }
}

void print_usage(const char *prog_name)
{
    printf("\n");
    printf("Usage: %s [-u <username>] [-p <password>] [-v] [-V] [-h]\n",
           prog_name);
    printf("\n");
    printf("Options:\n");
    printf("  -u <username>  Set username (if not set, the program asks for it).\n");
    printf("  -p <password>  Set password (if not set, the program asks for it).\n");
    printf("  -v             Enable verbose mode.\n");
    printf("  -V             Print program version.\n");
    printf("  -h             Show this help.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    bool verbose = false;
    bool version = false;
    char password[MAX_PASSWORD_LEN] = {0};
    char username[MAX_USERNAME_LEN] = {0};
    int opt;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "u:p:hvV")) != -1) {
        switch (opt) {
            case 'u':
                if (strlen(optarg) >= MAX_USERNAME_LEN) {
                    fprintf(stderr, "Error: Username is too long (maximum %d characters).\n",
                            MAX_USERNAME_LEN);
                    exit(1);
                }
                strncpy(username, optarg, MAX_USERNAME_LEN - 1);
                break;
            case 'p':
                if (strlen(optarg) >= MAX_PASSWORD_LEN) {
                    fprintf(stderr, "Error: Password is too long (maximum %d characters).\n",
                            MAX_PASSWORD_LEN);
                    exit(1);
                }
                strncpy(password, optarg, MAX_PASSWORD_LEN - 1);
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 'v':
                verbose = true;
                break;
            case 'V':
                version = true;
                break;
            default:
                break;
        }
    }

    if (version) {
        printf("%s\n", VERSION);
        exit(0);
    }

    if (username[0] == '\0') {
        prompt_for_input(username, sizeof(username), "Username: ", false);
        if (strlen(username) == 0) {
            fprintf(stderr, "Error: Username cannot be empty.\n");
            exit(1);
        }
    }

    if (password[0] == '\0') {
        prompt_for_input(password, sizeof(password), "Password: ", true);
        if (strlen(password) == 0) {
            fprintf(stderr, "Error: Password cannot be empty.\n");
            exit(1);
        }
    }

    // Retrieve user information from the username
    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        fprintf(stderr, "Error: User '%s' not found.\n", username);
        exit(1);
    }

    // Check if the user's UID is below the minimum allowed UID and not higher
    // than maximum allowed UID
    if (pwd->pw_uid < MIN_UID || pwd->pw_uid > MAX_UID) {
        fprintf(stderr, "Error: User '%s' has a UID higher than %d or lower than %d and is not allowed to authenticate.\n",
                username, MAX_UID, MIN_UID);
        exit(1);
    }

    if (verbose)
        printf("User '%s' passed UID check (UID: %d).\n", username,
               pwd->pw_uid);

    if (authenticate(username, password, verbose) == true) {
        printf("Authenticated successfully.\n");
        return 0;
    } else {
        printf("Authentication failed.\n");
        return 1;
    }
}

