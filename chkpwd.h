/**
 * chkpwd.h is part of chkpwd, a program that checks the validity of a users
 * password on a UNIX/PAM-based system.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * License: Apache-2.0 (see LICENSE)
 */

#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdbool.h>

#define VERSION "1.3.0"

bool checkpw_authenticate(const char *username, const char *password,
                          bool verbose);

int pam_conversation(int num_msg, const struct pam_message **msg,
                     struct pam_response **resp, void *appdata_ptr);

struct pam_credentials
{
    const char *password;
};

bool checkpw_authenticate(const char *username, const char *password,
                          bool verbose)
{
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
            fprintf(stderr, "Error: pam_start failed: %s\n", pam_strerror(pamh,
                    retval));
    }

    if (retval == PAM_SUCCESS) {
        if (verbose)
            printf("User '%s' authenticated successfully.\n", username);

        retval = pam_acct_mgmt(pamh, 0); // Check account validity
        if (retval != PAM_SUCCESS && verbose)
            fprintf(stderr, "Error: pam_acct_mgmt failed: %s\n",
                    pam_strerror(pamh, retval));
    } else {
        if (verbose)
            fprintf(stderr, "Error: pam_authenticate failed: %s\n",
                    pam_strerror(pamh, retval));
    }

    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "Error: Failed to release PAM authenticator.\n");
        exit(1);
    }

    if (retval != PAM_SUCCESS && verbose) {
        fprintf(stderr, "Error: Authentication failed for user '%s'.\n",
                username);
    }

    return (retval == PAM_SUCCESS ? true : false);
}

// PAM conversation function to supply the password
int pam_conversation(int num_msg, const struct pam_message **msg,
                     struct pam_response **resp, void *appdata_ptr)
{
    struct pam_response *response = NULL;
    struct pam_credentials *credentials = (struct pam_credentials *)appdata_ptr;
    int i;

    response = (struct pam_response *)malloc(sizeof(struct pam_response)
                * num_msg);

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

