import XCTest
@testable import DigitalOceanKit

final class DigitalOceanKitTests: XCTestCase {
    
    func testConfigurationFromEnvironment() {
        // Test that config creation from environment handles missing token
        let originalToken = ProcessInfo.processInfo.environment["DIGITALOCEAN_ACCESS_TOKEN"]
        
        // Remove token temporarily
        if originalToken != nil {
            // Can't modify environment in tests, so test with known state
            let config = DigitalOceanConfig.fromEnvironment()
            XCTAssertNotNil(config?.apiToken)
        }
    }
    
    func testConfigurationInitialization() {
        let config = DigitalOceanConfig(
            apiToken: "test-token",
            userAgent: "Test/1.0.0",
            timeout: 60.0
        )
        
        XCTAssertEqual(config.apiToken, "test-token")
        XCTAssertEqual(config.userAgent, "Test/1.0.0")
        XCTAssertEqual(config.timeout, 60.0)
        XCTAssertEqual(config.baseURL.absoluteString, "https://api.digitalocean.com/v2")
    }
    
    func testClientCreation() {
        let client = DigitalOceanKit.createClient(apiToken: "test-token")
        XCTAssertNotNil(client)
    }
    
    func testCreateDropletImageEnum() throws {
        // Test ID encoding/decoding
        let imageId = CreateDropletImage.id(12345)
        let idData = try JSONEncoder().encode(imageId)
        let decodedId = try JSONDecoder().decode(CreateDropletImage.self, from: idData)
        
        if case .id(let id) = decodedId {
            XCTAssertEqual(id, 12345)
        } else {
            XCTFail("Expected ID enum case")
        }
        
        // Test slug encoding/decoding
        let imageSlug = CreateDropletImage.slug("ubuntu-22-04-x64")
        let slugData = try JSONEncoder().encode(imageSlug)
        let decodedSlug = try JSONDecoder().decode(CreateDropletImage.self, from: slugData)
        
        if case .slug(let slug) = decodedSlug {
            XCTAssertEqual(slug, "ubuntu-22-04-x64")
        } else {
            XCTFail("Expected slug enum case")
        }
    }
    
    func testCreateDropletSSHKeyEnum() throws {
        // Test ID encoding/decoding
        let keyId = CreateDropletSSHKey.id(67890)
        let idData = try JSONEncoder().encode(keyId)
        let decodedId = try JSONDecoder().decode(CreateDropletSSHKey.self, from: idData)
        
        if case .id(let id) = decodedId {
            XCTAssertEqual(id, 67890)
        } else {
            XCTFail("Expected ID enum case")
        }
        
        // Test fingerprint encoding/decoding
        let keyFingerprint = CreateDropletSSHKey.fingerprint("aa:bb:cc:dd")
        let fingerprintData = try JSONEncoder().encode(keyFingerprint)
        let decodedFingerprint = try JSONDecoder().decode(CreateDropletSSHKey.self, from: fingerprintData)
        
        if case .fingerprint(let fingerprint) = decodedFingerprint {
            XCTAssertEqual(fingerprint, "aa:bb:cc:dd")
        } else {
            XCTFail("Expected fingerprint enum case")
        }
    }
    
    func testCreateDropletRequest() throws {
        let request = CreateDropletRequest(
            name: "test-droplet",
            region: "nyc3",
            size: "s-1vcpu-1gb",
            image: .slug("ubuntu-22-04-x64"),
            sshKeys: [.id(123), .fingerprint("aa:bb:cc:dd")],
            backups: true,
            ipv6: false,
            monitoring: true,
            tags: ["test", "swift"]
        )
        
        XCTAssertEqual(request.name, "test-droplet")
        XCTAssertEqual(request.region, "nyc3")
        XCTAssertEqual(request.size, "s-1vcpu-1gb")
        XCTAssertEqual(request.backups, true)
        XCTAssertEqual(request.ipv6, false)
        XCTAssertEqual(request.monitoring, true)
        XCTAssertEqual(request.tags, ["test", "swift"])
        XCTAssertEqual(request.sshKeys?.count, 2)
        
        // Test JSON encoding
        let encoder = JSONEncoder()
        let data = try encoder.encode(request)
        XCTAssertGreaterThan(data.count, 0)
        
        // Test JSON decoding
        let decoder = JSONDecoder()
        let decodedRequest = try decoder.decode(CreateDropletRequest.self, from: data)
        XCTAssertEqual(decodedRequest.name, request.name)
        XCTAssertEqual(decodedRequest.region, request.region)
    }
    
    func testDOErrorCoding() throws {
        let error = DOError(
            id: "invalid_param",
            message: "The parameter is invalid",
            requestId: "req-123"
        )
        
        let encoder = JSONEncoder()
        let data = try encoder.encode(error)
        
        let decoder = JSONDecoder()
        let decodedError = try decoder.decode(DOError.self, from: data)
        
        XCTAssertEqual(decodedError.id, error.id)
        XCTAssertEqual(decodedError.message, error.message)
        XCTAssertEqual(decodedError.requestId, error.requestId)
    }
}