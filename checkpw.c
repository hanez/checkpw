/*
 * checkpw is a program that checks the validity of a users password on a
 * Linux/PAM-based system.
 *
 * Usage: checkpw -u $USER -p $PASSWORD
 *
 * Returns 0 on success, 1 otherwise.
 *
 * License: MIT (see LICENSE)
 */

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For getopt and access to user info
#include <pwd.h>     // For struct passwd and getpwuid

#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_LEN 256

#ifndef MIN_UID
#define MIN_UID 1000
#endif

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
    if (response == NULL) {
        return PAM_CONV_ERR;
    }

    for (i = 0; i < num_msg; i++) {
        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF || msg[i]->msg_style == PAM_PROMPT_ECHO_ON) {
            response[i].resp = strdup(credentials->password);
            response[i].resp_retcode = 0;
        }
    }

    *resp = response;
    return PAM_SUCCESS;
}

int authenticate(const char *username, const char *password, int verbose) {
    pam_handle_t *pamh = NULL;
    int retval;
    struct pam_credentials credentials = { password };
    struct pam_conv conv = { pam_conversation, &credentials };

    if (verbose) {
        printf("Starting PAM authentication for user '%s'.\n", username);
    }

    retval = pam_start("login", username, &conv, &pamh);

    if (retval == PAM_SUCCESS) {
        if (verbose) {
            printf("PAM authentication initialized.\n");
        }
        retval = pam_authenticate(pamh, 0); // Attempt to authenticate
    }

    if (retval == PAM_SUCCESS) {
        if (verbose) {
            printf("User '%s' authenticated successfully.\n", username);
        }
        retval = pam_acct_mgmt(pamh, 0); // Check account validity
    }

    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        if (verbose) {
            fprintf(stderr, "Failed to release PAM authenticator\n");
        }
        exit(1);
    }

    if (retval != PAM_SUCCESS && verbose) {
        printf("Authentication failed for user '%s'.\n", username);
    }

    return (retval == PAM_SUCCESS ? 0 : 1); // 0 for success, 1 for failure
}

int main(int argc, char *argv[]) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int verbose = 0;  // Verbose mode flag
    int opt;

    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "u:p:v")) != -1) {
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
            case 'v':
                verbose = 1;  // Enable verbose mode
                break;
            default:
                fprintf(stderr, "Usage: %s -u <username> -p <password> [-v]\n", argv[0]);
                exit(1);
        }
    }

    // Check if both username and password are provided
    if (username[0] == '\0' || password[0] == '\0') {
        if (verbose) {
            fprintf(stderr, "Usage: %s -u <username> -p <password> [-v]\n", argv[0]);
        }
        exit(1);
    }

    // Retrieve user information from the username
    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        if (verbose) {
            fprintf(stderr, "Error: User '%s' not found.\n", username);
        }
        exit(1);
    }

    // Check if the user's UID is below the minimum allowed UID
    if (pwd->pw_uid < MIN_UID) {
        if (verbose) {
            fprintf(stderr, "Error: User '%s' has a UID less than %d and is not allowed to authenticate.\n", username, MIN_UID);
        }
        exit(1);
    }

    if (verbose) {
        printf("User '%s' passed UID check (UID: %d).\n", username, pwd->pw_uid);
    }

    // Authenticate the user
    if (authenticate(username, password, verbose) == 0) {
        if (verbose) {
            printf("Authenticated successfully.\n");
        }
        return 0;
    } else {
        if (verbose) {
            printf("Authentication failed.\n");
        }
        return 1;
    }
}

