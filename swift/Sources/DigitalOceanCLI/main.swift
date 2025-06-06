import ArgumentParser
import DigitalOceanKit
import Foundation

@main
struct DigitalOceanCLI: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        commandName: "doctl-swift",
        abstract: "DigitalOcean command line interface written in Swift",
        version: "1.0.0",
        subcommands: [
            Account.self,
            Droplets.self,
            Images.self,
            Regions.self,
            Sizes.self
        ]
    )
}

// MARK: - Account Commands

struct Account: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        abstract: "Account management commands"
    )
    
    @Option(name: .long, help: "DigitalOcean API token")
    var token: String?
    
    func run() async throws {
        let client = try createClient()
        let account = try await client.getAccount()
        
        print("Account Information:")
        print("Email: \(account.email)")
        print("UUID: \(account.uuid)")
        print("Status: \(account.status)")
        print("Email Verified: \(account.emailVerified)")
        
        if let team = account.team {
            print("Team: \(team.name) (\(team.uuid))")
        }
    }
    
    private func createClient() throws -> DigitalOceanClient {
        if let token = token {
            return DigitalOceanKit.createClient(apiToken: token)
        } else {
            return try DigitalOceanKit.createClientFromEnvironment()
        }
    }
}

// MARK: - Droplet Commands

