# DigitalOcean Go Client

A Go client library and tools for the DigitalOcean API v2.0.

## Architecture

This implementation follows a modular architecture supporting multiple frontends:

```
go/
├── cmd/
│   ├── cli/          # CLI frontend
│   ├── tui/          # TUI frontend (future)
│   └── web/          # Web frontend (future)
├── pkg/
│   ├── client/       # Core API client library
│   ├── config/       # Configuration management
│   └── types/        # Generated API types
└── internal/
    └── auth/         # Authentication handling
```

## Core Components

- **Client Library** (`pkg/client`): HTTP client with type-safe API operations
- **Configuration** (`pkg/config`): Token management and settings
- **Types** (`pkg/types`): Generated from OpenAPI schema
- **CLI** (`cmd/cli`): Command-line interface

## Getting Started

### Prerequisites

- Go 1.21+
- DigitalOcean API token

### Installation

```bash
go install ./cmd/cli
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
do-cli droplets create --name my-droplet --region nyc1 --size s-1vcpu-1gb

# Get account info
do-cli account info
```

## Development

Focus areas based on API analysis:
1. **Core Infrastructure**: Droplets, Images, Volumes (primary use cases)
2. **Networking**: Load Balancers, Firewalls, Floating IPs  
3. **Platform Services**: Apps, Databases, Kubernetes
4. **Account Management**: Billing, Projects, SSH Keys