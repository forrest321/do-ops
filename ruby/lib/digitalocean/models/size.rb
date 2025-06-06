module DigitalOcean
  module Models
    class Size < Base
      attr_accessor :slug, :memory, :vcpus, :disk, :transfer,
                    :price_monthly, :price_hourly, :regions, :available

      def initialize(attributes = {})
        super
        @regions = attributes[:regions] || []
      end
    end
  end
end