#ifndef CPU_H
#define CPU_H

typedef struct
{
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
} CPU_Stat;

typedef struct
{
    unsigned int pid;
    unsigned long utime;
    unsigned long stime;
} Process_Stat;

typedef struct
{
    char vendor_id[128];
    char model_name[256];
    double cpu_mhz;
    unsigned int cache_size; // kB
    unsigned char core_num;
} CPU_Info;

void test_cpu(void);

void read_cpu_stat(CPU_Stat *cpu_stat);

unsigned long calculate_total_cpu_time(const CPU_Stat *cpu_stat);

unsigned long get_total_cpu_time();

void read_process_stat(unsigned int pid, Process_Stat *process_stat);

unsigned long calculate_process_cpu_time(const Process_Stat *process_stat);

unsigned long get_total_process_time(unsigned int pid);

float get_process_cpu_usage(unsigned int pid);

void get_CPU_info(CPU_Info *cpu_info);

void print_CPU_Info(const CPU_Info *cpu_info);

#endif