#ifndef PROCESS_H
#define PROCESS_H
#include "stdio.h"
#include "cJSON.h"

typedef struct PROCESS_INFO
{
    char process_name[128];
    unsigned int process_pid;
    char process_status[128];
    struct PROCESS_INFO *next;
} Process_Info;

typedef Process_Info *Ptr_Process_Info;

void test_process(void);

/*
 * @brief	     obtain process info
 * @param        void
 * @return 		 the head node of process info list
 */
Ptr_Process_Info get_process_list(void);

/*
 * @brief   	 create a process info node
 * @param        void
 * @return 		 a node ptr
 */
Ptr_Process_Info create_process_info_node(void);

/*
 * @brief   	 free the process info list
 * @param   ptr  the head node of linked list
 * @return 		 void
 */
void free_process_info_list(Ptr_Process_Info ptr);

/*
 * @brief   	 print the process info list to stdout
 * @param - ptr  the head node of linked list
 * @return 		 void
 */
void print_process_info_list(Ptr_Process_Info ptr);

/*
 * @brief	     obtain process info list
 * @param        void
 * @return 		 the json string of process info list
 * @note         [!!!] please clear the memory of the returned string after calling this function.
 */
char *get_process_json_list(void);

#endif