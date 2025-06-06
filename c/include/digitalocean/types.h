#ifndef DIGITALOCEAN_TYPES_H
#define DIGITALOCEAN_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
typedef enum {
    DO_SUCCESS = 0,
    DO_ERROR_INVALID_PARAM = -1,
    DO_ERROR_MEMORY = -2,
    DO_ERROR_HTTP = -3,
    DO_ERROR_JSON = -4,
    DO_ERROR_AUTH = -5,
    DO_ERROR_CONFIG = -6,
    DO_ERROR_NOT_FOUND = -7,
    DO_ERROR_RATE_LIMIT = -8
} do_result_t;

// Basic types
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} do_string_t;

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} do_string_array_t;

// Meta and pagination
typedef struct {
    uint32_t total;
} do_meta_t;

typedef struct {
    char *first;
    char *prev;
    char *next;
    char *last;
} do_pages_t;

typedef struct {
    do_pages_t *pages;
} do_links_t;

// Region
typedef struct {
    char *name;
    char *slug;
    do_string_array_t features;
    bool available;
    do_string_array_t sizes;
} do_region_t;

// Size
typedef struct {
    char *slug;
    uint32_t memory;
    uint32_t vcpus;
    uint32_t disk;
    double transfer;
    double price_monthly;
    double price_hourly;
    do_string_array_t regions;
    bool available;
} do_size_t;

// Kernel
typedef struct {
    uint32_t id;
    char *name;
    char *version;
} do_kernel_t;

// Image
typedef struct {
    uint32_t id;
    char *name;
    char *type;
    char *distribution;
    char *slug;
    bool public;
    do_string_array_t regions;
    uint32_t min_disk_size;
    double size_gigabytes;
    time_t created_at;
    char *description;
    do_string_array_t tags;
    char *status;
    char *error_message;
} do_image_t;

// Networks
typedef struct {
    char *ip_address;
    char *netmask;
    char *gateway;
    char *type;
} do_network_v4_t;

typedef struct {
    char *ip_address;
    uint32_t netmask;
    char *gateway;
    char *type;
} do_network_v6_t;

typedef struct {
    do_network_v4_t *v4;
    size_t v4_count;
    do_network_v6_t *v6;
    size_t v6_count;
} do_networks_t;

// Droplet
typedef struct {
    uint32_t id;
    char *name;
    uint32_t memory;
    uint32_t vcpus;
    uint32_t disk;
    bool locked;
    char *status;
    do_kernel_t *kernel;
    time_t created_at;
    do_string_array_t features;
    uint32_t *backup_ids;
    size_t backup_ids_count;
    uint32_t *snapshot_ids;
    size_t snapshot_ids_count;
    do_image_t *image;
    do_size_t *size;
    char *size_slug;
    do_networks_t *networks;
    do_region_t *region;
    do_string_array_t tags;
    do_string_array_t volume_ids;
    char *vpc_uuid;
} do_droplet_t;

typedef struct {
    do_droplet_t *items;
    size_t count;
    size_t capacity;
    do_links_t links;
    do_meta_t meta;
} do_droplet_list_t;

// Create droplet request
typedef struct {
    char *name;
    char *region;
    char *size;
    char *image;
    do_string_array_t ssh_keys;
    bool backups;
    bool ipv6;
    bool monitoring;
    do_string_array_t tags;
    char *user_data;
    bool private_networking;
    do_string_array_t volumes;
    char *vpc_uuid;
} do_create_droplet_request_t;

// Team
typedef struct {
    char *uuid;
    char *name;
} do_team_t;

// Account
typedef struct {
    uint32_t droplet_limit;
    uint32_t floating_ip_limit;
    uint32_t volume_limit;
    char *email;
    char *uuid;
    bool email_verified;
    char *status;
    char *status_message;
    do_team_t *team;
} do_account_t;

// Function declarations for memory management
void do_string_init(do_string_t *str);
void do_string_free(do_string_t *str);
do_result_t do_string_set(do_string_t *str, const char *value);

void do_string_array_init(do_string_array_t *arr);
void do_string_array_free(do_string_array_t *arr);
do_result_t do_string_array_add(do_string_array_t *arr, const char *item);

void do_droplet_free(do_droplet_t *droplet);
void do_droplet_list_free(do_droplet_list_t *list);
void do_account_free(do_account_t *account);

#ifdef __cplusplus
}
#endif

#endif // DIGITALOCEAN_TYPES_H