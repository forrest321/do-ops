import Foundation
import HTTPTypes
import HTTPTypesFoundation

public class DigitalOceanClient {
    private let config: DigitalOceanConfig
    private let urlSession: URLSession
    private let jsonDecoder: JSONDecoder
    private let jsonEncoder: JSONEncoder
    
    public init(config: DigitalOceanConfig) {
        self.config = config
        
        let configuration = URLSessionConfiguration.default
        configuration.timeoutIntervalForRequest = config.timeout
        configuration.timeoutIntervalForResource = config.timeout
        self.urlSession = URLSession(configuration: configuration)
        
        self.jsonDecoder = JSONDecoder()
        self.jsonDecoder.dateDecodingStrategy = .iso8601
        
        self.jsonEncoder = JSONEncoder()
        self.jsonEncoder.dateEncodingStrategy = .iso8601
    }
    
    // MARK: - Private Methods
    
    private func createRequest(
        path: String,
        method: HTTPRequest.Method = .get,
        queryParameters: [String: String]? = nil,
        body: Data? = nil
    ) throws -> HTTPRequest {
        var urlComponents = URLComponents(url: config.baseURL.appendingPathComponent(path), resolvingAgainstBaseURL: false)!
        
        if let queryParameters = queryParameters {
            urlComponents.queryItems = queryParameters.map { URLQueryItem(name: $0.key, value: $0.value) }
        }
        
        guard let url = urlComponents.url else {
            throw URLError(.badURL)
        }
        
        var request = HTTPRequest(method: method, url: url)
        request.headerFields[.authorization] = "Bearer \(config.apiToken)"
        request.headerFields[.userAgent] = config.userAgent
        request.headerFields[.contentType] = "application/json"
        
        return request
    }
    
    private func performRequest<T: Codable>(_ request: HTTPRequest, body: Data? = nil, responseType: T.Type) async throws -> T {
        let (data, response) = try await urlSession.data(for: request, body: body)
        
        guard response.status.kind == .successful else {
            if let errorResponse = try? jsonDecoder.decode(DOErrorResponse.self, from: data) {
                throw errorResponse.errors.first ?? DOError(id: "unknown", message: "Unknown error", requestId: nil)
            }
            throw URLError(.badServerResponse)
        }
        
        return try jsonDecoder.decode(T.self, from: data)
    }
    
    // MARK: - Account Methods
    
    public func getAccount() async throws -> Account {
        let request = try createRequest(path: "account")
        let response: APIResponse<Account> = try await performRequest(request, responseType: APIResponse<Account>.self)
        
        guard let account = response.data else {
            throw DOError(id: "no_data", message: "No account data received", requestId: nil)
        }
        
        return account
    }
    
    // MARK: - Droplet Methods
    
    public func listDroplets(page: Int = 1, perPage: Int = 25, tag: String? = nil) async throws -> (droplets: [Droplet], meta: Meta?) {
        var queryParams = [
            "page": String(page),
            "per_page": String(perPage)
        ]
        
        if let tag = tag {
            queryParams["tag_name"] = tag
        }
        
        let request = try createRequest(path: "droplets", queryParameters: queryParams)
        let response: APIResponse<[Droplet]> = try await performRequest(request, responseType: APIResponse<[Droplet]>.self)
        
        return (droplets: response.data ?? [], meta: response.meta)
    }
    
    public func getDroplet(id: Int) async throws -> Droplet {
        let request = try createRequest(path: "droplets/\(id)")
        let response: APIResponse<Droplet> = try await performRequest(request, responseType: APIResponse<Droplet>.self)
        
        guard let droplet = response.data else {
            throw DOError(id: "no_data", message: "No droplet data received", requestId: nil)
        }
        
        return droplet
    }
    
    public func createDroplet(_ dropletRequest: CreateDropletRequest) async throws -> Droplet {
        let body = try jsonEncoder.encode(dropletRequest)
        let request = try createRequest(path: "droplets", method: .post)
        let response: APIResponse<Droplet> = try await performRequest(request, body: body, responseType: APIResponse<Droplet>.self)
        
        guard let droplet = response.data else {
            throw DOError(id: "no_data", message: "No droplet data received", requestId: nil)
        }
        
        return droplet
    }
    
    public func deleteDroplet(id: Int) async throws {
        let request = try createRequest(path: "droplets/\(id)", method: .delete)
        let _: APIResponse<String> = try await performRequest(request, responseType: APIResponse<String>.self)
    }
    
    public func rebootDroplet(id: Int) async throws {
        let body = try jsonEncoder.encode(["type": "reboot"])
        let request = try createRequest(path: "droplets/\(id)/actions", method: .post)
        let _: APIResponse<String> = try await performRequest(request, body: body, responseType: APIResponse<String>.self)
    }
    
    public func powerOffDroplet(id: Int) async throws {
        let body = try jsonEncoder.encode(["type": "power_off"])
        let request = try createRequest(path: "droplets/\(id)/actions", method: .post)
        let _: APIResponse<String> = try await performRequest(request, body: body, responseType: APIResponse<String>.self)
    }
    
    public func powerOnDroplet(id: Int) async throws {
        let body = try jsonEncoder.encode(["type": "power_on"])
        let request = try createRequest(path: "droplets/\(id)/actions", method: .post)
        let _: APIResponse<String> = try await performRequest(request, body: body, responseType: APIResponse<String>.self)
    }
    
    // MARK: - Region Methods
    
    public func listRegions() async throws -> [Region] {
        let request = try createRequest(path: "regions")
        let response: APIResponse<[Region]> = try await performRequest(request, responseType: APIResponse<[Region]>.self)
        
        return response.data ?? []
    }
    
    // MARK: - Size Methods
    
    public func listSizes() async throws -> [Size] {
        let request = try createRequest(path: "sizes")
        let response: APIResponse<[Size]> = try await performRequest(request, responseType: APIResponse<[Size]>.self)
        
        return response.data ?? []
    }
    
    // MARK: - Image Methods
    
    public func listImages(type: String? = nil, private: Bool? = nil) async throws -> [Image] {
        var queryParams: [String: String] = [:]
        
        if let type = type {
            queryParams["type"] = type
        }
        
        if let private = private {
            queryParams["private"] = String(private)
        }
        
        let request = try createRequest(path: "images", queryParameters: queryParams.isEmpty ? nil : queryParams)
        let response: APIResponse<[Image]> = try await performRequest(request, responseType: APIResponse<[Image]>.self)
        
        return response.data ?? []
    }
}