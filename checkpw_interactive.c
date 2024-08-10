#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int authenticate(const char *username, const char *password) {
    pam_handle_t *pamh = NULL;
    int retval;
    struct pam_credentials credentials = { password };
    struct pam_conv conv = { pam_conversation, &credentials };

    retval = pam_start("login", username, &conv, &pamh);

    if (retval == PAM_SUCCESS) {
        retval = pam_authenticate(pamh, 0); // Attempt to authenticate
    }

    if (retval == PAM_SUCCESS) {
        retval = pam_acct_mgmt(pamh, 0); // Check account validity
    }

    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "Failed to release PAM authenticator\n");
        exit(1);
    }

    return (retval == PAM_SUCCESS ? 0 : 1); // 0 for success, 1 for failure
}

int main() {
    char username[256];
    char *password;

    // Prompt user for a username
    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        fprintf(stderr, "Failed to read username\n");
        exit(1);
    }

    // Remove the trailing newline character if it exists
    username[strcspn(username, "\n")] = '\0';

    // Prompt user for a password
    password = getpass("Password: ");

    // Authenticate the user
    if (authenticate(username, password) == 0) {
        printf("Authenticated successfully.\n");
        return 0;
    } else {
        printf("Authentication failed.\n");
        return 1;
    }
}

