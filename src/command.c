#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>
#include "httpserver.h"
#include <signal.h>

void show_help(void)
{
    printf("CrossNex command help:\n");
    printf("\n");
    printf("CrossNex run\t\t\t\t\t\t- run CrossNex\n");
    printf("CrossNex stop\t\t\t\t\t\t- stop CrossNex\n");
    printf("CrossNex add -u \"username\" -p \"password\"\t\t- add a authorized user\n");
    printf("CrossNex delete -u \"username\"\t\t\t\t- delete a authorized user\n");
    printf("CrossNex help\t\t\t\t\t\t- display help info\n");
    printf("CrossNex run -b\t\t\t\t\t\t- run the service in the background\n");
    printf("CrossNex run -d \"ip address\"\t\t\t\t- run the service in the debug mode, \n");
    printf("\t\t\t\t\t\t\t  allow this ip to access the original API\n");
    printf("\n");
    printf("For options -u, -p, -d, input parameter is required, such as username.\n");
    printf("\n");
    printf("For any questions, please feel free to contact me: \n");
    printf("@Github: \thttps://github.com/thousandmiles\n");
    printf("@e-mail: \tlongxy98@foxmail.com\n");
}

static struct MHD_Daemon *my_daemon = NULL;

int run_default(void)
{
    printf("default running...\n");
    return run();
}

int run(void)
{
    my_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                                 &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == my_daemon)
    {
        perror("my_daemon failed");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    while (1)
    {
    };

    return 0;
}

void stop_service(void)
{
    const char *process_name = "CrossNex";
    char command[512];

    snprintf(command, sizeof(command), "pkill %s", process_name);

    if (system(command) == -1)
    {
        printf("%s can't be stopped, please try again\n", process_name);
        exit(EXIT_FAILURE);
    }
}

void cleanup_and_exit()
{

    if (my_daemon != NULL)
    {
        printf("resource release...\n");
        MHD_stop_daemon(my_daemon);
    }

    printf("exit safely\n");
    exit(EXIT_SUCCESS);
}

void handle_signal(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
    {
        cleanup_and_exit();
    }
}

void run_background(void)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork background process failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
    {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") == -1)
    {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    printf("background running...\n");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    run();
}