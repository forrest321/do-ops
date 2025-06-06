#ifndef DIGITALOCEAN_CLIENT_H
#define DIGITALOCEAN_CLIENT_H

#include "types.h"
#include "config.h"
#include "http.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    do_config_t *config;
    do_http_client_t *http_client;
    char *auth_header;
} do_client_t;

// Client lifecycle
do_client_t *do_client_new(void);
void do_client_free(do_client_t *client);

do_result_t do_client_init(do_client_t *client, do_config_t *config);
do_result_t do_client_init_from_config(do_client_t *client);

// Account operations
do_result_t do_client_get_account(do_client_t *client, do_account_t **account);

// Droplet operations
do_result_t do_client_list_droplets(do_client_t *client, do_droplet_list_t **droplets);
do_result_t do_client_get_droplet(do_client_t *client, uint32_t id, do_droplet_t **droplet);
do_result_t do_client_create_droplet(do_client_t *client, 
                                     const do_create_droplet_request_t *request,
                                     do_droplet_t **droplet);
do_result_t do_client_delete_droplet(do_client_t *client, uint32_t id);

// Error handling
const char *do_client_get_error_string(do_result_t result);

// Library initialization/cleanup
do_result_t do_library_init(void);
void do_library_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif // DIGITALOCEAN_CLIENT_H