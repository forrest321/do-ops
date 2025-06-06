# DigitalOcean C Client

A C client library and tools for the DigitalOcean API v2.0.

## Architecture

This implementation follows a modular C library architecture:

```
c/
├── include/
│   └── digitalocean/
│       ├── client.h       # Main client API
│       ├── config.h       # Configuration management
│       ├── types.h        # Data structures
│       └── http.h         # HTTP utilities
├── src/
│   ├── client.c           # Core client implementation
│   ├── config.c           # Config file handling
│   ├── http.c             # HTTP request handling
│   ├── json.c             # JSON parsing utilities
│   └── cli/               # CLI application
│       ├── main.c         # CLI entry point
│       ├── account.c      # Account commands
│       ├── droplets.c     # Droplet commands
│       └── config.c       # Config commands
├── examples/              # Usage examples
├── tests/                 # Unit tests
├── CMakeLists.txt         # CMake build system
└── Makefile              # Alternative make build
```

## Core Components

- **Client Library** (`libdigitalocean`): HTTP client with libcurl
- **Types** (`do_types.h`): C structs for API objects
- **CLI** (`do-cli`): Command-line interface with getopt
- **Configuration** (`do_config`): INI-style config file support

## Features

- **POSIX Compliance**: Works on Linux, macOS, and other Unix systems
- **libcurl HTTP**: Robust HTTP client with SSL support
- **cJSON Parsing**: Fast JSON parsing and generation
- **Memory Management**: Proper allocation/deallocation patterns
- **Error Handling**: Comprehensive error codes and messages

## Getting Started

### Prerequisites

- GCC or Clang compiler
- CMake 3.10+ or Make
- libcurl development headers
- cJSON library
- DigitalOcean API token

### Dependencies (Ubuntu/Debian)

```bash
sudo apt-get install build-essential cmake libcurl4-openssl-dev libcjson-dev
```

### Dependencies (macOS)

```bash
brew install cmake curl cjson
```

### Building

#### With CMake (Recommended)

```bash
mkdir build && cd build
cmake ..
make
```

#### With Make

```bash
make
```

### Installation

```bash
sudo make install
```

### Configuration

```bash
# Set API token
export DIGITALOCEAN_TOKEN="your_token_here"

# Or use config file
do-cli config set token your_token_here
```

### Usage

```bash
# List droplets
do-cli droplets list

# Create droplet
do-cli droplets create --name my-droplet --region nyc1 --size s-1vcpu-1gb --image ubuntu-22-04-x64

# Get account info
do-cli account info
```

## Library Usage

```c
#include <digitalocean/client.h>

int main() {
    // Initialize client
    do_client_t *client = do_client_new();
    do_config_t *config = do_config_load();
    
    if (do_client_init(client, config) != DO_SUCCESS) {
        fprintf(stderr, "Failed to initialize client\n");
        return 1;
    }

    // List droplets
    do_droplet_list_t *droplets;
    if (do_client_list_droplets(client, &droplets) == DO_SUCCESS) {
        for (size_t i = 0; i < droplets->count; i++) {
            printf("Droplet: %s (ID: %u)\n", 
                   droplets->items[i].name, 
                   droplets->items[i].id);
        }
        do_droplet_list_free(droplets);
    }

    // Cleanup
    do_client_free(client);
    do_config_free(config);
    return 0;
}
```

## Development

```bash
# Build with debug symbols
make debug

# Run tests
make test

# Clean build artifacts
make clean
```