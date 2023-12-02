#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "process.h"
#include "stdio.h"
#include "httpserver.h"
#include <unistd.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int optind, opterr, optopt;
extern char *optarg;

int main(int argc, char *argv[])
{

    int opt;
    char *username = NULL;
    char *password = NULL;
    char *ip_address = NULL;
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
            printf("background running... \n");
        }
        else if (run_in_background && debug_mode)
        {
            printf("background debug running: %s\n", ip_address);
        }
        else if (!run_in_background && debug_mode)
        {
            printf("debug running...\n");
        }
        else
        {
            printf("default running...\n");
        }
    }
    else if (strcmp(action, "stop") == 0)
    {
        printf("stop running...\n");
    }
    else if (strcmp(action, "add") == 0)
    {
        if (username != NULL && password != NULL)
        {
            printf("add users: %s, password: %s\n", username, password);
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
            printf("delete user: %s\n", username);
        }
        else
        {
            fprintf(stderr, "user name is empty!\n");
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(action, "help") == 0)
    {
        printf("help page ...\n");
    }
    else
    {
        fprintf(stderr, "undefined paramaters: %s\n", action);
        return EXIT_FAILURE;
    }
    return 0;

    // test_process();

    // printf("-------------------------------------------\n");
    // test_cpu();

    // printf("-------------------------------------------\n");
    // test_memory();

    // printf("-------------------------------------------\n");
    // test_disk();

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon)
    {
        perror("daemon failed!");
        return 1;
    }

    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}