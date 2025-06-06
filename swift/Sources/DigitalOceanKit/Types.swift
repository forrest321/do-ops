import Foundation

// MARK: - Common Types

public struct APIResponse<T: Codable>: Codable {
    public let data: T?
    public let meta: Meta?
    public let links: Links?
    
    private enum CodingKeys: String, CodingKey {
        case data
        case meta
        case links
    }
}

public struct Meta: Codable {
    public let total: Int?
    public let currentPage: Int?
    public let lastPage: Int?
    public let perPage: Int?
    
    private enum CodingKeys: String, CodingKey {
        case total
        case currentPage = "current_page"
        case lastPage = "last_page"
        case perPage = "per_page"
    }
}

public struct Links: Codable {
    public let first: String?
    public let last: String?
    public let next: String?
    public let prev: String?
}

// MARK: - Account Types

public struct Account: Codable, Identifiable {
    public let id: String
    public let email: String
    public let uuid: String
    public let emailVerified: Bool
    public let status: String
    public let statusMessage: String?
    public let team: Team?
    
    private enum CodingKeys: String, CodingKey {
        case id
        case email
        case uuid
        case emailVerified = "email_verified"
        case status
        case statusMessage = "status_message"
        case team
    }
}

public struct Team: Codable {
    public let uuid: String
    public let name: String
}

// MARK: - Droplet Types

public struct Droplet: Codable, Identifiable {
    public let id: Int
    public let name: String
    public let memory: Int
    public let vcpus: Int
    public let disk: Int
    public let locked: Bool
    public let status: String
    public let kernel: Kernel?
    public let createdAt: Date
    public let features: [String]
    public let backupIds: [Int]
    public let snapshotIds: [Int]
    public let image: Image
    public let size: Size
    public let sizeSlug: String
    public let networks: Networks
    public let region: Region
    public let tags: [String]
    public let volumeIds: [String]
    public let vpcUuid: String?
    
    private enum CodingKeys: String, CodingKey {
        case id, name, memory, vcpus, disk, locked, status, kernel, features, image, size, networks, region, tags
        case createdAt = "created_at"
        case backupIds = "backup_ids"
        case snapshotIds = "snapshot_ids"
        case sizeSlug = "size_slug"
        case volumeIds = "volume_ids"
        case vpcUuid = "vpc_uuid"
    }
}

public struct Kernel: Codable {
    public let id: Int
    public let name: String
    public let version: String
}

public struct Image: Codable, Identifiable {
    public let id: Int
    public let name: String
    public let distribution: String
    public let slug: String?
    public let public: Bool
    public let regions: [String]
    public let minDiskSize: Int
    public let type: String
    public let size: Double
    public let description: String?
    public let tags: [String]
    public let createdAt: Date
    public let status: String
    public let errorMessage: String?
    
    private enum CodingKeys: String, CodingKey {
        case id, name, distribution, slug, regions, type, size, description, tags, status
        case public = "public"
        case minDiskSize = "min_disk_size"
        case createdAt = "created_at"
        case errorMessage = "error_message"
    }
}

public struct Size: Codable {
    public let slug: String
    public let memory: Int
    public let vcpus: Int
    public let disk: Int
    public let transfer: Double
    public let priceMonthly: Double
    public let priceHourly: Double
    public let regions: [String]
    public let available: Bool
    public let description: String
    
    private enum CodingKeys: String, CodingKey {
        case slug, memory, vcpus, disk, transfer, regions, available, description
        case priceMonthly = "price_monthly"
        case priceHourly = "price_hourly"
    }
}

public struct Networks: Codable {
    public let v4: [NetworkV4]
    public let v6: [NetworkV6]
}

public struct NetworkV4: Codable {
    public let ipAddress: String
    public let netmask: String
    public let gateway: String
    public let type: String
    
    private enum CodingKeys: String, CodingKey {
        case netmask, gateway, type
        case ipAddress = "ip_address"
    }
}

public struct NetworkV6: Codable {
    public let ipAddress: String
    public let netmask: Int
    public let gateway: String
    public let type: String
    
    private enum CodingKeys: String, CodingKey {
        case netmask, gateway, type
        case ipAddress = "ip_address"
    }
}

public struct Region: Codable {
    public let name: String
    public let slug: String
    public let features: [String]
    public let available: Bool
    public let sizes: [String]
}

// MARK: - Create Droplet Request

public struct CreateDropletRequest: Codable {
    public let name: String
    public let region: String
    public let size: String
    public let image: CreateDropletImage
    public let sshKeys: [CreateDropletSSHKey]?
    public let backups: Bool?
    public let ipv6: Bool?
    public let monitoring: Bool?
    public let userData: String?
    public let privateNetworking: Bool?
    public let volumes: [String]?
    public let tags: [String]?
    public let vpcUuid: String?
    
    public init(
        name: String,
        region: String,
        size: String,
        image: CreateDropletImage,
        sshKeys: [CreateDropletSSHKey]? = nil,
        backups: Bool? = nil,
        ipv6: Bool? = nil,
        monitoring: Bool? = nil,
        userData: String? = nil,
        privateNetworking: Bool? = nil,
        volumes: [String]? = nil,
        tags: [String]? = nil,
        vpcUuid: String? = nil
    ) {
        self.name = name
        self.region = region
        self.size = size
        self.image = image
        self.sshKeys = sshKeys
        self.backups = backups
        self.ipv6 = ipv6
        self.monitoring = monitoring
        self.userData = userData
        self.privateNetworking = privateNetworking
        self.volumes = volumes
        self.tags = tags
        self.vpcUuid = vpcUuid
    }
    
    private enum CodingKeys: String, CodingKey {
        case name, region, size, image, backups, ipv6, monitoring, volumes, tags
        case sshKeys = "ssh_keys"
        case userData = "user_data"
        case privateNetworking = "private_networking"
        case vpcUuid = "vpc_uuid"
    }
}

public enum CreateDropletImage: Codable {
    case id(Int)
    case slug(String)
    
    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        if let id = try? container.decode(Int.self) {
            self = .id(id)
        } else if let slug = try? container.decode(String.self) {
            self = .slug(slug)
        } else {
            throw DecodingError.typeMismatch(CreateDropletImage.self, DecodingError.Context(codingPath: decoder.codingPath, debugDescription: "Expected Int or String"))
        }
    }
    
    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        switch self {
        case .id(let id):
            try container.encode(id)
        case .slug(let slug):
            try container.encode(slug)
        }
    }
}

public enum CreateDropletSSHKey: Codable {
    case id(Int)
    case fingerprint(String)
    
    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        if let id = try? container.decode(Int.self) {
            self = .id(id)
        } else if let fingerprint = try? container.decode(String.self) {
            self = .fingerprint(fingerprint)
        } else {
            throw DecodingError.typeMismatch(CreateDropletSSHKey.self, DecodingError.Context(codingPath: decoder.codingPath, debugDescription: "Expected Int or String"))
        }
    }
    
    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        switch self {
        case .id(let id):
            try container.encode(id)
        case .fingerprint(let fingerprint):
            try container.encode(fingerprint)
        }
    }
}

// MARK: - Error Types

public struct DOError: Codable, Error {
    public let id: String
    public let message: String
    public let requestId: String?
    
    private enum CodingKeys: String, CodingKey {
        case id, message
        case requestId = "request_id"
    }
}

public struct DOErrorResponse: Codable {
    public let errors: [DOError]
    public let errorMessage: String?
    
    private enum CodingKeys: String, CodingKey {
        case errors
        case errorMessage = "error_message"
    }
}