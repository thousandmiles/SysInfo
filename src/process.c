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

/*
 * @brief   	: create a process info node
 * @param       : void
 * @return 		: a node ptr
 */
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

/*
 * @brief   	: free the process info list
 * @param   ptr : the head node of linked list
 * @return 		: void
 */
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

/*
 * @brief   	: print the process info list to stdout
 * @param - ptr : the head node of linked list
 * @return 		: void
 */
void print_process_info_list(Ptr_Process_Info ptr)
{
    Ptr_Process_Info current = ptr;
    while (current != NULL)
    {
        printf("Name: %s, PID: %u, Status: %s\n", current->process_name, current->process_pid, current->process_status);
        current = current->next;
    }
}

/*
 * @brief	    : obtain process info
 * @param       : void
 * @return 		: error code, 0 represents success
 */
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

                            if (strstr(line, "Name") != NULL)
                            {
                                sscanf(line, "Name: %[^\n]", newProcess->process_name);
                            }
                            else if (strstr(line, "State") != NULL)
                            {
                                sscanf(line, "State: %[^\n]", newProcess->process_status);
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