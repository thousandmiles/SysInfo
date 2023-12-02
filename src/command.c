#include "command.h"
#include <stdio.h>

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