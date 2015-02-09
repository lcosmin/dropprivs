#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <pwd.h>


const char* user = NULL;
const char* group = NULL;
int command_index = 0;


int get_user_uid(const char* user, uid_t* uid)
{
    struct passwd* pwd = NULL;

    assert(user);
    assert(uid);

    while ((pwd = getpwent()) != NULL)
    {
        if (!strcmp(user, pwd->pw_name))
        {
            *uid = pwd->pw_uid;
            return 0;
        }
    }

    return -1;
}


void usage(const char* progname)
{
    printf("USAGE: %s [-u user] [-g group] -- command\n", progname);
    exit(0);
}


int main(int argc, char** argv)
{
    int c, i;
    uid_t uid = 0;

    while ((c = getopt(argc, argv, "u:g:h")) != -1)
    {
        switch (c)
        {
            case 'u':
                user = optarg;
                break;

            case 'g':
                group = optarg;
                break;

            case 'h':
            case '?':
            default:
                usage(argv[0]);
        }
    }

    argc -= optind;
    argv += optind;

    if (user)
    {
        if (get_user_uid(user, &uid) < 0)
        {
            printf("user not found\n");
        }
        else
        {
            printf("user: %s (uid: %u)\n", user, uid);
        }
    }

    printf("group: %s\n", group);

    for (i = 0; i < argc; i++)
    {
        printf("command: %s\n", argv[i]);
    }

    return 0;
}
