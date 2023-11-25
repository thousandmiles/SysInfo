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

/*
 * @brief	        : obtain cpu stat info
 * @param   cpu_stat: CPU_Stat ptr
 * @return 		    : void
 */
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

/*
 * @brief	        : calculate total cpu time
 * @param   cpu_stat: CPU_Stat ptr
 * @return 		    : total cpu time
 */
unsigned long calculate_total_cpu_time(const CPU_Stat *cpu_stat)
{
    return cpu_stat->user + cpu_stat->nice + cpu_stat->system + cpu_stat->idle;
}

/*
 * @brief	        : get total cpu time
 * @param           : void
 * @return 		    : total cpu time
 */
unsigned long get_total_cpu_time()
{
    CPU_Stat cpu_stat;
    read_cpu_stat(&cpu_stat);
    return calculate_total_cpu_time(&cpu_stat);
}

/*
 * @brief	                : read process stat file
 * @param     pid           : process pid number
 * @param     process_stat  : Process_Stat ptr
 * @return 		            : total cpu time
 */
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

    fclose(stat_file);
    // printf("utime: %lu, stime: %lu \n", process_stat->utime, process_stat->stime);
}

/*
 * @brief	            : calculate total cpu time
 * @param   process_stat: Process_Stat ptr
 * @return 		        : total cpu time of process
 */
unsigned long calculate_process_cpu_time(const Process_Stat *process_stat)
{
    return process_stat->utime + process_stat->stime;
}

/*
 * @brief	        : get total process time
 * @param    pid    : process pid number
 * @return 		    : total cpu time of process
 */
unsigned long get_total_process_time(unsigned int pid)
{
    Process_Stat process_stat;
    read_process_stat(pid, &process_stat);
    return calculate_process_cpu_time(&process_stat);
}

/*
 * @brief	        : get process cpu usage
 * @param    pid    : process pid number
 * @return 		    : process cpu usage
 */
double get_process_cpu_usage(unsigned int pid)
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

/*
 * @brief	        : get cpu info
 * @param   cpu_info: CPU_Info ptr
 * @return 		    : void
 */
void get_CPU_info(Ptr_CPU_Info cpu_info)
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL)
    {
        perror("Error opening /proc/cpuinfo");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {

        if (strstr(line, "vendor_id") != NULL)
        {
            sscanf(line, "vendor_id : %[^\n]", cpu_info->vendor_id);
        }
        else if (strstr(line, "model name") != NULL)
        {
            sscanf(line, "model name : %[^\n]", cpu_info->model_name);
        }
        else if (strstr(line, "cpu MHz") != NULL)
        {
            sscanf(line, "cpu MHz : %lf", &cpu_info->cpu_mhz);
        }
        else if (strstr(line, "cache size") != NULL)
        {
            sscanf(line, "cache size : %[^\n]", cpu_info->cache_size);
            break;
        }
    }

    cpu_info->core_num = sysconf(_SC_NPROCESSORS_ONLN);
}

/*
 * @brief	        : print cpu info
 * @param   cpu_info: CPU_Info ptr
 * @return 		    : void
 */
void print_CPU_Info(Ptr_CPU_Info cpu_info)
{
    printf("%s\n", cpu_info->vendor_id);
    printf("%s\n", cpu_info->model_name);
    printf("%lf\n", cpu_info->cpu_mhz);
    printf("%s\n", cpu_info->cache_size);
    printf("%u\n", cpu_info->core_num);
}