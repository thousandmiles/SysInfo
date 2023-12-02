#include "stdio.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "user.h"

extern int optind, opterr, optopt;
extern char *optarg;

char *ip_address = NULL;

int main(int argc, char *argv[])
{

    int opt;
    char *username = NULL;
    char *password = NULL;

    int run_in_background = 0;
    int debug_mode = 0;

    while ((opt = getopt(argc, argv, "u:p:d:b")) != -1)
    {
        switch (opt)
        {
        case 'u':
            username = optarg;
            break;
        case 'p':
            password = optarg;
            break;
        case 'd':
            debug_mode = 1;
            ip_address = optarg;
            break;
        case 'b':
            run_in_background = 1;
            break;
        default:
            break;
        }
    }

    if (optind >= argc)
    {
        fprintf(stderr, "please input parameters, see help by command: 'CrossNex help'\n");
        return EXIT_FAILURE;
    }

    char *action = argv[optind];

    if (strcmp(action, "run") == 0)
    {
        if (run_in_background && !debug_mode)
        {
            run_background();
        }
        else if (run_in_background && debug_mode)
        {
            printf("background debug running: %s\n", ip_address);
        }
        else if (!run_in_background && debug_mode)
        {
            run();
        }
        else
        {
            run_default();
        }
    }
    else if (strcmp(action, "stop") == 0)
    {
        stop_service();
    }
    else if (strcmp(action, "add") == 0)
    {
        if (username != NULL && password != NULL)
        {
            add_user(username, password);
        }
        else
        {
            fprintf(stderr, "username or password is empty!\n");
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(action, "delete") == 0)
    {
        if (username != NULL)
        {
            delete_user(username);
        }
        else
        {
            fprintf(stderr, "user name is empty!\n");
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(action, "help") == 0)
    {
        show_help();
    }
    else
    {
        fprintf(stderr, "undefined paramaters: %s\n", action);
        return EXIT_FAILURE;
    }
    return 0;
}