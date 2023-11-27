#include "process.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "cpu.h"

void test_process(void)
{
    printf("Hello process\r\n");
    Ptr_Process_Info ptr = get_process_list();
    print_process_info_list(ptr);
    free_process_info_list(ptr);
}

Ptr_Process_Info create_process_info_node()
{
    Ptr_Process_Info new_node = (Ptr_Process_Info)malloc(sizeof(Process_Info));
    if (new_node != NULL)
    {
        new_node->next = NULL;
        memset(new_node, 0, sizeof(Process_Info));

        strcpy(new_node->process_name, "");
        new_node->process_pid = 0;
        strcpy(new_node->process_status, "");
        new_node->next = NULL;
    }
    return new_node;
}

void free_process_info_list(Ptr_Process_Info ptr)
{
    while (ptr != NULL)
    {
        Ptr_Process_Info temp = ptr;
        ptr = ptr->next;

        free(temp);
        temp = NULL;
    }
}

void print_process_info_list(Ptr_Process_Info ptr)
{
    Ptr_Process_Info current = ptr;
    while (current != NULL)
    {
        printf("Name: %s, PID: %u, Status: %s\n", current->process_name, current->process_pid, current->process_status);
        current = current->next;
    }
}

Ptr_Process_Info get_process_list()
{
    Ptr_Process_Info head = NULL;
    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            // Check if the entry is a directory
            struct stat statbuf;
            char path[512];
            snprintf(path, sizeof(path), "/proc/%s", entry->d_name);

            if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                // Check if the entry is a directory (process directories in /proc are named with numbers)
                unsigned int pid = atoi(entry->d_name);
                if (pid > 0)
                {
                    char filename[256];
                    snprintf(filename, sizeof(filename), "/proc/%u/status", pid);

                    FILE *file = fopen(filename, "r");

                    if (file != NULL)
                    {
                        Ptr_Process_Info newProcess = create_process_info_node();
                        newProcess->process_pid = pid;

                        char line[256];

                        while (fgets(line, sizeof(line), file) != NULL)
                        {
                            sscanf(line, "Name: %[^\n]", newProcess->process_name);
                            sscanf(line, "State: %[^\n]", newProcess->process_status);

                            if (strlen(newProcess->process_name) > 0 && strlen(newProcess->process_status) > 0)
                            {
                                break;
                            }
                        }

                        fclose(file);

                        newProcess->next = head;
                        head = newProcess;
                    }
                }
            }
        }
        closedir(dir);
    }
    return head;
}

char *get_process_json_list(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *body = cJSON_CreateArray();

    if (root == NULL || body == NULL)
    {
        return NULL;
    }

    cJSON_AddItemToObject(root, "process", body);

    DIR *dir = opendir("/proc");
    struct dirent *entry;

    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            // Check if the entry is a directory
            struct stat statbuf;
            char path[512];
            snprintf(path, sizeof(path), "/proc/%s", entry->d_name);

            if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                // Check if the entry is a directory (process directories in /proc are named with numbers)
                unsigned int pid = atoi(entry->d_name);
                if (pid > 0)
                {
                    char filename[256];
                    snprintf(filename, sizeof(filename), "/proc/%u/status", pid);

                    FILE *file = fopen(filename, "r");

                    if (file != NULL)
                    {
                        char process_name[128];
                        char process_status[128];
                        char line[256];

                        memset(process_name, 0, sizeof(process_name));
                        memset(process_status, 0, sizeof(process_status));

                        while (fgets(line, sizeof(line), file) != NULL)
                        {
                            sscanf(line, "Name: %[^\n]", process_name);
                            sscanf(line, "State: %[^\n]", process_status);

                            if (strlen(process_name) > 0 && strlen(process_status) > 0)
                            {
                                break;
                            }
                        }

                        cJSON *item = cJSON_CreateObject();
                        cJSON_AddNumberToObject(item, "pid", pid);
                        cJSON_AddStringToObject(item, "name", process_name);
                        cJSON_AddStringToObject(item, "state", process_status);
                        cJSON_AddItemToArray(body, item);

                        fclose(file);
                    }
                }
            }
        }
        closedir(dir);
    }

    char *out = cJSON_PrintUnformatted(root);

    if (root)
    {
        cJSON_Delete(root);
    }

    return out;
}