#ifndef PROCESS_H
#define PROCESS_H
#include "stdio.h"

enum ATTRIBUTE
{
    PROCESS_NAME = 1,
    PROCESS_STATUS,
};

typedef struct PROCESS_INFO
{
    char process_name[128];
    unsigned int process_pid;
    char process_status[128];
    unsigned char process_cpu_usage;
    unsigned char process_disk_usage;
    unsigned char process_memory_usage;
    struct PROCESS_INFO *next;
} Process_Info;

typedef Process_Info *Ptr_Process_Info;

void test_process(void);

Ptr_Process_Info get_process_list(void);

Ptr_Process_Info create_process_info_node(void);

void free_process_info_list(Ptr_Process_Info ptr);

void print_process_info_list(Ptr_Process_Info ptr);

void get_single_attribute(FILE *f, unsigned char flag, char *result);

#endif