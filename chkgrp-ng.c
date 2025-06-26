/**
 * chkgrp-ng is a program that checks if a user is a member of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkgrp
 * License: Apache-2.0 (see LICENSE)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 256

void print_help(const char *prog)
{
    printf("Usage: %s [OPTIONS] <username> <groupname>\n", prog);
    printf("Options:\n");
    printf("  --passwd PATH    Use custom /etc/passwd file\n");
    printf("  --group PATH     Use custom /etc/group file\n");
    printf("  -q               Quiet mode (no output)\n");
    printf("  -h               Show this help message\n");
}

const char *get_arg(int *i, int argc, char *argv[], const char *opt)
{
    if (*i + 1 >= argc) {
        fprintf(stderr, "Missing argument after %s\n", opt);
        exit(2);
    }
    return argv[++(*i)];
}

int get_user_gid(const char *user, const char *passwd_path)
{
    FILE *fp = fopen(passwd_path, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open passwd file: %s\n", passwd_path);
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        char *name = strtok(line, ":");
        if (!name || strcmp(name, user) != 0) continue;

        strtok(NULL, ":"); // password
        strtok(NULL, ":"); // UID
        char *gid_str = strtok(NULL, ":");
        fclose(fp);
        return gid_str ? atoi(gid_str) : -1;
    }

    fclose(fp);
    return -1;
}

int check_group(const char *user, const char *group, int user_gid,
                const char *group_path)
{
    FILE *fp = fopen(group_path, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open group file: %s\n", group_path);
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        char *grp = strtok(line, ":");
        if (!grp || strcmp(grp, group) != 0) continue;

        strtok(NULL, ":"); // password
        char *gid_str = strtok(NULL, ":");
        if (!gid_str) break;

        int group_gid = atoi(gid_str);
        if (group_gid == user_gid) {
            fclose(fp);
            return 1;
        }

        char *members = strtok(NULL, ":\n");
        if (!members) break;

        char *m = strtok(members, ",");
        while (m) {
            if (strcmp(m, user) == 0) {
                fclose(fp);
                return 1;
            }
            m = strtok(NULL, ",");
        }

        fclose(fp);
        return 0;
    }

    fclose(fp);
    return -1;
}

int main(int argc, char *argv[])
{
    const char *passwd_file = "/etc/passwd";
    const char *group_file  = "/etc/group";
    int quiet = 0;

    int i = 1;
    const char *username = NULL, *groupname = NULL;

    while (i < argc) {
        if (!strcmp(argv[i], "--passwd")) {
            passwd_file = get_arg(&i, argc, argv, "--passwd");
        } else if (!strcmp(argv[i], "--group")) {
            group_file = get_arg(&i, argc, argv, "--group");
        } else if (!strcmp(argv[i], "-q")) {
            quiet = 1;
        } else if (!strcmp(argv[i], "-h")) {
            print_help(argv[0]);
            return 0;
        } else if (!username) {
            username = argv[i];
        } else if (!groupname) {
            groupname = argv[i];
        } else {
            fprintf(stderr, "Unexpected argument: %s\n", argv[i]);
            return 2;
        }
        i++;
    }

    if (!username || !groupname ||
        strlen(username) > MAX_NAME || strlen(groupname) > MAX_NAME) {
        return 2;
    }

    int user_gid = get_user_gid(username, passwd_file);
    if (user_gid < 0)
        return 2;

    int result = check_group(username, groupname, user_gid, group_file);
    if (result == 1) {
        if (!quiet) puts("Yes");
        return 0;
    } else if (result == 0) {
        if (!quiet) puts("No");
        return 1;
    }

    return 2;
}

