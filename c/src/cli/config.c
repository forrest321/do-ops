#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "digitalocean/config.h"

int cmd_config_set(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: config-set <key> <value>\n");
        fprintf(stderr, "Keys: token, base-url\n");
        return 1;
    }
    
    const char *key = argv[1];
    const char *value = argv[2];
    
    do_config_t *config = do_config_new();
    if (!config) {
        fprintf(stderr, "Failed to create config\n");
        return 1;
    }
    
    // Load existing config first
    do_config_load(config);
    
    do_result_t result;
    if (strcmp(key, "token") == 0) {
        result = do_config_set_token(config, value);
    } else if (strcmp(key, "base-url") == 0) {
        result = do_config_set_base_url(config, value);
    } else {
        fprintf(stderr, "Unknown config key: %s\n", key);
        do_config_free(config);
        return 1;
    }
    
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to set config value\n");
        do_config_free(config);
        return 1;
    }
    
    result = do_config_save(config);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to save config\n");
        do_config_free(config);
        return 1;
    }
    
    printf("Configuration updated: %s = %s\n", key, value);
    do_config_free(config);
    return 0;
}

int cmd_config_get(int argc, char **argv) {
    do_config_t *config = do_config_new();
    if (!config) {
        fprintf(stderr, "Failed to create config\n");
        return 1;
    }
    
    do_result_t result = do_config_load(config);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to load config\n");
        do_config_free(config);
        return 1;
    }
    
    if (argc >= 2) {
        const char *key = argv[1];
        
        if (strcmp(key, "token") == 0) {
            char *masked = do_config_mask_token(do_config_get_token(config));
            printf("token: %s\n", masked ? masked : "<error>");
            free(masked);
        } else if (strcmp(key, "base-url") == 0) {
            printf("base-url: %s\n", do_config_get_base_url(config));
        } else {
            fprintf(stderr, "Unknown config key: %s\n", key);
            do_config_free(config);
            return 1;
        }
    } else {
        printf("Configuration:\n");
        char *masked = do_config_mask_token(do_config_get_token(config));
        printf("  token: %s\n", masked ? masked : "<error>");
        printf("  base-url: %s\n", do_config_get_base_url(config));
        free(masked);
    }
    
    do_config_free(config);
    return 0;
}