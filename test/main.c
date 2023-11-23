#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "process.h"
#include "stdio.h"

int main(void)
{
    test_cpu();
    test_disk();
    test_memory();
    test_process();

    return 0;
}