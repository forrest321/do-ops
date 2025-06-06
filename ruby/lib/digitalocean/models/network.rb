module DigitalOcean
  module Models
    class Networks < Base
      attr_accessor :v4, :v6

      def initialize(attributes = {})
        super
        @v4 = (attributes[:v4] || []).map { |net| NetworkV4.new(net) }
        @v6 = (attributes[:v6] || []).map { |net| NetworkV6.new(net) }
      end
    end

    class NetworkV4 < Base
      attr_accessor :ip_address, :netmask, :gateway, :type
    end

    class NetworkV6 < Base
      attr_accessor :ip_address, :netmask, :gateway, :type
    end
  end
end