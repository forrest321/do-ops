#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cjson.h>
#include "digitalocean/client.h"

// Forward declarations for JSON parsing
extern do_result_t json_parse_droplet(const cJSON *json, do_droplet_t *droplet);
extern do_result_t json_parse_account(const cJSON *json, do_account_t *account);

do_client_t *do_client_new(void) {
    do_client_t *client = calloc(1, sizeof(do_client_t));
    return client;
}

void do_client_free(do_client_t *client) {
    if (!client) {
        return;
    }
    
    do_config_free(client->config);
    do_http_client_free(client->http_client);
    free(client->auth_header);
    free(client);
}

do_result_t do_client_init(do_client_t *client, do_config_t *config) {
    if (!client || !config) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    do_result_t result = do_config_validate(config);
    if (result != DO_SUCCESS) {
        return result;
    }
    
    client->config = config;
    
    // Initialize HTTP client
    client->http_client = do_http_client_new();
    if (!client->http_client) {
        return DO_ERROR_MEMORY;
    }
    
    result = do_http_client_init(client->http_client);
    if (result != DO_SUCCESS) {
        return result;
    }
    
    // Build auth header
    client->auth_header = do_http_build_auth_header(config->token);
    if (!client->auth_header) {
        return DO_ERROR_MEMORY;
    }
    
    return DO_SUCCESS;
}

