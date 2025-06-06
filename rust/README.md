# DigitalOcean Rust Client

A Rust client library and tools for the DigitalOcean API v2.0.

## Architecture

This implementation follows a modular workspace architecture supporting multiple frontends:

```
rust/
├── crates/
│   ├── do-client/    # Core API client library
│   ├── do-types/     # Type definitions and models
│   └── do-cli/       # CLI frontend
├── apps/
│   ├── tui/          # Terminal UI frontend (future)
│   └── desktop/      # Tauri desktop app (future)
└── examples/         # Usage examples
```

## Core Components

- **Client Library** (`do-client`): HTTP client with async/await and type safety
- **Types** (`do-types`): Rust structs generated from OpenAPI schema
- **CLI** (`do-cli`): Command-line interface built with Clap

## Features

- **Async/Await**: Built on tokio with reqwest for HTTP
- **Type Safety**: Strong typing with serde for JSON serialization
- **Error Handling**: Comprehensive error types with context
- **Configuration**: TOML-based config with environment variable support
- **CLI**: Rich command-line interface with subcommands

## Getting Started

### Prerequisites

- Rust 1.70+
- DigitalOcean API token

### Installation

```bash
# Install CLI
cargo install do-cli

# Or build from source
cargo build --release
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

```rust
use do_client::{Client, Config};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let config = Config::from_env()?;
    let client = Client::new(config);

    // List droplets
    let droplets = client.droplets().list().await?;

    // Create droplet
    let new_droplet = client.droplets().create()
        .name("my-droplet")
        .region("nyc1")
        .size("s-1vcpu-1gb")
        .image("ubuntu-22-04-x64")
        .send()
        .await?;

    Ok(())
}
```

## Development

This project uses a Cargo workspace for better code organization and sharing between crates.