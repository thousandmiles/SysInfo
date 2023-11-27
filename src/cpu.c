#include "cpu.h"
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void test_cpu(void)
{
    printf("Hello cpu\r\n");

    // printf("Total CPU Time: %lu\n", get_total_cpu_time());

    // printf("Process cpu time: %lu\n", get_total_process_time(49730));

    // printf("Process cpu usage: %.2f\n", get_process_cpu_usage(49730));

    CPU_Info info;
    get_CPU_info(&info);
    print_CPU_Info(&info);
}

void read_cpu_stat(CPU_Stat *cpu_stat)
{
    FILE *stat_file = fopen("/proc/stat", "r");
    if (stat_file == NULL)
    {
        perror("Error opening /proc/stat");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), stat_file);

    sscanf(line, "cpu %lu %lu %lu %lu",
           &(cpu_stat->user), &(cpu_stat->nice),
           &(cpu_stat->system), &(cpu_stat->idle));

    fclose(stat_file);
    // printf("user: %lu, nice: %lu, system: %lu, idle: %lu \n", (cpu_stat->user), (cpu_stat->nice), (cpu_stat->system), (cpu_stat->idle));
}

unsigned long calculate_total_cpu_time(const CPU_Stat *cpu_stat)
{
    return cpu_stat->user + cpu_stat->nice + cpu_stat->system + cpu_stat->idle;
}

unsigned long get_total_cpu_time()
{
    CPU_Stat cpu_stat;
    read_cpu_stat(&cpu_stat);
    return calculate_total_cpu_time(&cpu_stat);
}

void read_process_stat(unsigned int pid, Process_Stat *process_stat)
{
    char stat_path[256];
    sprintf(stat_path, "/proc/%u/stat", pid);
    FILE *stat_file = fopen(stat_path, "r");
    if (stat_file == NULL)
    {
        perror("Error opening /proc/[pid]/stat");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), stat_file);

    sscanf(line, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu",
           &(process_stat->utime), &(process_stat->stime));

    process_stat->pid = pid;
    fclose(stat_file);
    // printf("utime: %lu, stime: %lu \n", process_stat->utime, process_stat->stime);
}

unsigned long calculate_process_cpu_time(const Process_Stat *process_stat)
{
    return process_stat->utime + process_stat->stime;
}

unsigned long get_total_process_time(unsigned int pid)
{
    Process_Stat process_stat;
    read_process_stat(pid, &process_stat);
    return calculate_process_cpu_time(&process_stat);
}

float get_process_cpu_usage(unsigned int pid)
{
    unsigned long total_cpu_time_1 = get_total_cpu_time();
    unsigned long total_process_time_1 = get_total_process_time(pid);

    sleep(1);

    unsigned long total_cpu_time_2 = get_total_cpu_time();
    unsigned long total_process_time_2 = get_total_process_time(pid);

    if (total_cpu_time_2 == total_cpu_time_1)
    {
        return 0.0;
    }

    return ((double)(total_process_time_2 - total_process_time_1) / (double)(total_cpu_time_2 - total_cpu_time_1)) * 100.0;
}

void get_CPU_info(CPU_Info *cpu_info)
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL)
    {
        perror("Error opening /proc/cpuinfo");
        return;
    }

    char line[256];

    unsigned char process_num = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {

        sscanf(line, "processor : %hhu", &process_num);

        if (process_num == 1)
        {
            break; // every core is same, so we do't need others
        }

        sscanf(line, "vendor_id : %[^\n]", cpu_info->vendor_id);
        sscanf(line, "model name : %[^\n]", cpu_info->model_name);
        sscanf(line, "cpu MHz : %lf", &cpu_info->cpu_mhz);
        sscanf(line, "cache size : %u kB", &cpu_info->cache_size);
    }

    cpu_info->core_num = sysconf(_SC_NPROCESSORS_ONLN);
}

void print_CPU_Info(const CPU_Info *cpu_info)
{
    printf("%s\n", cpu_info->vendor_id);
    printf("%s\n", cpu_info->model_name);
    printf("%lf\n", cpu_info->cpu_mhz);
    printf("%u\n", cpu_info->cache_size);
    printf("%u\n", cpu_info->core_num);
}