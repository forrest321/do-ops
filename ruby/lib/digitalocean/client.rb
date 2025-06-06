require 'faraday'
require 'faraday/multipart'
require 'json'

module DigitalOcean
  class Client
    attr_reader :config, :connection

    def initialize(token: nil, base_url: nil, config: nil)
      @config = config || Config.new(token: token, base_url: base_url)
      @config.validate!
      @connection = build_connection
    end

    def self.from_config
      config = Config.load
      new(config: config)
    end

    # Account methods
    def account
      @account_client ||= AccountClient.new(self)
    end

    # Droplet methods
    def droplets
      @droplets_client ||= DropletsClient.new(self)
    end

    def request(method, path, body: nil, params: nil)
      response = connection.send(method) do |req|
        req.url(path)
        req.params = params if params
        req.body = body.to_json if body
      end

      handle_response(response)
    rescue Faraday::Error => e
      raise APIError, "HTTP request failed: #{e.message}"
    end

    private

    def build_connection
      Faraday.new(url: config.base_url) do |conn|
        conn.request :json
        conn.response :json, content_type: /\bjson$/
        conn.headers['Authorization'] = "Bearer #{config.token}"
        conn.headers['Content-Type'] = 'application/json'
        conn.headers['User-Agent'] = "digitalocean-ruby/#{DigitalOcean::VERSION}"
        conn.adapter Faraday.default_adapter
      end
    end

    def handle_response(response)
      case response.status
      when 200..299
        response.body
      when 401
        raise AuthenticationError, 'Invalid or missing authentication token'
      when 404
        raise APIError, 'Resource not found'
      when 422
        errors = extract_errors(response.body)
        raise APIError, errors.any? ? errors.join(', ') : 'Validation failed'
      when 429
        raise APIError, 'Rate limit exceeded'
      else
        errors = extract_errors(response.body)
        message = errors.any? ? errors.join(', ') : "HTTP #{response.status}"
        raise APIError.new(message, status: response.status, response_body: response.body)
      end
    end

    def extract_errors(body)
      return [] unless body.is_a?(Hash) && body['errors']

      body['errors'].map do |error|
        error.is_a?(Hash) ? error['message'] : error.to_s
      end
    end
  end

  class AccountClient
    def initialize(client)
      @client = client
    end

    def info
      response = @client.request(:get, '/v2/account')
      Models::Account.new(response['account'])
    end
  end

  class DropletsClient
    def initialize(client)
      @client = client
    end

    def list
      response = @client.request(:get, '/v2/droplets')
      response['droplets'].map { |droplet| Models::Droplet.new(droplet) }
    end

    def find(id)
      response = @client.request(:get, "/v2/droplets/#{id}")
      Models::Droplet.new(response['droplet'])
    end

    def create(attributes)
      request = if attributes.is_a?(Models::CreateDropletRequest)
                  attributes
                else
                  Models::CreateDropletRequest.new(attributes)
                end

      response = @client.request(:post, '/v2/droplets', body: request.to_h)
      Models::Droplet.new(response['droplet'])
    end

    def delete(id)
      @client.request(:delete, "/v2/droplets/#{id}")
      true
    end
  end
end