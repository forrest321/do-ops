require 'yaml'
require 'fileutils'

module DigitalOcean
  class Config
    DEFAULT_BASE_URL = 'https://api.digitalocean.com'
    CONFIG_FILE_NAME = 'config.yml'

    attr_accessor :token, :base_url

    def initialize(token: nil, base_url: nil)
      @token = token || ENV['DIGITALOCEAN_TOKEN']
      @base_url = base_url || ENV['DIGITALOCEAN_BASE_URL'] || DEFAULT_BASE_URL
    end

    def self.load
      config = new
      
      # Try to load from config file if no environment variables
      if config.token.nil? || config.token.empty?
        file_config = load_from_file
        config.token = file_config[:token] if file_config[:token]
        config.base_url = file_config[:base_url] if file_config[:base_url]
      end

      config
    end

    def self.load_from_file
      config_path = File.join(config_dir, CONFIG_FILE_NAME)
      return {} unless File.exist?(config_path)

      YAML.load_file(config_path, symbolize_names: true) || {}
    rescue => e
      raise ConfigurationError, "Failed to load config file: #{e.message}"
    end

    def save
      FileUtils.mkdir_p(self.class.config_dir)
      config_path = File.join(self.class.config_dir, CONFIG_FILE_NAME)
      
      config_data = {
        token: @token,
        base_url: @base_url
      }

      File.write(config_path, YAML.dump(config_data))
    rescue => e
      raise ConfigurationError, "Failed to save config file: #{e.message}"
    end

    def valid?
      !token.nil? && !token.empty? && !base_url.nil? && !base_url.empty?
    end

    def validate!
      raise AuthenticationError, 'API token is required' if token.nil? || token.empty?
      raise ConfigurationError, 'Base URL is required' if base_url.nil? || base_url.empty?
    end

    def to_h
      {
        token: mask_token,
        base_url: @base_url
      }
    end

    private

    def self.config_dir
      File.join(Dir.home, '.config', 'do-cli')
    end

    def mask_token
      return '<not set>' if @token.nil? || @token.empty?
      return '***' if @token.length <= 8
      
      "#{@token[0..3]}...#{@token[-4..-1]}"
    end
  end
end