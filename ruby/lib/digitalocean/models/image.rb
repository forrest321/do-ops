module DigitalOcean
  module Models
    class Image < Base
      attr_accessor :id, :name, :type, :distribution, :slug, :public,
                    :regions, :min_disk_size, :size_gigabytes, :created_at,
                    :description, :tags, :status, :error_message

      def initialize(attributes = {})
        super
        @created_at = parse_datetime(attributes[:created_at])
        @regions = attributes[:regions] || []
        @tags = attributes[:tags] || []
      end
    end
  end
end