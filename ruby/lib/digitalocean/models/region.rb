module DigitalOcean
  module Models
    class Region < Base
      attr_accessor :name, :slug, :features, :available, :sizes

      def initialize(attributes = {})
        super
        @features = attributes[:features] || []
        @sizes = attributes[:sizes] || []
      end
    end
  end
end