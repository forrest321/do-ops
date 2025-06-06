import Foundation

public struct DigitalOceanConfig {
    public let apiToken: String
    public let baseURL: URL
    public let userAgent: String
    public let timeout: TimeInterval
    
    public init(
        apiToken: String,
        baseURL: URL = URL(string: "https://api.digitalocean.com/v2")!,
        userAgent: String = "DigitalOceanKit/1.0.0 Swift",
        timeout: TimeInterval = 30.0
    ) {
        self.apiToken = apiToken
        self.baseURL = baseURL
        self.userAgent = userAgent
        self.timeout = timeout
    }
    
    public static func fromEnvironment() -> DigitalOceanConfig? {
        guard let token = ProcessInfo.processInfo.environment["DIGITALOCEAN_ACCESS_TOKEN"] else {
            return nil
        }
        
        let baseURL = ProcessInfo.processInfo.environment["DIGITALOCEAN_API_URL"]
            .flatMap { URL(string: $0) } ?? URL(string: "https://api.digitalocean.com/v2")!
        
        return DigitalOceanConfig(
            apiToken: token,
            baseURL: baseURL
        )
    }
}