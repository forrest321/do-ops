import Foundation
import DigitalOceanKit

// Example: Basic usage of DigitalOceanKit

@main
struct BasicUsageExample {
    static func main() async {
        do {
            // Create client from environment variable
            let client = try DigitalOceanKit.createClientFromEnvironment()
            
            // Or create client with explicit token
            // let client = DigitalOceanKit.createClient(apiToken: "your_token_here")
            
            // Get account information
            print("Fetching account information...")
            let account = try await client.getAccount()
            print("Account: \(account.email) (\(account.status))")
            
            // List droplets
            print("\nFetching droplets...")
            let (droplets, meta) = try await client.listDroplets(perPage: 10)
            print("Found \(droplets.count) droplets")
            
            for droplet in droplets {
                let ip = droplet.networks.v4.first?.ipAddress ?? "N/A"
                print("- \(droplet.name) (\(droplet.id)): \(droplet.status) - \(ip)")
            }
            
            // List available regions
            print("\nFetching regions...")
            let regions = try await client.listRegions()
            let availableRegions = regions.filter { $0.available }
            print("Available regions: \(availableRegions.map { $0.slug }.joined(separator: ", "))")
            
            // List available sizes
            print("\nFetching sizes...")
            let sizes = try await client.listSizes()
            let availableSizes = sizes.filter { $0.available }.prefix(5)
            print("Sample sizes:")
            for size in availableSizes {
                print("- \(size.slug): \(size.memory)MB RAM, \(size.vcpus) vCPUs, $\(size.priceMonthly)/mo")
            }
            
            // List images
            print("\nFetching distribution images...")
            let images = try await client.listImages(type: "distribution")
            let sampleImages = images.prefix(5)
            print("Sample distribution images:")
            for image in sampleImages {
                print("- \(image.slug ?? String(image.id)): \(image.name)")
            }
            
            // Example: Create a new droplet (commented out for safety)
            /*
            print("\nCreating a new droplet...")
            let createRequest = CreateDropletRequest(
                name: "swift-example-droplet",
                region: "nyc3",
                size: "s-1vcpu-1gb",
                image: .slug("ubuntu-22-04-x64"),
                monitoring: true
            )
            
            let newDroplet = try await client.createDroplet(createRequest)
            print("Created droplet: \(newDroplet.name) (\(newDroplet.id))")
            
            // Get specific droplet details
            let dropletDetails = try await client.getDroplet(id: newDroplet.id)
            print("Droplet status: \(dropletDetails.status)")
            */
            
        } catch {
            print("Error: \(error)")
            if let doError = error as? DOError {
                print("DigitalOcean Error: \(doError.message)")
            }
        }
    }
}