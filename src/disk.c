#include "disk.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

void test_disk(void)
{
    printf("Hello disk\r\n");
    // Disk_Info *head = get_disk_list();
    // print_disk_info_list(head);
    char *res = get_disk_json_list();
    printf("%s", res);
    free(res);
}

Ptr_Disk_Info create_disk_node(void)
{
    Disk_Info *disk_info = (Disk_Info *)malloc(sizeof(Disk_Info));
    if (disk_info != NULL)
    {
        strcpy(disk_info->filesystem, "");
        strcpy(disk_info->size, "");
        strcpy(disk_info->used, "");
        strcpy(disk_info->avail, "");
        strcpy(disk_info->use_percent, "");
        strcpy(disk_info->mounted_on, "");
        disk_info->next = NULL;
    }
    return disk_info;
}

void free_disk_node(Disk_Info *head)
{
    Disk_Info *current = head;
    while (current != NULL)
    {
        Disk_Info *temp = current;
        current = current->next;

        free(temp);
        temp = NULL;
    }
}

Ptr_Disk_Info get_disk_list(void)
{
    char line[512];
    Disk_Info *head = NULL;

    const char *command = "df -h";
    FILE *output = popen(command, "r");

    if (output == NULL)
    {
        perror("Error executing command");
        return NULL;
    }

    while (fgets(line, sizeof(line), output) != NULL)
    {
        Disk_Info *disk_info = create_disk_node();
        if (disk_info != NULL)
        {
            sscanf(line, "%s %s %s %s %s %s",
                   disk_info->filesystem,
                   disk_info->size,
                   disk_info->used,
                   disk_info->avail,
                   disk_info->use_percent,
                   disk_info->mounted_on);

            disk_info->next = head;
            head = disk_info;
        }
    }

    pclose(output);
    return head;
}

void print_disk_info_list(Disk_Info *head)
{
    Disk_Info *current = head;
    while (current != NULL)
    {
        printf("Filesystem: %s\n", current->filesystem);
        printf("Size: %s\n", current->size);
        printf("Used: %s\n", current->used);
        printf("Avail: %s\n", current->avail);
        printf("Use Percent: %s\n", current->use_percent);
        printf("Mounted On: %s\n\n", current->mounted_on);

        current = current->next;
    }
}

char *get_disk_json_list(void)
{
    char line[512];
    const char *command = "df -h";
    FILE *output = popen(command, "r");

    if (output == NULL)
    {
        perror("Error executing command");
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON *body = cJSON_CreateArray();

    if (root == NULL || body == NULL)
    {
        return NULL;
    }

    cJSON_AddItemToObject(root, "disk", body);

    Disk_Info disk_info;
    fgets(line, sizeof(line), output); // skip caption row

    while (fgets(line, sizeof(line), output) != NULL)
    {
        memset(&disk_info, 0, sizeof(disk_info));

        sscanf(line, "%s %s %s %s %s %s",
               disk_info.filesystem,
               disk_info.size,
               disk_info.used,
               disk_info.avail,
               disk_info.use_percent,
               disk_info.mounted_on);

        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "filesystem", disk_info.filesystem);
        cJSON_AddStringToObject(item, "size", disk_info.size);
        cJSON_AddStringToObject(item, "used", disk_info.used);
        cJSON_AddStringToObject(item, "avail", disk_info.avail);
        cJSON_AddStringToObject(item, "use_percent", disk_info.use_percent);
        cJSON_AddStringToObject(item, "mounted_on", disk_info.mounted_on);

        cJSON_AddItemToArray(body, item);
    }

    pclose(output);

    char *out = cJSON_PrintUnformatted(root);

    if (root)
    {
        cJSON_Delete(root);
    }

    return out;
}