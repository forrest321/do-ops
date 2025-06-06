#include <stdlib.h>
#include <string.h>
#include <cjson/cjson.h>
#include "digitalocean/types.h"

// Helper function to safely get string from JSON
static char *json_get_string(const cJSON *json, const char *key) {
    const cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
    if (!cJSON_IsString(item) || item->valuestring == NULL) {
        return NULL;
    }
    return strdup(item->valuestring);
}

// Helper function to safely get number from JSON
static double json_get_number(const cJSON *json, const char *key, double default_value) {
    const cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
    if (!cJSON_IsNumber(item)) {
        return default_value;
    }
    return item->valuedouble;
}

// Helper function to safely get boolean from JSON
static bool json_get_bool(const cJSON *json, const char *key, bool default_value) {
    const cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
    if (!cJSON_IsBool(item)) {
        return default_value;
    }
    return cJSON_IsTrue(item);
}

// Parse string array from JSON
static do_result_t json_parse_string_array(const cJSON *json, const char *key, 
                                           do_string_array_t *array) {
    const cJSON *json_array = cJSON_GetObjectItemCaseSensitive(json, key);
    if (!cJSON_IsArray(json_array)) {
        return DO_SUCCESS; // Empty array is OK
    }
    
    const cJSON *item;
    cJSON_ArrayForEach(item, json_array) {
        if (cJSON_IsString(item) && item->valuestring != NULL) {
            if (do_string_array_add(array, item->valuestring) != DO_SUCCESS) {
                return DO_ERROR_MEMORY;
            }
        }
    }
    
    return DO_SUCCESS;
}

// Parse region from JSON
static do_result_t json_parse_region(const cJSON *json, do_region_t *region) {
    if (!json || !region) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    region->name = json_get_string(json, "name");
    region->slug = json_get_string(json, "slug");
    region->available = json_get_bool(json, "available", false);
    
    do_string_array_init(&region->features);
    json_parse_string_array(json, "features", &region->features);
    
    do_string_array_init(&region->sizes);
    json_parse_string_array(json, "sizes", &region->sizes);
    
    return DO_SUCCESS;
}

// Parse size from JSON
static do_result_t json_parse_size(const cJSON *json, do_size_t *size) {
    if (!json || !size) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    size->slug = json_get_string(json, "slug");
    size->memory = (uint32_t)json_get_number(json, "memory", 0);
    size->vcpus = (uint32_t)json_get_number(json, "vcpus", 0);
    size->disk = (uint32_t)json_get_number(json, "disk", 0);
    size->transfer = json_get_number(json, "transfer", 0.0);
    size->price_monthly = json_get_number(json, "price_monthly", 0.0);
    size->price_hourly = json_get_number(json, "price_hourly", 0.0);
    size->available = json_get_bool(json, "available", false);
    
    do_string_array_init(&size->regions);
    json_parse_string_array(json, "regions", &size->regions);
    
    return DO_SUCCESS;
}

// Parse networks from JSON
static do_result_t json_parse_networks(const cJSON *json, do_networks_t *networks) {
    if (!json || !networks) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    // Parse IPv4 networks
    const cJSON *v4_array = cJSON_GetObjectItemCaseSensitive(json, "v4");
    if (cJSON_IsArray(v4_array)) {
        networks->v4_count = cJSON_GetArraySize(v4_array);
        if (networks->v4_count > 0) {
            networks->v4 = calloc(networks->v4_count, sizeof(do_network_v4_t));
            if (!networks->v4) {
                return DO_ERROR_MEMORY;
            }
            
            size_t i = 0;
            const cJSON *item;
            cJSON_ArrayForEach(item, v4_array) {
                networks->v4[i].ip_address = json_get_string(item, "ip_address");
                networks->v4[i].netmask = json_get_string(item, "netmask");
                networks->v4[i].gateway = json_get_string(item, "gateway");
                networks->v4[i].type = json_get_string(item, "type");
                i++;
            }
        }
    }
    
    // Parse IPv6 networks
    const cJSON *v6_array = cJSON_GetObjectItemCaseSensitive(json, "v6");
    if (cJSON_IsArray(v6_array)) {
        networks->v6_count = cJSON_GetArraySize(v6_array);
        if (networks->v6_count > 0) {
            networks->v6 = calloc(networks->v6_count, sizeof(do_network_v6_t));
            if (!networks->v6) {
                return DO_ERROR_MEMORY;
            }
            
            size_t i = 0;
            const cJSON *item;
            cJSON_ArrayForEach(item, v6_array) {
                networks->v6[i].ip_address = json_get_string(item, "ip_address");
                networks->v6[i].netmask = (uint32_t)json_get_number(item, "netmask", 0);
                networks->v6[i].gateway = json_get_string(item, "gateway");
                networks->v6[i].type = json_get_string(item, "type");
                i++;
            }
        }
    }
    
    return DO_SUCCESS;
}

