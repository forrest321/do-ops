require_relative 'digitalocean/version'
require_relative 'digitalocean/config'
require_relative 'digitalocean/client'
require_relative 'digitalocean/models'

module DigitalOcean
  class Error < StandardError; end
  class APIError < Error
    attr_reader :status, :response_body

    def initialize(message, status: nil, response_body: nil)
      super(message)
      @status = status
      @response_body = response_body
    end
  end
  class ConfigurationError < Error; end
  class AuthenticationError < Error; end
end