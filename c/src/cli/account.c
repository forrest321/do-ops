#include <stdio.h>
#include <stdlib.h>
#include "digitalocean/client.h"

int cmd_account_info(int argc, char **argv) {
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
    
    do_account_t *account;
    result = do_client_get_account(client, &account);
    if (result != DO_SUCCESS) {
        fprintf(stderr, "Failed to get account: %s\n", do_client_get_error_string(result));
        do_client_free(client);
        return 1;
    }
    
    // Display account information
    printf("Account Information:\n");
    printf("  Email: %s\n", account->email ? account->email : "N/A");
    printf("  UUID: %s\n", account->uuid ? account->uuid : "N/A");
    printf("  Status: %s\n", account->status ? account->status : "N/A");
    printf("  Email Verified: %s\n", account->email_verified ? "true" : "false");
    printf("  Droplet Limit: %u\n", account->droplet_limit);
    printf("  Floating IP Limit: %u\n", account->floating_ip_limit);
    printf("  Volume Limit: %u\n", account->volume_limit);
    
    if (account->team) {
        printf("  Team: %s (%s)\n", 
               account->team->name ? account->team->name : "N/A",
               account->team->uuid ? account->team->uuid : "N/A");
    }
    
    do_account_free(account);
    do_client_free(client);
    return 0;
}