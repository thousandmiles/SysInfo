#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <stdio.h>
#include <microhttpd.h>

#define PORT 8888

enum HANDLER_CODE
{
    HANDLE_404 = 0,
    HANDLE_CPU_INFO,
    HANDLE_CPU_TOTAL_TIME,
    HANDLE_PROCESS_CPU_TOAL_TIME
};

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls);

/*
 * @brief parse the url, and let us know what to do.
 * @param url: connection url
 * @param connection: connection instance
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_GET_url(const char *url, struct MHD_Connection *connection);

/*
 * @brief response the 404 not found
 * @param connection: MHD_Connection
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_404_error(struct MHD_Connection *connection);

/*
 * @brief response the cpu info
 * @param connection: MHD_Connection
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_get_cpu_info(struct MHD_Connection *connection);

/*
 * @brief response the total cpu time
 * @param connection: MHD_Connection
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_get_cpu_total_time(struct MHD_Connection *connection);

/*
 * @brief response the [pid] process total cpu time
 * @param connection: MHD_Connection
 * @param pid: process id
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_get_process_cpu_total_time(unsigned int pid, struct MHD_Connection *connection);

/*
 * @brief response the process info
 * @param connection: MHD_Connection
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_get_process_info_list(struct MHD_Connection *connection);

/*
 * @brief response the disk info
 * @param connection: MHD_Connection
 * @return MHD_NO (0) on error (i.e. reply already sent), MHD_YES (1) on success or if message has been queued
 */
int handle_get_disk_info_list(struct MHD_Connection *connection);

#endif