do_result_t do_client_init_from_config(do_client_t *client) {
    if (!client) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    do_config_t *config = do_config_new();
    if (!config) {
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_config_load(config);
    if (result != DO_SUCCESS) {
        do_config_free(config);
        return result;
    }
    
    return do_client_init(client, config);
}

do_result_t do_client_get_account(do_client_t *client, do_account_t **account) {
    if (!client || !account) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    char *url = do_http_build_url(client->config->base_url, "/v2/account");
    if (!url) {
        return DO_ERROR_MEMORY;
    }
    
    do_http_response_t *response = do_http_response_new();
    if (!response) {
        free(url);
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_http_get(client->http_client, url, client->auth_header, response);
    free(url);
    
    if (result != DO_SUCCESS) {
        do_http_response_free(response);
        return result;
    }
    
    // Parse JSON response
    cJSON *json = cJSON_Parse(response->data);
    do_http_response_free(response);
    
    if (!json) {
        return DO_ERROR_JSON;
    }
    
    cJSON *account_json = cJSON_GetObjectItemCaseSensitive(json, "account");
    if (!account_json) {
        cJSON_Delete(json);
        return DO_ERROR_JSON;
    }
    
    *account = calloc(1, sizeof(do_account_t));
    if (!*account) {
        cJSON_Delete(json);
        return DO_ERROR_MEMORY;
    }
    
    result = json_parse_account(account_json, *account);
    cJSON_Delete(json);
    
    if (result != DO_SUCCESS) {
        do_account_free(*account);
        *account = NULL;
    }
    
    return result;
}

do_result_t do_client_list_droplets(do_client_t *client, do_droplet_list_t **droplets) {
    if (!client || !droplets) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    char *url = do_http_build_url(client->config->base_url, "/v2/droplets");
    if (!url) {
        return DO_ERROR_MEMORY;
    }
    
    do_http_response_t *response = do_http_response_new();
    if (!response) {
        free(url);
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_http_get(client->http_client, url, client->auth_header, response);
    free(url);
    
    if (result != DO_SUCCESS) {
        do_http_response_free(response);
        return result;
    }
    
    // Parse JSON response
    cJSON *json = cJSON_Parse(response->data);
    do_http_response_free(response);
    
    if (!json) {
        return DO_ERROR_JSON;
    }
    
    cJSON *droplets_array = cJSON_GetObjectItemCaseSensitive(json, "droplets");
    if (!cJSON_IsArray(droplets_array)) {
        cJSON_Delete(json);
        return DO_ERROR_JSON;
    }
    
    *droplets = calloc(1, sizeof(do_droplet_list_t));
    if (!*droplets) {
        cJSON_Delete(json);
        return DO_ERROR_MEMORY;
    }
    
    size_t count = cJSON_GetArraySize(droplets_array);
    if (count > 0) {
        (*droplets)->items = calloc(count, sizeof(do_droplet_t));
        if (!(*droplets)->items) {
            free(*droplets);
            *droplets = NULL;
            cJSON_Delete(json);
            return DO_ERROR_MEMORY;
        }
        
        (*droplets)->count = count;
        (*droplets)->capacity = count;
        
        size_t i = 0;
        const cJSON *droplet_json;
        cJSON_ArrayForEach(droplet_json, droplets_array) {
            result = json_parse_droplet(droplet_json, &(*droplets)->items[i]);
            if (result != DO_SUCCESS) {
                do_droplet_list_free(*droplets);
                *droplets = NULL;
                cJSON_Delete(json);
                return result;
            }
            i++;
        }
    }
    
    cJSON_Delete(json);
    return DO_SUCCESS;
}

do_result_t do_client_get_droplet(do_client_t *client, uint32_t id, do_droplet_t **droplet) {
    if (!client || !droplet) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    char endpoint[64];
    snprintf(endpoint, sizeof(endpoint), "/v2/droplets/%u", id);
    
    char *url = do_http_build_url(client->config->base_url, endpoint);
    if (!url) {
        return DO_ERROR_MEMORY;
    }
    
    do_http_response_t *response = do_http_response_new();
    if (!response) {
        free(url);
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_http_get(client->http_client, url, client->auth_header, response);
    free(url);
    
    if (result != DO_SUCCESS) {
        do_http_response_free(response);
        return result;
    }
    
    // Parse JSON response
    cJSON *json = cJSON_Parse(response->data);
    do_http_response_free(response);
    
    if (!json) {
        return DO_ERROR_JSON;
    }
    
    cJSON *droplet_json = cJSON_GetObjectItemCaseSensitive(json, "droplet");
    if (!droplet_json) {
        cJSON_Delete(json);
        return DO_ERROR_JSON;
    }
    
    *droplet = calloc(1, sizeof(do_droplet_t));
    if (!*droplet) {
        cJSON_Delete(json);
        return DO_ERROR_MEMORY;
    }
    
    result = json_parse_droplet(droplet_json, *droplet);
    cJSON_Delete(json);
    
    if (result != DO_SUCCESS) {
        do_droplet_free(*droplet);
        *droplet = NULL;
    }
    
    return result;
}

do_result_t do_client_create_droplet(do_client_t *client, 
                                     const do_create_droplet_request_t *request,
                                     do_droplet_t **droplet) {
    if (!client || !request || !droplet) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    // Build JSON request
    cJSON *json = cJSON_CreateObject();
    if (!json) {
        return DO_ERROR_MEMORY;
    }
    
    cJSON_AddStringToObject(json, "name", request->name);
    cJSON_AddStringToObject(json, "region", request->region);
    cJSON_AddStringToObject(json, "size", request->size);
    cJSON_AddStringToObject(json, "image", request->image);
    
    if (request->tags.count > 0) {
        cJSON *tags_array = cJSON_CreateArray();
        for (size_t i = 0; i < request->tags.count; i++) {
            cJSON_AddItemToArray(tags_array, cJSON_CreateString(request->tags.items[i]));
        }
        cJSON_AddItemToObject(json, "tags", tags_array);
    }
    
    char *json_string = cJSON_Print(json);
    cJSON_Delete(json);
    
    if (!json_string) {
        return DO_ERROR_MEMORY;
    }
    
    char *url = do_http_build_url(client->config->base_url, "/v2/droplets");
    if (!url) {
        free(json_string);
        return DO_ERROR_MEMORY;
    }
    
    do_http_response_t *response = do_http_response_new();
    if (!response) {
        free(url);
        free(json_string);
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_http_post(client->http_client, url, client->auth_header, 
                                      json_string, response);
    free(url);
    free(json_string);
    
    if (result != DO_SUCCESS) {
        do_http_response_free(response);
        return result;
    }
    
    // Parse JSON response
    cJSON *response_json = cJSON_Parse(response->data);
    do_http_response_free(response);
    
    if (!response_json) {
        return DO_ERROR_JSON;
    }
    
    cJSON *droplet_json = cJSON_GetObjectItemCaseSensitive(response_json, "droplet");
    if (!droplet_json) {
        cJSON_Delete(response_json);
        return DO_ERROR_JSON;
    }
    
    *droplet = calloc(1, sizeof(do_droplet_t));
    if (!*droplet) {
        cJSON_Delete(response_json);
        return DO_ERROR_MEMORY;
    }
    
    result = json_parse_droplet(droplet_json, *droplet);
    cJSON_Delete(response_json);
    
    if (result != DO_SUCCESS) {
        do_droplet_free(*droplet);
        *droplet = NULL;
    }
    
    return result;
}

do_result_t do_client_delete_droplet(do_client_t *client, uint32_t id) {
    if (!client) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    char endpoint[64];
    snprintf(endpoint, sizeof(endpoint), "/v2/droplets/%u", id);
    
    char *url = do_http_build_url(client->config->base_url, endpoint);
    if (!url) {
        return DO_ERROR_MEMORY;
    }
    
    do_http_response_t *response = do_http_response_new();
    if (!response) {
        free(url);
        return DO_ERROR_MEMORY;
    }
    
    do_result_t result = do_http_delete(client->http_client, url, client->auth_header, response);
    free(url);
    do_http_response_free(response);
    
    return result;
}

const char *do_client_get_error_string(do_result_t result) {
    switch (result) {
        case DO_SUCCESS:
            return "Success";
        case DO_ERROR_INVALID_PARAM:
            return "Invalid parameter";
        case DO_ERROR_MEMORY:
            return "Memory allocation failed";
        case DO_ERROR_HTTP:
            return "HTTP request failed";
        case DO_ERROR_JSON:
            return "JSON parsing failed";
        case DO_ERROR_AUTH:
            return "Authentication failed";
        case DO_ERROR_CONFIG:
            return "Configuration error";
        case DO_ERROR_NOT_FOUND:
            return "Resource not found";
        case DO_ERROR_RATE_LIMIT:
            return "Rate limit exceeded";
        default:
            return "Unknown error";
    }
}

do_result_t do_library_init(void) {
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    return (res == CURLE_OK) ? DO_SUCCESS : DO_ERROR_HTTP;
}

void do_library_cleanup(void) {
    curl_global_cleanup();
}