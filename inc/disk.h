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

/*
 * @brief   	 create a disk info node
 * @param        void
 * @return 		 a node ptr
 */
Ptr_Disk_Info create_disk_node(void);

/*
 * @brief   	 free the disk info list
 * @param  head  the head node of linked list
 * @return 		 void
 */
void free_disk_node(Disk_Info *head);

/*
 * @brief	     obtain disk info
 * @param        void
 * @return 		 the head node of disk info list
 */
Ptr_Disk_Info get_disk_list(void);

/*
 * @brief   	  print the disk info list to stdout
 * @param - head  the head node of linked list
 * @return 		  void
 */
void print_disk_info_list(Disk_Info *head);

#endif