struct Droplets: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        abstract: "Droplet management commands",
        subcommands: [
            List.self,
            Get.self,
            Create.self,
            Delete.self,
            Reboot.self,
            PowerOff.self,
            PowerOn.self
        ]
    )
    
    struct List: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "List all droplets"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Option(name: .long, help: "Filter by tag")
        var tag: String?
        
        @Flag(name: .long, help: "Output in JSON format")
        var json = false
        
        func run() async throws {
            let client = try createClient()
            let result = try await client.listDroplets(tag: tag)
            
            if json {
                let encoder = JSONEncoder()
                encoder.outputFormatting = .prettyPrinted
                encoder.dateEncodingStrategy = .iso8601
                let data = try encoder.encode(result.droplets)
                print(String(data: data, encoding: .utf8)!)
            } else {
                print(String(format: "%-12s %-20s %-12s %-10s %-15s", "ID", "Name", "Status", "Region", "IP Address"))
                print(String(repeating: "-", count: 80))
                
                for droplet in result.droplets {
                    let ip = droplet.networks.v4.first?.ipAddress ?? "N/A"
                    print(String(format: "%-12d %-20s %-12s %-10s %-15s",
                                droplet.id,
                                String(droplet.name.prefix(20)),
                                droplet.status,
                                droplet.region.slug,
                                ip))
                }
                
                if let meta = result.meta, let total = meta.total {
                    print("\nTotal: \(total) droplets")
                }
            }
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct Get: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Get details of a specific droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet ID")
        var id: Int
        
        @Flag(name: .long, help: "Output in JSON format")
        var json = false
        
        func run() async throws {
            let client = try createClient()
            let droplet = try await client.getDroplet(id: id)
            
            if json {
                let encoder = JSONEncoder()
                encoder.outputFormatting = .prettyPrinted
                encoder.dateEncodingStrategy = .iso8601
                let data = try encoder.encode(droplet)
                print(String(data: data, encoding: .utf8)!)
            } else {
                print("Droplet Details:")
                print("ID: \(droplet.id)")
                print("Name: \(droplet.name)")
                print("Status: \(droplet.status)")
                print("Memory: \(droplet.memory) MB")
                print("vCPUs: \(droplet.vcpus)")
                print("Disk: \(droplet.disk) GB")
                print("Region: \(droplet.region.name) (\(droplet.region.slug))")
                print("Image: \(droplet.image.name)")
                print("Size: \(droplet.size.slug)")
                print("Created: \(ISO8601DateFormatter().string(from: droplet.createdAt))")
                
                if !droplet.networks.v4.isEmpty {
                    print("IPv4 Addresses:")
                    for network in droplet.networks.v4 {
                        print("  \(network.ipAddress) (\(network.type))")
                    }
                }
                
                if !droplet.tags.isEmpty {
                    print("Tags: \(droplet.tags.joined(separator: ", "))")
                }
            }
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct Create: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Create a new droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet name")
        var name: String
        
        @Option(name: .long, help: "Region slug")
        var region: String
        
        @Option(name: .long, help: "Size slug")
        var size: String
        
        @Option(name: .long, help: "Image slug or ID")
        var image: String
        
        @Flag(name: .long, help: "Enable backups")
        var backups = false
        
        @Flag(name: .long, help: "Enable IPv6")
        var ipv6 = false
        
        @Flag(name: .long, help: "Enable monitoring")
        var monitoring = false
        
        func run() async throws {
            let client = try createClient()
            
            let imageValue: CreateDropletImage
            if let imageId = Int(image) {
                imageValue = .id(imageId)
            } else {
                imageValue = .slug(image)
            }
            
            let request = CreateDropletRequest(
                name: name,
                region: region,
                size: size,
                image: imageValue,
                backups: backups,
                ipv6: ipv6,
                monitoring: monitoring
            )
            
            let droplet = try await client.createDroplet(request)
            
            print("Droplet created successfully!")
            print("ID: \(droplet.id)")
            print("Name: \(droplet.name)")
            print("Status: \(droplet.status)")
            print("Region: \(droplet.region.slug)")
            print("Size: \(droplet.size.slug)")
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct Delete: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Delete a droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet ID")
        var id: Int
        
        @Flag(name: .long, help: "Force deletion without confirmation")
        var force = false
        
        func run() async throws {
            let client = try createClient()
            
            if !force {
                print("Are you sure you want to delete droplet \(id)? (y/N): ", terminator: "")
                guard let response = readLine()?.lowercased(), response == "y" || response == "yes" else {
                    print("Deletion cancelled.")
                    return
                }
            }
            
            try await client.deleteDroplet(id: id)
            print("Droplet \(id) deleted successfully.")
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct Reboot: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Reboot a droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet ID")
        var id: Int
        
        func run() async throws {
            let client = try createClient()
            try await client.rebootDroplet(id: id)
            print("Droplet \(id) reboot initiated.")
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct PowerOff: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Power off a droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet ID")
        var id: Int
        
        func run() async throws {
            let client = try createClient()
            try await client.powerOffDroplet(id: id)
            print("Droplet \(id) power off initiated.")
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
    
    struct PowerOn: AsyncParsableCommand {
        static let configuration = CommandConfiguration(
            abstract: "Power on a droplet"
        )
        
        @Option(name: .long, help: "DigitalOcean API token")
        var token: String?
        
        @Argument(help: "Droplet ID")
        var id: Int
        
        func run() async throws {
            let client = try createClient()
            try await client.powerOnDroplet(id: id)
            print("Droplet \(id) power on initiated.")
        }
        
        private func createClient() throws -> DigitalOceanClient {
            if let token = token {
                return DigitalOceanKit.createClient(apiToken: token)
            } else {
                return try DigitalOceanKit.createClientFromEnvironment()
            }
        }
    }
}

// MARK: - Image Commands

struct Images: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        abstract: "List available images"
    )
    
    @Option(name: .long, help: "DigitalOcean API token")
    var token: String?
    
    @Option(name: .long, help: "Image type filter (distribution, application)")
    var type: String?
    
    @Flag(name: .long, help: "Show private images only")
    var `private` = false
    
    @Flag(name: .long, help: "Output in JSON format")
    var json = false
    
    func run() async throws {
        let client = try createClient()
        let images = try await client.listImages(type: type, private: `private`)
        
        if json {
            let encoder = JSONEncoder()
            encoder.outputFormatting = .prettyPrinted
            encoder.dateEncodingStrategy = .iso8601
            let data = try encoder.encode(images)
            print(String(data: data, encoding: .utf8)!)
        } else {
            print(String(format: "%-12s %-25s %-15s %-10s", "ID", "Name", "Distribution", "Type"))
            print(String(repeating: "-", count: 70))
            
            for image in images {
                print(String(format: "%-12d %-25s %-15s %-10s",
                            image.id,
                            String(image.name.prefix(25)),
                            String(image.distribution.prefix(15)),
                            image.type))
            }
            
            print("\nTotal: \(images.count) images")
        }
    }
    
    private func createClient() throws -> DigitalOceanClient {
        if let token = token {
            return DigitalOceanKit.createClient(apiToken: token)
        } else {
            return try DigitalOceanKit.createClientFromEnvironment()
        }
    }
}

// MARK: - Region Commands

struct Regions: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        abstract: "List available regions"
    )
    
    @Option(name: .long, help: "DigitalOcean API token")
    var token: String?
    
    @Flag(name: .long, help: "Output in JSON format")
    var json = false
    
    func run() async throws {
        let client = try createClient()
        let regions = try await client.listRegions()
        
        if json {
            let encoder = JSONEncoder()
            encoder.outputFormatting = .prettyPrinted
            let data = try encoder.encode(regions)
            print(String(data: data, encoding: .utf8)!)
        } else {
            print(String(format: "%-12s %-25s %-10s", "Slug", "Name", "Available"))
            print(String(repeating: "-", count: 50))
            
            for region in regions {
                print(String(format: "%-12s %-25s %-10s",
                            region.slug,
                            String(region.name.prefix(25)),
                            region.available ? "Yes" : "No"))
            }
            
            print("\nTotal: \(regions.count) regions")
        }
    }
    
    private func createClient() throws -> DigitalOceanClient {
        if let token = token {
            return DigitalOceanKit.createClient(apiToken: token)
        } else {
            return try DigitalOceanKit.createClientFromEnvironment()
        }
    }
}

// MARK: - Size Commands

struct Sizes: AsyncParsableCommand {
    static let configuration = CommandConfiguration(
        abstract: "List available sizes"
    )
    
    @Option(name: .long, help: "DigitalOcean API token")
    var token: String?
    
    @Flag(name: .long, help: "Output in JSON format")
    var json = false
    
    func run() async throws {
        let client = try createClient()
        let sizes = try await client.listSizes()
        
        if json {
            let encoder = JSONEncoder()
            encoder.outputFormatting = .prettyPrinted
            let data = try encoder.encode(sizes)
            print(String(data: data, encoding: .utf8)!)
        } else {
            print(String(format: "%-15s %-8s %-6s %-8s %-12s %-12s", "Slug", "Memory", "vCPUs", "Disk", "Price/Mo", "Price/Hr"))
            print(String(repeating: "-", count: 80))
            
            for size in sizes.filter({ $0.available }) {
                print(String(format: "%-15s %-8d %-6d %-8d $%-11.2f $%-11.4f",
                            size.slug,
                            size.memory,
                            size.vcpus,
                            size.disk,
                            size.priceMonthly,
                            size.priceHourly))
            }
            
            print("\nTotal: \(sizes.filter { $0.available }.count) available sizes")
        }
    }
    
    private func createClient() throws -> DigitalOceanClient {
        if let token = token {
            return DigitalOceanKit.createClient(apiToken: token)
        } else {
            return try DigitalOceanKit.createClientFromEnvironment()
        }
    }
}