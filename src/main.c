#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "config.h"

const char* user = NULL;
const char* cwd = NULL;
int verbose = 0;


int get_user_uid_gid(const char* user, uid_t* uid, gid_t* gid)
{
    struct passwd* pwd = NULL;

    assert(user);
    assert(uid);

    while ((pwd = getpwent()) != NULL)
    {
        if (!strcmp(user, pwd->pw_name))
        {
            *uid = pwd->pw_uid;
            *gid = pwd->pw_gid;
            return 0;
        }
    }

    return -1;
}


int set_user_privileges(const char* user, uid_t uid, gid_t gid)
{

  if (verbose)
      printf("Switching to user %s (uid %u, gid %u)\n", user, uid, gid);

#if HAVE_INITGROUPS
    if (initgroups(user, gid) != 0)
    {
        perror("initgroups");
        return -1;
    }
#endif

#if HAVE_SETRESGID
    if (setresgid(gid, gid, gid) < 0)
    {
        perror("setresgid");
        return -1;
    }
#elif HAVE_SETREGID
    if (setregid(gid, gid) < 0)
    {
        perror("setregid");
        return -1;
    }
#else
    if (setgid(gid) != 0)
    {
        perror("setgid");
        return -1;
    }
#endif


#if HAVE_SETRESUID
    if (setresuid(uid, uid, uid) < 0)
    {
        perror("setresuid");
        return -1;
    }
#elif HAVE_SETREUID
    if (setreuid(uid, uid) < 0)
    {
        perror("setreuid");
        return -1;
    }
#else
    if (setuid(uid) != 0)
    {
        perror("setuid");
        return -1;
    }
#endif

    /* check if we succeeded */
    if (getuid() != uid)
    {
        fprintf(stderr, "failed to set UID!\n");
        return -1;
    }

    if (getgid() != gid)
    {
        fprintf(stderr, "failed to set GID!\n");
        return -1;
    }

    return 0;
}

void usage()
{
    printf("USAGE: dropprivs [-u user] [-d directory] [-v] -- command\n");
    exit(0);
}


int main(int argc, char** argv)
{
    int c;
    uid_t uid;
    gid_t gid;

    while ((c = getopt(argc, argv, "u:d:hv")) != -1)
    {
        switch (c)
        {
            case 'u':
                user = optarg;
                break;

            case 'd':
                cwd = optarg;
                break;

            case 'v':
                verbose = 1;
                break;

            case 'h':
            case '?':
            default:
                usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (getuid() != 0)
    {
        fprintf(stderr, "This program needs to be run as root!\n");
        return 1;
    }

    if (!user)
    {
        fprintf(stderr, "No user name specified!\n");
        return 1;
    }

    if (!argc)
    {
        fprintf(stderr, "No command specified!\n");
        return 1;
    }

    if (get_user_uid_gid(user, &uid, &gid) < 0)
    {
        fprintf(stderr, "User %s not found\n", user);
        return 2;
    }

    if (set_user_privileges(user, uid, gid) < 0)
    {
        return 3;
    }

    if (cwd)
    {
        if (verbose)
            printf("Changing path to %s\n", cwd);

        if (chdir(cwd) < 0)
        {
            perror("chdir");
            return 4;
        }
    }

    if (execvp(argv[0], argv) < 0)
    {
        perror("execvp");
        return 4;
    }

    return 0;
}
