#include <stdlib.h>
#include <string.h>
#include "digitalocean/types.h"

void do_string_init(do_string_t *str) {
    if (!str) return;
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

void do_string_free(do_string_t *str) {
    if (!str) return;
    free(str->data);
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

do_result_t do_string_set(do_string_t *str, const char *value) {
    if (!str) return DO_ERROR_INVALID_PARAM;
    
    do_string_free(str);
    
    if (!value) return DO_SUCCESS;
    
    size_t len = strlen(value);
    str->data = malloc(len + 1);
    if (!str->data) return DO_ERROR_MEMORY;
    
    strcpy(str->data, value);
    str->length = len;
    str->capacity = len + 1;
    
    return DO_SUCCESS;
}

void do_string_array_init(do_string_array_t *arr) {
    if (!arr) return;
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

void do_string_array_free(do_string_array_t *arr) {
    if (!arr) return;
    
    for (size_t i = 0; i < arr->count; i++) {
        free(arr->items[i]);
    }
    free(arr->items);
    
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

do_result_t do_string_array_add(do_string_array_t *arr, const char *item) {
    if (!arr || !item) return DO_ERROR_INVALID_PARAM;
    
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity ? arr->capacity * 2 : 4;
        char **new_items = realloc(arr->items, new_capacity * sizeof(char*));
        if (!new_items) return DO_ERROR_MEMORY;
        
        arr->items = new_items;
        arr->capacity = new_capacity;
    }
    
    arr->items[arr->count] = strdup(item);
    if (!arr->items[arr->count]) return DO_ERROR_MEMORY;
    
    arr->count++;
    return DO_SUCCESS;
}

static void do_networks_free(do_networks_t *networks) {
    if (!networks) return;
    
    for (size_t i = 0; i < networks->v4_count; i++) {
        free(networks->v4[i].ip_address);
        free(networks->v4[i].netmask);
        free(networks->v4[i].gateway);
        free(networks->v4[i].type);
    }
    free(networks->v4);
    
    for (size_t i = 0; i < networks->v6_count; i++) {
        free(networks->v6[i].ip_address);
        free(networks->v6[i].gateway);
        free(networks->v6[i].type);
    }
    free(networks->v6);
    
    free(networks);
}

static void do_region_free(do_region_t *region) {
    if (!region) return;
    
    free(region->name);
    free(region->slug);
    do_string_array_free(&region->features);
    do_string_array_free(&region->sizes);
    free(region);
}

static void do_size_free(do_size_t *size) {
    if (!size) return;
    
    free(size->slug);
    do_string_array_free(&size->regions);
    free(size);
}

static void do_image_free(do_image_t *image) {
    if (!image) return;
    
    free(image->name);
    free(image->type);
    free(image->distribution);
    free(image->slug);
    free(image->description);
    free(image->status);
    free(image->error_message);
    do_string_array_free(&image->regions);
    do_string_array_free(&image->tags);
    free(image);
}

void do_droplet_free(do_droplet_t *droplet) {
    if (!droplet) return;
    
    free(droplet->name);
    free(droplet->status);
    free(droplet->size_slug);
    free(droplet->vpc_uuid);
    
    if (droplet->kernel) {
        free(droplet->kernel->name);
        free(droplet->kernel->version);
        free(droplet->kernel);
    }
    
    do_image_free(droplet->image);
    do_size_free(droplet->size);
    do_networks_free(droplet->networks);
    do_region_free(droplet->region);
    
    do_string_array_free(&droplet->features);
    do_string_array_free(&droplet->tags);
    do_string_array_free(&droplet->volume_ids);
    
    free(droplet->backup_ids);
    free(droplet->snapshot_ids);
}

void do_droplet_list_free(do_droplet_list_t *list) {
    if (!list) return;
    
    for (size_t i = 0; i < list->count; i++) {
        do_droplet_free(&list->items[i]);
    }
    free(list->items);
    free(list);
}

void do_account_free(do_account_t *account) {
    if (!account) return;
    
    free(account->email);
    free(account->uuid);
    free(account->status);
    free(account->status_message);
    
    if (account->team) {
        free(account->team->uuid);
        free(account->team->name);
        free(account->team);
    }
    
    free(account);
}