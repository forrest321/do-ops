#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "digitalocean/config.h"

do_config_t *do_config_new(void) {
    do_config_t *config = calloc(1, sizeof(do_config_t));
    if (!config) {
        return NULL;
    }
    
    // Set default base URL
    config->base_url = strdup(DO_DEFAULT_BASE_URL);
    if (!config->base_url) {
        free(config);
        return NULL;
    }
    
    return config;
}

void do_config_free(do_config_t *config) {
    if (!config) {
        return;
    }
    
    free(config->token);
    free(config->base_url);
    free(config);
}

do_result_t do_config_load(do_config_t *config) {
    if (!config) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    // First try environment variables
    const char *env_token = getenv("DIGITALOCEAN_TOKEN");
    if (env_token) {
        free(config->token);
        config->token = strdup(env_token);
        if (!config->token) {
            return DO_ERROR_MEMORY;
        }
    }
    
    const char *env_base_url = getenv("DIGITALOCEAN_BASE_URL");
    if (env_base_url) {
        free(config->base_url);
        config->base_url = strdup(env_base_url);
        if (!config->base_url) {
            return DO_ERROR_MEMORY;
        }
    }
    
    // If we have a token from environment, we're done
    if (config->token) {
        return DO_SUCCESS;
    }
    
    // Try to load from config file
    char *config_path = do_config_get_config_path();
    if (!config_path) {
        return DO_ERROR_CONFIG;
    }
    
    FILE *file = fopen(config_path, "r");
    free(config_path);
    
    if (!file) {
        // Config file doesn't exist, that's OK
        return DO_SUCCESS;
    }
    
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\0') {
            continue;
        }
        
        // Parse key=value pairs
        char *equals = strchr(line, '=');
        if (!equals) {
            continue;
        }
        
        *equals = '\0';
        char *key = line;
        char *value = equals + 1;
        
        // Trim whitespace
        while (*key == ' ' || *key == '\t') key++;
        while (*value == ' ' || *value == '\t') value++;
        
        if (strcmp(key, "token") == 0 && !config->token) {
            config->token = strdup(value);
        } else if (strcmp(key, "base_url") == 0) {
            free(config->base_url);
            config->base_url = strdup(value);
        }
    }
    
    fclose(file);
    return DO_SUCCESS;
}

do_result_t do_config_save(const do_config_t *config) {
    if (!config) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    char *config_dir = do_config_get_config_dir();
    if (!config_dir) {
        return DO_ERROR_CONFIG;
    }
    
    // Create config directory if it doesn't exist
    struct stat st;
    if (stat(config_dir, &st) != 0) {
        if (mkdir(config_dir, 0755) != 0) {
            free(config_dir);
            return DO_ERROR_CONFIG;
        }
    }
    
    char *config_path = do_config_get_config_path();
    free(config_dir);
    
    if (!config_path) {
        return DO_ERROR_CONFIG;
    }
    
    FILE *file = fopen(config_path, "w");
    free(config_path);
    
    if (!file) {
        return DO_ERROR_CONFIG;
    }
    
    fprintf(file, "# DigitalOcean CLI Configuration\n");
    if (config->token) {
        fprintf(file, "token=%s\n", config->token);
    }
    if (config->base_url) {
        fprintf(file, "base_url=%s\n", config->base_url);
    }
    
    fclose(file);
    return DO_SUCCESS;
}

do_result_t do_config_set_token(do_config_t *config, const char *token) {
    if (!config || !token) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    free(config->token);
    config->token = strdup(token);
    return config->token ? DO_SUCCESS : DO_ERROR_MEMORY;
}

do_result_t do_config_set_base_url(do_config_t *config, const char *base_url) {
    if (!config || !base_url) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    free(config->base_url);
    config->base_url = strdup(base_url);
    return config->base_url ? DO_SUCCESS : DO_ERROR_MEMORY;
}

const char *do_config_get_token(const do_config_t *config) {
    return config ? config->token : NULL;
}

const char *do_config_get_base_url(const do_config_t *config) {
    return config ? config->base_url : NULL;
}

bool do_config_is_valid(const do_config_t *config) {
    return config && config->token && config->base_url;
}

do_result_t do_config_validate(const do_config_t *config) {
    if (!config) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    if (!config->token || strlen(config->token) == 0) {
        return DO_ERROR_AUTH;
    }
    
    if (!config->base_url || strlen(config->base_url) == 0) {
        return DO_ERROR_CONFIG;
    }
    
    return DO_SUCCESS;
}

char *do_config_get_config_dir(void) {
    const char *home = getenv("HOME");
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        if (!pw) {
            return NULL;
        }
        home = pw->pw_dir;
    }
    
    size_t len = strlen(home) + strlen("/.config/do-cli") + 1;
    char *config_dir = malloc(len);
    if (!config_dir) {
        return NULL;
    }
    
    snprintf(config_dir, len, "%s/.config/do-cli", home);
    return config_dir;
}

char *do_config_get_config_path(void) {
    char *config_dir = do_config_get_config_dir();
    if (!config_dir) {
        return NULL;
    }
    
    size_t len = strlen(config_dir) + strlen("/") + strlen(DO_CONFIG_FILE_NAME) + 1;
    char *config_path = malloc(len);
    if (!config_path) {
        free(config_dir);
        return NULL;
    }
    
    snprintf(config_path, len, "%s/%s", config_dir, DO_CONFIG_FILE_NAME);
    free(config_dir);
    return config_path;
}

char *do_config_mask_token(const char *token) {
    if (!token || strlen(token) == 0) {
        return strdup("<not set>");
    }
    
    size_t len = strlen(token);
    if (len <= 8) {
        return strdup("***");
    }
    
    char *masked = malloc(16); // "xxxx...xxxx\0"
    if (!masked) {
        return NULL;
    }
    
    snprintf(masked, 16, "%.4s...%.4s", token, token + len - 4);
    return masked;
}