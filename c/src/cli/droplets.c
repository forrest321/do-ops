#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "digitalocean/client.h"

static const char *get_public_ip(const do_droplet_t *droplet) {
    if (!droplet->networks) return "N/A";
    
    for (size_t i = 0; i < droplet->networks->v4_count; i++) {
        if (droplet->networks->v4[i].type && 
            strcmp(droplet->networks->v4[i].type, "public") == 0) {
            return droplet->networks->v4[i].ip_address;
        }
    }
    return "N/A";
}

static void format_time(time_t timestamp, char *buffer, size_t size) {
    struct tm *tm_info = gmtime(&timestamp);
    strftime(buffer, size, "%Y-%m-%d", tm_info);
}

int cmd_droplets_list(int argc, char **argv) {
    (void)argc; // Unused parameter
    (void)argv; // Unused parameter
    
    do_client_t *client = do_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }
    
    do_result_t result = do_client_init_from_config(client);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    do_droplet_list_t *droplets;
    result = do_client_list_droplets(client, &droplets);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to list droplets: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    if (droplets->count == 0) {
        printf("No droplets found\n");
    } else {
        printf("%-8s %-20s %-10s %-12s %-10s %-15s %s\n", 
               "ID", "NAME", "STATUS", "SIZE", "REGION", "IP", "CREATED");
        printf("%-8s %-20s %-10s %-12s %-10s %-15s %s\n", 
               "--", "----", "------", "----", "------", "--", "-------");
        
        for (size_t i = 0; i < droplets->count; i++) {
            const do_droplet_t *droplet = &droplets->items[i];
            char created_str[32];
            format_time(droplet->created_at, created_str, sizeof(created_str));
            
            printf("%-8u %-20s %-10s %-12s %-10s %-15s %s\n",
                   droplet->id,
                   droplet->name ? droplet->name : "N/A",
                   droplet->status ? droplet->status : "N/A",
                   droplet->size_slug ? droplet->size_slug : "N/A",
                   droplet->region && droplet->region->slug ? droplet->region->slug : "N/A",
                   get_public_ip(droplet),
                   created_str);
        }
    }
    
    do_droplet_list_free(droplets);
    do_client_free(client);
    return 0;
}

int cmd_droplets_get(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: droplets-get <id>\n");
        return 1;
    }
    
    uint32_t id = (uint32_t)atoi(argv[1]);
    if (id == 0) {
        fprintf(stderr, "Invalid droplet ID: %s\n", argv[1]);
        return 1;
    }
    
    do_client_t *client = do_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }
    
    do_result_t result = do_client_init_from_config(client);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    do_droplet_t *droplet;
    result = do_client_get_droplet(client, id, &droplet);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to get droplet: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    // Display droplet details
    printf("Droplet Details:\n");
    printf("  ID: %u\n", droplet->id);
    printf("  Name: %s\n", droplet->name ? droplet->name : "N/A");
    printf("  Status: %s\n", droplet->status ? droplet->status : "N/A");
    printf("  Memory: %u MB\n", droplet->memory);
    printf("  VCPUs: %u\n", droplet->vcpus);
    printf("  Disk: %u GB\n", droplet->disk);
    printf("  Locked: %s\n", droplet->locked ? "true" : "false");
    
    char created_str[64];
    struct tm *tm_info = gmtime(&droplet->created_at);
    strftime(created_str, sizeof(created_str), "%Y-%m-%d %H:%M:%S UTC", tm_info);
    printf("  Created: %s\n", created_str);
    
    if (droplet->image) {
        printf("  Image: %s (%s)\n", 
               droplet->image->name ? droplet->image->name : "N/A",
               droplet->image->distribution ? droplet->image->distribution : "N/A");
    }
    
    if (droplet->size) {
        printf("  Size: %s ($%.2f/month)\n", 
               droplet->size->slug ? droplet->size->slug : "N/A",
               droplet->size->price_monthly);
    }
    
    if (droplet->region) {
        printf("  Region: %s (%s)\n", 
               droplet->region->name ? droplet->region->name : "N/A",
               droplet->region->slug ? droplet->region->slug : "N/A");
    }
    
    if (droplet->networks) {
        printf("  Networks:\n");
        for (size_t i = 0; i < droplet->networks->v4_count; i++) {
            printf("    %s: %s\n", 
                   droplet->networks->v4[i].type ? droplet->networks->v4[i].type : "unknown",
                   droplet->networks->v4[i].ip_address ? droplet->networks->v4[i].ip_address : "N/A");
        }
        for (size_t i = 0; i < droplet->networks->v6_count; i++) {
            printf("    %s (v6): %s\n", 
                   droplet->networks->v6[i].type ? droplet->networks->v6[i].type : "unknown",
                   droplet->networks->v6[i].ip_address ? droplet->networks->v6[i].ip_address : "N/A");
        }
    }
    
    if (droplet->tags.count > 0) {
        printf("  Tags: ");
        for (size_t i = 0; i < droplet->tags.count; i++) {
            printf("%s", droplet->tags.items[i]);
            if (i < droplet->tags.count - 1) printf(", ");
        }
        printf("\n");
    }
    
    do_droplet_free(droplet);
    do_client_free(client);
    return 0;
}

