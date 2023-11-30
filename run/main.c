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

    while ((opt = getopt(argc, argv, "abc:")) != -1)
    {
        switch (opt)
        {
        case 'a':
            printf("do a...\n");
            break;
        case 'b':
            printf("do b...\n");
            break;
        case 'c':
            printf("do c...\n");
            break;
        default:
            printf("do default...\n");
            break;
        }
    }

    char *action;
    for (int i = optind; i < argc; i++)
    {
        action = argv[i];
        if (strcmp(action, "run") == 0)
        {
            printf("do run...\n");
        }
        else if (strcmp(action, "stop") == 0)
        {
            printf("do stop...\n");
        }
        else
        {
            fprintf(stderr, "unknown cmd: %s\n", action);
            return EXIT_FAILURE;
        }
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