// Parse droplet from JSON
do_result_t json_parse_droplet(const cJSON *json, do_droplet_t *droplet) {
    if (!json || !droplet) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    droplet->id = (uint32_t)json_get_number(json, "id", 0);
    droplet->name = json_get_string(json, "name");
    droplet->memory = (uint32_t)json_get_number(json, "memory", 0);
    droplet->vcpus = (uint32_t)json_get_number(json, "vcpus", 0);
    droplet->disk = (uint32_t)json_get_number(json, "disk", 0);
    droplet->locked = json_get_bool(json, "locked", false);
    droplet->status = json_get_string(json, "status");
    droplet->size_slug = json_get_string(json, "size_slug");
    droplet->vpc_uuid = json_get_string(json, "vpc_uuid");
    
    // Parse created_at (simplified - would need proper ISO 8601 parsing)
    const char *created_str = json_get_string(json, "created_at");
    if (created_str) {
        // This is a simplified parsing - in production, use strptime or similar
        droplet->created_at = time(NULL); // Placeholder
        free((char*)created_str);
    }
    
    // Parse nested objects
    const cJSON *region_json = cJSON_GetObjectItemCaseSensitive(json, "region");
    if (region_json) {
        droplet->region = calloc(1, sizeof(do_region_t));
        if (droplet->region) {
            json_parse_region(region_json, droplet->region);
        }
    }
    
    const cJSON *size_json = cJSON_GetObjectItemCaseSensitive(json, "size");
    if (size_json) {
        droplet->size = calloc(1, sizeof(do_size_t));
        if (droplet->size) {
            json_parse_size(size_json, droplet->size);
        }
    }
    
    const cJSON *networks_json = cJSON_GetObjectItemCaseSensitive(json, "networks");
    if (networks_json) {
        droplet->networks = calloc(1, sizeof(do_networks_t));
        if (droplet->networks) {
            json_parse_networks(networks_json, droplet->networks);
        }
    }
    
    // Parse string arrays
    do_string_array_init(&droplet->features);
    json_parse_string_array(json, "features", &droplet->features);
    
    do_string_array_init(&droplet->tags);
    json_parse_string_array(json, "tags", &droplet->tags);
    
    do_string_array_init(&droplet->volume_ids);
    json_parse_string_array(json, "volume_ids", &droplet->volume_ids);
    
    return DO_SUCCESS;
}

// Parse account from JSON
do_result_t json_parse_account(const cJSON *json, do_account_t *account) {
    if (!json || !account) {
        return DO_ERROR_INVALID_PARAM;
    }
    
    account->droplet_limit = (uint32_t)json_get_number(json, "droplet_limit", 0);
    account->floating_ip_limit = (uint32_t)json_get_number(json, "floating_ip_limit", 0);
    account->volume_limit = (uint32_t)json_get_number(json, "volume_limit", 0);
    account->email = json_get_string(json, "email");
    account->uuid = json_get_string(json, "uuid");
    account->email_verified = json_get_bool(json, "email_verified", false);
    account->status = json_get_string(json, "status");
    account->status_message = json_get_string(json, "status_message");
    
    // Parse team if present
    const cJSON *team_json = cJSON_GetObjectItemCaseSensitive(json, "team");
    if (team_json) {
        account->team = calloc(1, sizeof(do_team_t));
        if (account->team) {
            account->team->uuid = json_get_string(team_json, "uuid");
            account->team->name = json_get_string(team_json, "name");
        }
    }
    
    return DO_SUCCESS;
}