#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "process.h"
#include "stdio.h"
#include "httpserver.h"
#include <microhttpd.h>

int main(void)
{
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

    return 0;
}