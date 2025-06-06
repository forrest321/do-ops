# DigitalOcean JavaScript/TypeScript Client

A JavaScript/TypeScript client library and tools for the DigitalOcean API v2.0.

## Architecture

This implementation follows a modular architecture supporting multiple frontends:

```
js/
├── packages/
│   ├── client/       # Core API client library
│   ├── types/        # TypeScript type definitions
│   └── cli/          # CLI frontend
├── apps/
│   ├── web/          # Web frontend (future)
│   └── desktop/      # Electron app (future)
└── tools/
    └── build/        # Build and development tools
```

## Core Components

- **Client Library** (`packages/client`): HTTP client with type-safe API operations
- **Types** (`packages/types`): TypeScript definitions generated from OpenAPI schema
- **CLI** (`packages/cli`): Command-line interface built with Commander.js

## Getting Started

### Prerequisites

- Node.js 18+
- npm or yarn
- DigitalOcean API token

### Installation

```bash
# Install CLI globally
npm install -g @do-op/cli

# Or use locally
npm install @do-op/client @do-op/types
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

```typescript
import { DigitalOceanClient } from '@do-op/client';

const client = new DigitalOceanClient({
  token: process.env.DIGITALOCEAN_TOKEN
});

// List droplets
const droplets = await client.droplets.list();

// Create droplet
const newDroplet = await client.droplets.create({
  name: 'my-droplet',
  region: 'nyc1',
  size: 's-1vcpu-1gb',
  image: 'ubuntu-22-04-x64'
});
```

## Development

This project uses a monorepo structure with workspaces for better code organization and sharing between packages.