# DigitalOcean Ruby Client

A Ruby client library and tools for the DigitalOcean API v2.0.

## Architecture

This implementation follows a modular gem architecture supporting multiple frontends:

```
ruby/
├── lib/
│   ├── digitalocean/
│   │   ├── client.rb      # Core API client
│   │   ├── config.rb      # Configuration management
│   │   ├── models/        # API models and types
│   │   └── cli/           # CLI commands
│   └── digitalocean.rb    # Main entry point
├── bin/
│   └── do-cli            # CLI executable
├── spec/                 # RSpec tests
└── examples/             # Usage examples
```

## Core Components

- **Client Library** (`DigitalOcean::Client`): HTTP client with Ruby conventions
- **Models** (`DigitalOcean::Models`): Ruby classes for API objects
- **CLI** (`DigitalOcean::CLI`): Command-line interface built with Thor
- **Configuration** (`DigitalOcean::Config`): YAML-based config with ENV support

## Features

- **Ruby Conventions**: Idiomatic Ruby with snake_case and proper modules
- **HTTP Client**: Built on Faraday with JSON middleware
- **Error Handling**: Custom exception classes with detailed messages
- **Configuration**: YAML config files with environment variable overrides
- **CLI**: Rich command-line interface with colorized output

## Getting Started

### Prerequisites

- Ruby 3.0+
- Bundler
- DigitalOcean API token

### Installation

```bash
# Install gem
gem install digitalocean-cli

# Or add to Gemfile
gem 'digitalocean-client'
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

```ruby
require 'digitalocean'

# Initialize client
client = DigitalOcean::Client.new(token: ENV['DIGITALOCEAN_TOKEN'])

# List droplets
droplets = client.droplets.list

# Create droplet
new_droplet = client.droplets.create(
  name: 'my-droplet',
  region: 'nyc1',
  size: 's-1vcpu-1gb',
  image: 'ubuntu-22-04-x64'
)

# Get account info
account = client.account.info
```

## Development

```bash
# Install dependencies
bundle install

# Run tests
bundle exec rspec

# Build gem
gem build digitalocean.gemspec
```