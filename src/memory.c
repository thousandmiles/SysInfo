#include "memory.h"
#include "stdio.h"
#include "cJSON.h"
#include <stdlib.h>

void test_memory(void)
{
    printf("Hello memory\r\n");
    unsigned int pid = 150893;
    char *res = get_process_memory_json(pid);
    printf("%s\n", res);
    free(res);

    Machine_Memory_Info minfo;
    get_machine_memory_info(&minfo);
    print_machine_memory_info(&minfo);

    printf("Process %u memory usage: %f \n", pid, get_process_memory_usage(pid));
}

void get_process_memory_info(unsigned int pid, Process_Memory_Info *info)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "/proc/%u/status", pid);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening status file");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "VmPeak: %lu kB", &info->vm_peak);
        sscanf(line, "VmSize: %lu kB", &info->vm_size);
        sscanf(line, "VmLck: %lu kB", &info->vm_lck);
        sscanf(line, "VmPin: %lu kB", &info->vm_pin);
        sscanf(line, "VmHWM: %lu kB", &info->vm_hwm);
        sscanf(line, "VmRSS: %lu kB", &info->vm_rss);
        sscanf(line, "VmData: %lu kB", &info->vm_data);
        sscanf(line, "VmStk: %lu kB", &info->vm_stk);
        sscanf(line, "VmExe: %lu kB", &info->vm_exe);
        sscanf(line, "VmLib: %lu kB", &info->vm_lib);
        sscanf(line, "VmPTE: %lu kB", &info->vm_pte);
        sscanf(line, "VmSwap: %lu kB", &info->vm_swap);
    }

    fclose(file);
    info->pid = pid;

    return;
}

char *get_process_memory_json(unsigned int pid)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        return NULL;
    }

    Process_Memory_Info info;
    get_process_memory_info(pid, &info);

    cJSON_AddNumberToObject(root, "vm_peak", info.vm_peak);
    cJSON_AddNumberToObject(root, "vm_size", info.vm_size);
    cJSON_AddNumberToObject(root, "vm_lck", info.vm_lck);
    cJSON_AddNumberToObject(root, "vm_pin", info.vm_pin);
    cJSON_AddNumberToObject(root, "vm_hwm", info.vm_hwm);
    cJSON_AddNumberToObject(root, "vm_rss", info.vm_rss);
    cJSON_AddNumberToObject(root, "vm_data", info.vm_data);
    cJSON_AddNumberToObject(root, "vm_stk", info.vm_stk);
    cJSON_AddNumberToObject(root, "vm_exe", info.vm_exe);
    cJSON_AddNumberToObject(root, "vm_lib", info.vm_lib);
    cJSON_AddNumberToObject(root, "vm_pte", info.vm_pte);
    cJSON_AddNumberToObject(root, "vm_swap", info.vm_swap);

    char *out = cJSON_PrintUnformatted(root);

    if (root)
    {
        cJSON_Delete(root);
    }

    return out;
}

void print_process_memory_info(const Process_Memory_Info *info)
{
    printf("Process %u memory information:\n", info->pid);
    printf("VmPeak: %lu kB\n", info->vm_peak);
    printf("VmSize: %lu kB\n", info->vm_size);
    printf("VmLck: %lu kB\n", info->vm_lck);
    printf("VmPin: %lu kB\n", info->vm_pin);
    printf("VmHWM: %lu kB\n", info->vm_hwm);
    printf("VmRSS: %lu kB\n", info->vm_rss);
    printf("VmData: %lu kB\n", info->vm_data);
    printf("VmStk: %lu kB\n", info->vm_stk);
    printf("VmExe: %lu kB\n", info->vm_exe);
    printf("VmLib: %lu kB\n", info->vm_lib);
    printf("VmPTE: %lu kB\n", info->vm_pte);
    printf("VmSwap: %lu kB\n", info->vm_swap);
}

void get_machine_memory_info(Machine_Memory_Info *info)
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL)
    {
        perror("Error opening meminfo file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "MemTotal: %lu kB", &info->total_memory);
        sscanf(line, "MemFree: %lu kB", &info->free_memory);
        sscanf(line, "MemAvailable: %lu kB", &info->available_memory);
        sscanf(line, "Buffers: %lu kB", &info->buffers);
        sscanf(line, "Cached: %lu kB", &info->cached);
    }

    fclose(file);
}

char *get_machine_memory_json(void)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        return NULL;
    }

    Machine_Memory_Info minfo;
    get_machine_memory_info(&minfo);

    cJSON_AddNumberToObject(root, "total_memory", minfo.total_memory);
    cJSON_AddNumberToObject(root, "free_memory", minfo.free_memory);
    cJSON_AddNumberToObject(root, "available_memory", minfo.available_memory);
    cJSON_AddNumberToObject(root, "buffers", minfo.buffers);
    cJSON_AddNumberToObject(root, "cached", minfo.cached);

    char *out = cJSON_PrintUnformatted(root);

    if (root)
    {
        cJSON_Delete(root);
    }

    return out;
}

void print_machine_memory_info(const Machine_Memory_Info *info)
{
    printf("Total Memory: %lu kB\n", info->total_memory);
    printf("Free Memory: %lu kB\n", info->free_memory);
    printf("Available Memory: %lu kB\n", info->available_memory);
    printf("Buffers: %lu kB\n", info->buffers);
    printf("Cached: %lu kB\n", info->cached);
}

float get_process_memory_usage(unsigned int pid)
{
    float usage = 0.0;

    char filename[256];
    sprintf(filename, "/proc/%u/status", pid);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening process status file");
        return usage;
    }

    char line[256];
    unsigned int vm_rss = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "VmRSS: %u kB", &vm_rss) == 1)
        {
            break;
        }
    }

    fclose(file);

    file = fopen("/proc/meminfo", "r");
    if (file == NULL)
    {
        perror("Error opening meminfo file");
        return usage;
    }

    unsigned long total_memory = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "MemTotal: %lu kB", &total_memory) == 1)
        {
            break;
        }
    }

    fclose(file);

    if (total_memory > 0)
    {
        usage = ((float)vm_rss / total_memory) * 100.0;
    }

    return usage;
}