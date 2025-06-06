# DigitalOcean Swift SDK

A modern Swift SDK and CLI for the DigitalOcean API v2, providing both a powerful client library and a command-line interface.

## Features

- **Modern Swift**: Built with Swift 5.9+, using async/await and structured concurrency
- **Type Safety**: Comprehensive type definitions for all DigitalOcean API resources
- **Cross-Platform**: Supports macOS, iOS, watchOS, and tvOS
- **CLI Tool**: Full-featured command-line interface
- **Environment Configuration**: Automatic configuration from environment variables

## Installation

### Swift Package Manager

Add the following to your `Package.swift`:

```swift
dependencies: [
    .package(url: "https://github.com/your-org/digitalocean-swift", from: "1.0.0")
]
```

### Building from Source

```bash
git clone https://github.com/your-org/digitalocean-swift
cd digitalocean-swift
swift build
```

## Quick Start

### Library Usage

```swift
import DigitalOceanKit

// Create client from environment variable DIGITALOCEAN_ACCESS_TOKEN
let client = try DigitalOceanKit.createClientFromEnvironment()

// Or create with explicit token
let client = DigitalOceanKit.createClient(apiToken: "your_token_here")

// Get account information
let account = try await client.getAccount()
print("Account: \(account.email)")

// List droplets
let (droplets, meta) = try await client.listDroplets()
for droplet in droplets {
    print("\(droplet.name): \(droplet.status)")
}

// Create a new droplet
let request = CreateDropletRequest(
    name: "my-swift-droplet",
    region: "nyc3",
    size: "s-1vcpu-1gb",
    image: .slug("ubuntu-22-04-x64")
)
let newDroplet = try await client.createDroplet(request)
```

### CLI Usage

Set your DigitalOcean token:
```bash
export DIGITALOCEAN_ACCESS_TOKEN="your_token_here"
```

Build and run the CLI:
```bash
swift run doctl-swift --help
```

#### CLI Commands

```bash
# Account information
swift run doctl-swift account

# List droplets
swift run doctl-swift droplets list

# Get droplet details
swift run doctl-swift droplets get 12345

# Create a droplet
swift run doctl-swift droplets create my-droplet \
  --region nyc3 \
  --size s-1vcpu-1gb \
  --image ubuntu-22-04-x64 \
  --monitoring

# Droplet actions
swift run doctl-swift droplets reboot 12345
swift run doctl-swift droplets power-off 12345
swift run doctl-swift droplets power-on 12345

# Delete droplet
swift run doctl-swift droplets delete 12345

# List regions, sizes, and images
swift run doctl-swift regions
swift run doctl-swift sizes
swift run doctl-swift images
```

## API Coverage

The SDK currently supports:

- **Account**: Get account information
- **Droplets**: List, get, create, delete, and manage droplets
- **Regions**: List available regions
- **Sizes**: List available droplet sizes
- **Images**: List available images

## Configuration

### Environment Variables

- `DIGITALOCEAN_ACCESS_TOKEN`: Your DigitalOcean API token (required)
- `DIGITALOCEAN_API_URL`: Custom API URL (optional, defaults to https://api.digitalocean.com/v2)

### Programmatic Configuration

```swift
let config = DigitalOceanConfig(
    apiToken: "your_token",
    baseURL: URL(string: "https://api.digitalocean.com/v2")!,
    userAgent: "MyApp/1.0.0",
    timeout: 30.0
)
let client = DigitalOceanClient(config: config)
```

## Error Handling

The SDK provides structured error handling:

```swift
do {
    let droplet = try await client.getDroplet(id: 12345)
} catch let error as DOError {
    print("DigitalOcean API Error: \(error.message)")
} catch {
    print("Network or other error: \(error)")
}
```

## Examples

See the `Examples/` directory for complete usage examples:

- `basic_usage.swift`: Demonstrates core functionality

## Requirements

- Swift 5.9+
- macOS 12.0+ / iOS 15.0+ / watchOS 8.0+ / tvOS 15.0+

## Dependencies

- [swift-argument-parser](https://github.com/apple/swift-argument-parser): For CLI argument parsing
- [swift-http-types](https://github.com/apple/swift-http-types): For modern HTTP handling

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Related Projects

This Swift SDK is part of a multi-language DigitalOcean API client collection:

- [Go SDK](../go/)
- [Rust SDK](../rust/)
- [JavaScript/TypeScript SDK](../js/)
- [Ruby SDK](../ruby/)
- [C SDK](../c/)