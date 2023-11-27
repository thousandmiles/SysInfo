#ifndef CPU_H
#define CPU_H

typedef struct
{
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
} CPU_Stat;

typedef struct
{
    unsigned int pid;
    unsigned long utime;
    unsigned long stime;
} Process_Stat;

typedef struct
{
    char vendor_id[128];
    char model_name[256];
    double cpu_mhz;
    unsigned int cache_size; // kB
    unsigned char core_num;
} CPU_Info;

void test_cpu(void);

/*
 * @brief	            obtain cpu stat info
 * @param   cpu_stat    CPU_Stat ptr
 * @return 		        void
 */
void read_cpu_stat(CPU_Stat *cpu_stat);

/*
 * @brief	         calculate total cpu time
 * @param   cpu_stat CPU_Stat ptr
 * @return 		     total cpu time
 */
unsigned long calculate_total_cpu_time(const CPU_Stat *cpu_stat);

/*
 * @brief	        get total cpu time
 * @param           void
 * @return 		    total cpu time
 */
unsigned long get_total_cpu_time();

/*
 * @brief	                 read process stat file
 * @param     pid            process pid number
 * @param     process_stat   Process_Stat ptr
 * @return 		             total cpu time
 */
void read_process_stat(unsigned int pid, Process_Stat *process_stat);

/*
 * @brief	             calculate total cpu time
 * @param   process_stat Process_Stat ptr
 * @return 		         total cpu time of process
 */
unsigned long calculate_process_cpu_time(const Process_Stat *process_stat);

/*
 * @brief	         get total process time
 * @param    pid     process pid number
 * @return 		     total cpu time of process
 */
unsigned long get_total_process_time(unsigned int pid);

/*
 * @brief	        get process cpu usage
 * @param    pid    process pid number
 * @return 		    process cpu usage
 */
float get_process_cpu_usage(unsigned int pid);

/*
 * @brief	         get cpu info
 * @param   cpu_info CPU_Info ptr
 * @return 		     void
 */
void get_CPU_info(CPU_Info *cpu_info);

/*
 * @brief	         print cpu info
 * @param   cpu_info CPU_Info ptr
 * @return 		     void
 */
void print_CPU_Info(const CPU_Info *cpu_info);

#endif