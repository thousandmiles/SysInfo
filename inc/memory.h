#ifndef MEMORY_C
#define MEMORY_C

typedef struct
{
    unsigned int pid;
    unsigned long vm_peak;
    unsigned long vm_size;
    unsigned long vm_lck;
    unsigned long vm_pin;
    unsigned long vm_hwm;
    unsigned long vm_rss;
    unsigned long vm_data;
    unsigned long vm_stk;
    unsigned long vm_exe;
    unsigned long vm_lib;
    unsigned long vm_pte;
    unsigned long vm_swap;
} Process_Memory_Info;

typedef struct
{
    unsigned long total_memory;
    unsigned long free_memory;
    unsigned long available_memory;
    unsigned long buffers;
    unsigned long cached;
} Machine_Memory_Info;

void test_memory(void);

/*
 * @brief	       get process memory info
 * @param   pid    process id
 * @param   info   Process_Memory_Info ptr
 * @return 		   void
 */
void get_process_memory_info(unsigned int pid, Process_Memory_Info *info);

/*
 * @brief	       print process memory info
 * @param   info   Process_Memory_Info ptr
 * @return 		   void
 */
void print_process_memory_info(const Process_Memory_Info *info);

/*
 * @brief	       get machine memory info
 * @param   info   Machine_Memory_Info ptr
 * @return 		   void
 */
void get_machine_memory_info(Machine_Memory_Info *info);

/*
 * @brief	       print machine memory info
 * @param   info   Machine_Memory_Info ptr
 * @return 		   void
 */
void print_machine_memory_info(const Machine_Memory_Info *info);

/*
 * @brief	       calculate process memory usage
 * @param   pid    process id
 * @return 		   memory usage (%)
 */
float get_process_memory_usage(unsigned int pid);

#endif