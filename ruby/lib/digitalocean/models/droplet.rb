module DigitalOcean
  module Models
    class Droplet < Base
      attr_accessor :id, :name, :memory, :vcpus, :disk, :locked, :status,
                    :kernel, :created_at, :features, :backup_ids, :snapshot_ids,
                    :image, :size, :size_slug, :networks, :region, :tags,
                    :volume_ids, :vpc_uuid

      def initialize(attributes = {})
        super
        @created_at = parse_datetime(attributes[:created_at])
        @kernel = Kernel.new(attributes[:kernel]) if attributes[:kernel]
        @image = Image.new(attributes[:image]) if attributes[:image]
        @size = Size.new(attributes[:size]) if attributes[:size]
        @networks = Networks.new(attributes[:networks]) if attributes[:networks]
        @region = Region.new(attributes[:region]) if attributes[:region]
        @features = attributes[:features] || []
        @backup_ids = attributes[:backup_ids] || []
        @snapshot_ids = attributes[:snapshot_ids] || []
        @tags = attributes[:tags] || []
        @volume_ids = attributes[:volume_ids] || []
      end

      def public_ip
        return nil unless networks&.v4

        public_network = networks.v4.find { |net| net.type == 'public' }
        public_network&.ip_address
      end

      def private_ip
        return nil unless networks&.v4

        private_network = networks.v4.find { |net| net.type == 'private' }
        private_network&.ip_address
      end
    end

    class Kernel < Base
      attr_accessor :id, :name, :version
    end

    class CreateDropletRequest < Base
      attr_accessor :name, :region, :size, :image, :ssh_keys, :backups,
                    :ipv6, :monitoring, :tags, :user_data, :private_networking,
                    :volumes, :vpc_uuid

      def initialize(attributes = {})
        super
        @tags = attributes[:tags] || []
        @ssh_keys = attributes[:ssh_keys] || []
        @volumes = attributes[:volumes] || []
      end

      def to_h
        super.reject { |_, v| v.nil? || (v.respond_to?(:empty?) && v.empty?) }
      end
    end
  end
end