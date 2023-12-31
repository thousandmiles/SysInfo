#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "httpserver.h"
#include <string.h>
#include "cpu.h"
#include "time.h"
#include "process.h"
#include "disk.h"
#include "memory.h"

extern char *ip_address;

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls)
{
    int ret = MHD_YES;
    if (strcmp(method, "GET") == 0)
    {
        ret = handle_GET_url(url, connection);
    }
    else
    {
        //...
    }

    return ret == MHD_YES ? ret : handle_404_error(connection);
}

int handle_GET_url(const char *url, struct MHD_Connection *connection)
{
    printf("%s\n", url);

    if (ip_address)
    {
        printf("ip: %s\n", ip_address);
    }

    if (strcmp(url, "/cpu-info") == 0)
    {
        return handle_get_cpu_info(connection);
    }
    else if (strcmp(url, "/cpu-total-time") == 0)
    {
        return handle_get_cpu_total_time(connection);
    }
    else if (strcmp(url, "/process-info") == 0)
    {
        return handle_get_process_info_list(connection);
    }
    else if (strcmp(url, "/disk-info") == 0)
    {
        return handle_get_disk_info_list(connection);
    }
    else if (strcmp(url, "/machine-memory") == 0)
    {
        return handle_get_machine_memory_info_list(connection);
    }
    else if (strncmp(url, "/process-total-time/", 20) == 0)
    {
        unsigned int pid;
        if (sscanf(url + 20, "%u", &pid) == 1)
        {
            return handle_get_process_cpu_total_time(pid, connection);
        }
    }
    else if (strncmp(url, "/process-memory/", 16) == 0)
    {
        unsigned int pid;
        if (sscanf(url + 16, "%u", &pid) == 1)
        {
            return handle_get_process_memory(pid, connection);
        }
    }

    return MHD_NO;
}

int handle_404_error(struct MHD_Connection *connection)
{
    const char *page = "<html><body>404 Not Found</body></html>";
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_MUST_COPY);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_cpu_info(struct MHD_Connection *connection)
{
    /*
    typedef struct
    {
        char vendor_id[128];
        char model_name[256];
        double cpu_mhz;
        unsigned int cache_size; // kB
        unsigned char core_num;
    } CPU_Info;
    */

    CPU_Info info;
    get_CPU_info(&info);
    const char *response_json = "{\"vendor_id\": \"%s\", \"model_name\": \"%s\", \"cpu_mhz\": %lf, \"cache_size\": %u, \"core_num\": %hu}";
    char buffer[512];
    snprintf(buffer, sizeof(buffer), response_json, info.vendor_id, info.model_name, info.cpu_mhz, info.cache_size, info.core_num);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_cpu_total_time(struct MHD_Connection *connection)
{
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);

    unsigned long total_cpu_time = get_total_cpu_time();
    const char *response_json = "{\"current_time\": \"%s\", \"total_cpu_time\": %u}";
    char buffer[128];
    snprintf(buffer, sizeof(buffer), response_json, asctime(local_time), total_cpu_time);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_process_cpu_total_time(unsigned int pid, struct MHD_Connection *connection)
{
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);

    unsigned long total_process_time = get_total_process_time(pid);
    const char *response_json = "{\"pid\": %u, \"current_time\": \"%s\", \"total_cpu_time\": %u}";
    char buffer[128];
    snprintf(buffer, sizeof(buffer), response_json, pid, asctime(local_time), total_process_time);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_process_info_list(struct MHD_Connection *connection)
{
    char *buffer = get_process_json_list();
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    free(buffer);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_disk_info_list(struct MHD_Connection *connection)
{
    char *buffer = get_disk_json_list();
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    free(buffer);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_process_memory(unsigned int pid, struct MHD_Connection *connection)
{
    char *buffer = get_process_memory_json(pid);
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    free(buffer);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int handle_get_machine_memory_info_list(struct MHD_Connection *connection)
{
    char *buffer = get_machine_memory_json();
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(buffer), buffer, MHD_RESPMEM_MUST_COPY);
    free(buffer);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}