int cmd_droplets_create(int argc, char **argv) {
    char *name = NULL;
    char *region = NULL;
    char *size = NULL;
    char *image = NULL;
    
    static struct option long_options[] = {
        {"name", required_argument, 0, 'n'},
        {"region", required_argument, 0, 'r'},
        {"size", required_argument, 0, 's'},
        {"image", required_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int c;
    while ((c = getopt_long(argc, argv, "n:r:s:i:h", long_options, NULL)) != -1) {
        switch (c) {
            case 'n':
                name = optarg;
                break;
            case 'r':
                region = optarg;
                break;
            case 's':
                size = optarg;
                break;
            case 'i':
                image = optarg;
                break;
            case 'h':
                printf("Usage: droplets-create --name NAME --region REGION --size SIZE --image IMAGE\n");
                return 0;
            default:
                fprintf(stderr, "Use --help for usage information\n");
                return 1;
        }
    }
    
    if (!name || !region || !size || !image) {
        fprintf(stderr, "Required options: --name, --region, --size, --image\n");
        return 1;
    }
    
    do_client_t *client = do_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }
    
    do_result_t result = do_client_init_from_config(client);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    do_create_droplet_request_t request = {0};
    request.name = name;
    request.region = region;
    request.size = size;
    request.image = image;
    do_string_array_init(&request.tags);
    do_string_array_init(&request.ssh_keys);
    do_string_array_init(&request.volumes);
    
    do_droplet_t *droplet;
    result = do_client_create_droplet(client, &request, &droplet);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to create droplet: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    printf("Droplet created successfully!\n");
    printf("  ID: %u\n", droplet->id);
    printf("  Name: %s\n", droplet->name ? droplet->name : "N/A");
    printf("  Status: %s\n", droplet->status ? droplet->status : "N/A");
    printf("  Region: %s\n", region);
    printf("  Size: %s\n", size);
    
    do_droplet_free(droplet);
    do_client_free(client);
    return 0;
}

int cmd_droplets_delete(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: droplets-delete <id>\n");
        return 1;
    }
    
    uint32_t id = (uint32_t)atoi(argv[1]);
    if (id == 0) {
        fprintf(stderr, "Invalid droplet ID: %s\n", argv[1]);
        return 1;
    }
    
    printf("Are you sure you want to delete droplet %u? (y/N): ", id);
    char response[10];
    if (!fgets(response, sizeof(response), stdin)) {
        printf("Cancelled\n");
        return 0;
    }
    
    if (response[0] != 'y' && response[0] != 'Y') {
        printf("Cancelled\n");
        return 0;
    }
    
    do_client_t *client = do_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client\n");
        return 1;
    }
    
    do_result_t result = do_client_init_from_config(client);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    result = do_client_delete_droplet(client, id);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to delete droplet: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    printf("Droplet %u deleted successfully\n", id);
    do_client_free(client);
    return 0;
}