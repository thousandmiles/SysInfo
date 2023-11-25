#ifndef DISK_H
#define DISK_H

typedef struct DISK_INFO
{
    char filesystem[256];
    char size[10];
    char used[10];
    char avail[10];
    char use_percent[5];
    char mounted_on[256];
    struct DISK_INFO *next;
} Disk_Info;

typedef Disk_Info *Ptr_Disk_Info;

void test_disk(void);

Ptr_Disk_Info create_disk_node(void);

void free_disk_node(Disk_Info *head);

Ptr_Disk_Info get_disk_list(void);

void print_disk_info_list(Disk_Info *head);

#endif