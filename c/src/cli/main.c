#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "digitalocean/client.h"

// Command function declarations
int cmd_account_info(int argc, char **argv);
int cmd_droplets_list(int argc, char **argv);
int cmd_droplets_get(int argc, char **argv);
int cmd_droplets_create(int argc, char **argv);
int cmd_droplets_delete(int argc, char **argv);
int cmd_config_set(int argc, char **argv);
int cmd_config_get(int argc, char **argv);

typedef struct {
    const char *name;
    int (*handler)(int argc, char **argv);
    const char *description;
} command_t;

static const command_t commands[] = {
    {"account-info", cmd_account_info, "Show account information"},
    {"droplets-list", cmd_droplets_list, "List all droplets"},
    {"droplets-get", cmd_droplets_get, "Get droplet details"},
    {"droplets-create", cmd_droplets_create, "Create a new droplet"},
    {"droplets-delete", cmd_droplets_delete, "Delete a droplet"},
    {"config-set", cmd_config_set, "Set configuration value"},
    {"config-get", cmd_config_get, "Get configuration value"},
    {NULL, NULL, NULL}
};

static void print_usage(const char *program_name) {
    printf("Usage: %s <command> [options]\n\n", program_name);
    printf("Commands:\n");
    
    for (int i = 0; commands[i].name; i++) {
        printf("  %-20s %s\n", commands[i].name, commands[i].description);
    }
    
    printf("\nEnvironment Variables:\n");
    printf("  DIGITALOCEAN_TOKEN    API authentication token\n");
    printf("  DIGITALOCEAN_BASE_URL Base URL for API (default: %s)\n", DO_DEFAULT_BASE_URL);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Initialize libcurl
    if (do_library_init() != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize HTTP library\n");
        return 1;
    }
    
    const char *command = argv[1];
    
    // Find and execute command
    for (int i = 0; commands[i].name; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            int result = commands[i].handler(argc - 1, argv + 1);
            do_library_cleanup();
            return result;
        }
    }
    
    // Command not found
    fprintf(stderr, "Unknown command: %s\n", command);
    print_usage(argv[0]);
    do_library_cleanup();
    return 1;
}