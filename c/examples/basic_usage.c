#include <stdio.h>
#include <stdlib.h>
#include "digitalocean/client.h"

int main() {
    printf("=== DigitalOcean C Client Example ===\n\n");
    
    // Initialize the library
    if (do_library_init() != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize HTTP library\n");
        return 1;
    }
    
    // Create and initialize client
    do_client_t *client = do_client_new();
    if (!client) {
        fprintf(stderr, "Failed to create client\n");
        do_library_cleanup();
        return 1;
    }
    
    do_result_t result = do_client_init_from_config(client);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client: %s\n", do_client_get_error_string(result));
        fprintf(stderr, "Make sure DIGITALOCEAN_TOKEN environment variable is set\n");
        do_client_free(client);
        do_library_cleanup();
        return 1;
    }
    
    // Get account information
    printf("=== Account Information ===\n");
    do_account_t *account;
    result = do_client_get_account(client, &account);
    if (result == DO_SUCCESS) {
        printf("Email: %s\n", account->email ? account->email : "N/A");
        printf("UUID: %s\n", account->uuid ? account->uuid : "N/A");
        printf("Status: %s\n", account->status ? account->status : "N/A");
        printf("Droplet Limit: %u\n", account->droplet_limit);
        
        do_account_free(account);
    } else {
        printf("Failed to get account info: %s\n", do_client_get_error_string(result));
    }
    
    printf("\n=== Droplets ===\n");
    
    // List droplets
    do_droplet_list_t *droplets;
    result = do_client_list_droplets(client, &droplets);
    if (result == DO_SUCCESS) {
        if (droplets->count == 0) {
            printf("No droplets found\n");
        } else {
            printf("Found %zu droplets:\n", droplets->count);
            for (size_t i = 0; i < droplets->count; i++) {
                const do_droplet_t *droplet = &droplets->items[i];
                printf("  ID: %u, Name: %s, Status: %s\n",
                       droplet->id,
                       droplet->name ? droplet->name : "N/A",
                       droplet->status ? droplet->status : "N/A");
                
                // Find public IP
                if (droplet->networks) {
                    for (size_t j = 0; j < droplet->networks->v4_count; j++) {
                        if (droplet->networks->v4[j].type &&
                            strcmp(droplet->networks->v4[j].type, "public") == 0) {
                            printf("    IP: %s\n", droplet->networks->v4[j].ip_address);
                            break;
                        }
                    }
                }
                
                if (droplet->region) {
                    printf("    Region: %s\n", droplet->region->slug);
                }
                printf("    Size: %s\n", droplet->size_slug ? droplet->size_slug : "N/A");
            }
        }
        
        do_droplet_list_free(droplets);
    } else {
        printf("Failed to list droplets: %s\n", do_client_get_error_string(result));
    }
    
    // Example: Create a droplet (commented out to avoid accidental creation)
    /*
    printf("\n=== Creating Droplet ===\n");
    do_create_droplet_request_t request = {0};
    request.name = "example-droplet";
    request.region = "nyc1";
    request.size = "s-1vcpu-1gb";
    request.image = "ubuntu-22-04-x64";
    do_string_array_init(&request.tags);
    do_string_array_add(&request.tags, "example");
    do_string_array_add(&request.tags, "c-client");
    
    do_droplet_t *new_droplet;
    result = do_client_create_droplet(client, &request, &new_droplet);
    if (result == DO_SUCCESS) {
        printf("Created droplet: %s (ID: %u)\n", new_droplet->name, new_droplet->id);
        do_droplet_free(new_droplet);
    } else {
        printf("Failed to create droplet: %s\n", do_client_get_error_string(result));
    }
    
    do_string_array_free(&request.tags);
    */
    
    // Cleanup
    do_client_free(client);
    do_library_cleanup();
    
    printf("\nExample completed successfully!\n");
    return 0;
}