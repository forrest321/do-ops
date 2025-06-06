#ifndef DIGITALOCEAN_HTTP_H
#define DIGITALOCEAN_HTTP_H

#include "types.h"
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} do_http_response_t;

typedef struct {
    CURL *curl;
    char *user_agent;
    long timeout;
} do_http_client_t;

// HTTP client functions
do_http_client_t *do_http_client_new(void);
void do_http_client_free(do_http_client_t *client);

do_result_t do_http_client_init(do_http_client_t *client);
do_result_t do_http_client_set_timeout(do_http_client_t *client, long timeout_seconds);
do_result_t do_http_client_set_user_agent(do_http_client_t *client, const char *user_agent);

// HTTP response functions
do_http_response_t *do_http_response_new(void);
void do_http_response_free(do_http_response_t *response);
void do_http_response_clear(do_http_response_t *response);

// HTTP request functions
do_result_t do_http_get(do_http_client_t *client, const char *url, 
                        const char *auth_header, do_http_response_t *response);

do_result_t do_http_post(do_http_client_t *client, const char *url, 
                         const char *auth_header, const char *json_data,
                         do_http_response_t *response);

do_result_t do_http_delete(do_http_client_t *client, const char *url, 
                           const char *auth_header, do_http_response_t *response);

// Utility functions
size_t do_http_write_callback(void *contents, size_t size, size_t nmemb, do_http_response_t *response);
char *do_http_build_auth_header(const char *token);
char *do_http_build_url(const char *base_url, const char *endpoint);

// Error handling
const char *do_http_get_error_string(CURLcode code);
do_result_t do_http_code_to_result(CURLcode code);

#ifdef __cplusplus
}
#endif

#endif // DIGITALOCEAN_HTTP_H