module DigitalOcean
  module CLI
    class Config < Base
      desc 'set KEY VALUE', 'Set a configuration value'
      def set(key, value)
        config = DigitalOcean::Config.load

        case key.downcase
        when 'token'
          config.token = value
        when 'base-url'
          config.base_url = value
        else
          error "Unknown config key: #{key}"
          exit 1
        end

        config.save
        success "Configuration updated: #{key} = #{value}"
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      desc 'get [KEY]', 'Get configuration value(s)'
      def get(key = nil)
        config = DigitalOcean::Config.load

        if key
          case key.downcase
          when 'token'
            puts "token: #{mask_token(config.token)}"
          when 'base-url'
            puts "base-url: #{config.base_url}"
          else
            error "Unknown config key: #{key}"
            exit 1
          end
        else
          puts Rainbow('Configuration:').bold
          puts "  token: #{mask_token(config.token)}"
          puts "  base-url: #{config.base_url}"
        end
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      private

      def mask_token(token)
        return '<not set>' if token.nil? || token.empty?
        return '***' if token.length <= 8
        
        "#{token[0..3]}...#{token[-4..-1]}"
      end
    end
  end
end