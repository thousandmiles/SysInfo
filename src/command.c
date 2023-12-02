#include "command.h"
#include <stdio.h>
#include <microhttpd.h>
#include "httpserver.h"

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

int run_default(void)
{
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