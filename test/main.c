#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "process.h"
#include "stdio.h"

int main(void)
{
    test_process();

    printf("-------------------------------------------\n");
    test_cpu();

    printf("-------------------------------------------\n");
    test_memory();

    printf("-------------------------------------------\n");
    test_disk();

    return 0;
}