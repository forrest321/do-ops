#ifndef DIGITALOCEAN_CONFIG_H
#define DIGITALOCEAN_CONFIG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DO_DEFAULT_BASE_URL "https://api.digitalocean.com"
#define DO_CONFIG_FILE_NAME "config.ini"

typedef struct {
    char *token;
    char *base_url;
} do_config_t;

// Configuration functions
do_config_t *do_config_new(void);
void do_config_free(do_config_t *config);

do_result_t do_config_load(do_config_t *config);
do_result_t do_config_save(const do_config_t *config);

do_result_t do_config_set_token(do_config_t *config, const char *token);
do_result_t do_config_set_base_url(do_config_t *config, const char *base_url);

const char *do_config_get_token(const do_config_t *config);
const char *do_config_get_base_url(const do_config_t *config);

bool do_config_is_valid(const do_config_t *config);
do_result_t do_config_validate(const do_config_t *config);

// Utility functions
char *do_config_get_config_dir(void);
char *do_config_get_config_path(void);
char *do_config_mask_token(const char *token);

#ifdef __cplusplus
}
#endif

#endif // DIGITALOCEAN_CONFIG_H