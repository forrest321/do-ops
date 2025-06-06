import Foundation

// Re-export public types for convenience
@_exported import struct Foundation.URL
@_exported import struct Foundation.Data
@_exported import struct Foundation.Date

// Main module entry point
public enum DigitalOceanKit {
    public static let version = "1.0.0"
    
    public static func createClient(apiToken: String) -> DigitalOceanClient {
        let config = DigitalOceanConfig(apiToken: apiToken)
        return DigitalOceanClient(config: config)
    }
    
    public static func createClientFromEnvironment() throws -> DigitalOceanClient {
        guard let config = DigitalOceanConfig.fromEnvironment() else {
            throw DOError(
                id: "missing_token",
                message: "DIGITALOCEAN_ACCESS_TOKEN environment variable not set",
                requestId: nil
            )
        }
        return DigitalOceanClient(config: config)
    }
}