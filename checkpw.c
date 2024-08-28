/**
 * checkpw is a program that checks the validity of a users password on a
 * Linux/PAM-based system.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2024
 * License: MIT (see LICENSE)
 */

#include <pwd.h>        // For struct passwd and getpwnam
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>    // For terminal input settings
#include <unistd.h>     // For getopt and access to user info

#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_LEN 256

#ifndef MAX_UID
#define MAX_UID 1000
#endif

#ifndef MIN_UID
#define MIN_UID 1000
#endif

#define VERSION 1_0_3

// Custom data structure to hold user-entered password
struct pam_credentials {
    const char *password;
};

// PAM conversation function to supply the password
int pam_conversation(int num_msg, const struct pam_message **msg,
                     struct pam_response **resp, void *appdata_ptr) {
    struct pam_response *response = NULL;
    struct pam_credentials *credentials = (struct pam_credentials *)appdata_ptr;
    int i;

    response = (struct pam_response *)malloc(sizeof(struct pam_response) * num_msg);
    if (response == NULL)
        return PAM_CONV_ERR;

    for (i = 0; i < num_msg; i++) {
        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_ON:
            case PAM_PROMPT_ECHO_OFF:
                response[i].resp = strdup(credentials->password);
                response[i].resp_retcode = 0;
                break;
            default:
                free(response);
                return PAM_CONV_ERR;
        }
    }

    *resp = response;
    return PAM_SUCCESS;
}

int authenticate(const char *username, const char *password, int verbose) {
    int retval;
    pam_handle_t *pamh = NULL;
    struct pam_credentials credentials = { password };
    struct pam_conv conv = { pam_conversation, &credentials };

    if (verbose)
        printf("Starting PAM authentication for user '%s'.\n", username);

    retval = pam_start("login", username, &conv, &pamh);

    if (retval == PAM_SUCCESS) {
        if (verbose)
            printf("PAM authentication initialized.\n");
        retval = pam_authenticate(pamh, 0); // Attempt to authenticate
    } else {
        if (verbose)
            printf("pam_start failed: %s\n", pam_strerror(pamh, retval));
    }

    if (retval == PAM_SUCCESS) {
        if (verbose)
            printf("User '%s' authenticated successfully.\n", username);

        retval = pam_acct_mgmt(pamh, 0); // Check account validity
        if (retval != PAM_SUCCESS && verbose)
            printf("pam_acct_mgmt failed: %s\n", pam_strerror(pamh, retval));

    } else {
        if (verbose)
            printf("pam_authenticate failed: %s\n", pam_strerror(pamh, retval));
    }

    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "Failed to release PAM authenticator\n");
        exit(1);
    }

    if (retval != PAM_SUCCESS && verbose) {
        printf("Authentication failed for user '%s'.\n", username);
    }

    return (retval == PAM_SUCCESS ? 0 : 1); // 0 for success, 1 for failure
}

// Function to prompt user for input, optionally hiding input
void prompt_for_input(char *buffer, size_t size, const char *prompt, int hide_input) {
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

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [-u <username>] [-p <password>] [-i] [-v] [-h]\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -u <username>  Specify username.\n");
    fprintf(stderr, "  -p <password>  Specify password.\n");
    fprintf(stderr, "  -i             Enable interactive mode to prompt for missing username/password.\n");
    fprintf(stderr, "  -v             Enable verbose mode.\n");
    fprintf(stderr, "  -V             Show version.\n");
    fprintf(stderr, "  -h             Show this help.\n");
}

int main(int argc, char *argv[]) {
    bool interactive = false;
    bool verbose = false;
    bool version = false;
    char username[MAX_USERNAME_LEN] = {0};
    char password[MAX_PASSWORD_LEN] = {0};
    int opt;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "u:p:hivV")) != -1) {
        switch (opt) {
            case 'u':
                if (strlen(optarg) >= MAX_USERNAME_LEN) {
                    fprintf(stderr, "Error: Username is too long (maximum %d characters).\n", MAX_USERNAME_LEN);
                    exit(1);
                }
                strncpy(username, optarg, MAX_USERNAME_LEN - 1);
                break;
            case 'p':
                if (strlen(optarg) >= MAX_PASSWORD_LEN) {
                    fprintf(stderr, "Error: Password is too long (maximum %d characters).\n", MAX_PASSWORD_LEN);
                    exit(1);
                }
                strncpy(password, optarg, MAX_PASSWORD_LEN - 1);
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case 'i':
                interactive = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'V':
                version = true;
                break;
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }

    if (version) {
        printf("1.0.3\n");
        exit(0);
    }

    // If interactive mode is enabled, prompt for missing username and/or password
    if (interactive) {
        if (username[0] == '\0') {
            prompt_for_input(username, sizeof(username), "Username: ", 0);
            if (strlen(username) == 0) {
                fprintf(stderr, "Error: Username cannot be empty.\n");
                exit(1);
            }
        }

        if (password[0] == '\0') {
            prompt_for_input(password, sizeof(password), "Password: ", 1);
            if (strlen(password) == 0) {
                fprintf(stderr, "Error: Password cannot be empty.\n");
                exit(1);
            }
        }
    } else {
        // If not in interactive mode, ensure username and password are provided
        if (username[0] == '\0' || password[0] == '\0') {
            fprintf(stderr, "Error: Username and password must be provided unless interactive mode is enabled.\n");
            print_usage(argv[0]);
            exit(1);
        }
    }

    // Retrieve user information from the username
    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        fprintf(stderr, "Error: User '%s' not found.\n", username);
        exit(1);
    }

    // Check if the user's UID is below the minimum allowed UID and not higher than maximum allowed UID
    if (pwd->pw_uid < MIN_UID || pwd->pw_uid > MAX_UID) {
        fprintf(stderr, "Error: User '%s' has a UID less than %d or higher than %d and is not allowed to authenticate.\n", username, MIN_UID, MAX_UID);
        exit(1);
    }

    if (verbose)
        printf("User '%s' passed UID check (UID: %d).\n", username, pwd->pw_uid);

    // Authenticate the user
    if (authenticate(username, password, verbose) == 0) {
        printf("Authenticated successfully.\n");
        return 0;
    } else {
        printf("Authentication failed.\n");
        return 1;
    }
}

