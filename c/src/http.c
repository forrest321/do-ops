#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "digitalocean/http.h"

// Write callback for libcurl
size_t do_http_write_callback(void *contents, size_t size, size_t nmemb, do_http_response_t *response) {
    size_t real_size = size * nmemb;
    
    // Resize buffer if needed
    if (response->size + real_size + 1 > response->capacity) {
        size_t new_capacity = response->capacity ? response->capacity * 2 : 1024;
        while (new_capacity < response->size + real_size + 1) {
            new_capacity *= 2;
        }
        
        char *new_data = realloc(response->data, new_capacity);
        if (!new_data) {
            return 0; // Error
        }
        
        response->data = new_data;
        response->capacity = new_capacity;
    }
    
    memcpy(response->data + response->size, contents, real_size);
    response->size += real_size;
    response->data[response->size] = '\0';
    
    return real_size;
}

do_http_client_t *do_http_client_new(void) {
    do_http_client_t *client = calloc(1, sizeof(do_http_client_t));
    if (!client) {
        return NULL;
    }
    
    client->timeout = 30; // 30 seconds default
    client->user_agent = strdup("digitalocean-c/1.0.0");
    
    return client;
}

void do_http_client_free(do_http_client_t *client) {
    if (!client) {
        return;
    }
    
    if (client->curl) {
        curl_easy_cleanup(client->curl);
    }
    
    free(client->user_agent);
    free(client);
}

do_result_t do_http_client_init(do_http_client_t *client) {
    if (!client) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    client->curl = curl_easy_init();
    if (!client->curl) {
        return DO_ERROR_HTTP;
    }
    
    // Set common options
    curl_easy_setopt(client->curl, CURLOPT_TIMEOUT, client->timeout);
    curl_easy_setopt(client->curl, CURLOPT_USERAGENT, client->user_agent);
    curl_easy_setopt(client->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(client->curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(client->curl, CURLOPT_SSL_VERIFYHOST, 2L);
    
    return DO_SUCCESS;
}

do_result_t do_http_client_set_timeout(do_http_client_t *client, long timeout_seconds) {
    if (!client || timeout_seconds < 0) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    client->timeout = timeout_seconds;
    if (client->curl) {
        curl_easy_setopt(client->curl, CURLOPT_TIMEOUT, timeout_seconds);
    }
    
    return DO_SUCCESS;
}

do_result_t do_http_client_set_user_agent(do_http_client_t *client, const char *user_agent) {
    if (!client || !user_agent) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    free(client->user_agent);
    client->user_agent = strdup(user_agent);
    if (!client->user_agent) {
        return DO_ERROR_MEMORY;
    }
    
    if (client->curl) {
        curl_easy_setopt(client->curl, CURLOPT_USERAGENT, client->user_agent);
    }
    
    return DO_SUCCESS;
}

do_http_response_t *do_http_response_new(void) {
    do_http_response_t *response = calloc(1, sizeof(do_http_response_t));
    return response;
}

void do_http_response_free(do_http_response_t *response) {
    if (!response) {
        return;
    }
    
    free(response->data);
    free(response);
}

void do_http_response_clear(do_http_response_t *response) {
    if (!response) {
        return;
    }
    
    response->size = 0;
    if (response->data) {
        response->data[0] = '\0';
    }
}

do_result_t do_http_get(do_http_client_t *client, const char *url, 
                        const char *auth_header, do_http_response_t *response) {
    if (!client || !client->curl || !url || !response) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    struct curl_slist *headers = NULL;
    
    // Set headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (auth_header) {
        headers = curl_slist_append(headers, auth_header);
    }
    
    // Configure request
    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, do_http_write_callback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, response);
    
    // Perform request
    CURLcode res = curl_easy_perform(client->curl);
    
    // Clean up
    curl_slist_free_all(headers);
    
    return do_http_code_to_result(res);
}

do_result_t do_http_post(do_http_client_t *client, const char *url, 
                         const char *auth_header, const char *json_data,
                         do_http_response_t *response) {
    if (!client || !client->curl || !url || !response) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    struct curl_slist *headers = NULL;
    
    // Set headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (auth_header) {
        headers = curl_slist_append(headers, auth_header);
    }
    
    // Configure request
    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_POST, 1L);
    curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, do_http_write_callback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, response);
    
    if (json_data) {
        curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(client->curl, CURLOPT_POSTFIELDSIZE, strlen(json_data));
    }
    
    // Perform request
    CURLcode res = curl_easy_perform(client->curl);
    
    // Clean up
    curl_slist_free_all(headers);
    
    return do_http_code_to_result(res);
}

do_result_t do_http_delete(do_http_client_t *client, const char *url, 
                           const char *auth_header, do_http_response_t *response) {
    if (!client || !client->curl || !url || !response) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    struct curl_slist *headers = NULL;
    
    // Set headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (auth_header) {
        headers = curl_slist_append(headers, auth_header);
    }
    
    // Configure request
    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, do_http_write_callback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, response);
    
    // Perform request
    CURLcode res = curl_easy_perform(client->curl);
    
    // Clean up
    curl_slist_free_all(headers);
    
    return do_http_code_to_result(res);
}

char *do_http_build_auth_header(const char *token) {
    if (!token) {
        return NULL;
    }
    
    size_t len = strlen("Authorization: Bearer ") + strlen(token) + 1;
    char *header = malloc(len);
    if (!header) {
        return NULL;
    }
    
    snprintf(header, len, "Authorization: Bearer %s", token);
    return header;
}

char *do_http_build_url(const char *base_url, const char *endpoint) {
    if (!base_url || !endpoint) {
        return NULL;
    }
    
    size_t len = strlen(base_url) + strlen(endpoint) + 1;
    char *url = malloc(len);
    if (!url) {
        return NULL;
    }
    
    snprintf(url, len, "%s%s", base_url, endpoint);
    return url;
}

const char *do_http_get_error_string(CURLcode code) {
    return curl_easy_strerror(code);
}

do_result_t do_http_code_to_result(CURLcode code) {
    switch (code) {
        case CURLE_OK:
            return DO_SUCCESS;
        case CURLE_OUT_OF_MEMORY:
            return DO_ERROR_MEMORY;
        case CURLE_COULDNT_CONNECT:
        case CURLE_COULDNT_RESOLVE_HOST:
        case CURLE_OPERATION_TIMEDOUT:
            return DO_ERROR_HTTP;
        default:
            return DO_ERROR_HTTP;
    }
}