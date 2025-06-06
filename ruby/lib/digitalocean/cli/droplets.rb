module DigitalOcean
  module CLI
    class Droplets < Base
      desc 'list', 'List all droplets'
      def list
        droplets = client.droplets.list

        if droplets.empty?
          info 'No droplets found'
          return
        end

        headers = ['ID', 'NAME', 'STATUS', 'SIZE', 'REGION', 'IP', 'CREATED']
        rows = droplets.map do |droplet|
          [
            droplet.id,
            droplet.name,
            Rainbow(droplet.status).cyan,
            droplet.size_slug,
            droplet.region&.slug || 'N/A',
            droplet.public_ip || 'N/A',
            format_date(droplet.created_at)
          ]
        end

        format_table(headers, rows)
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      desc 'get ID', 'Get droplet details'
      def get(id)
        droplet = client.droplets.find(id.to_i)

        puts Rainbow('Droplet Details:').bold
        puts "  ID: #{droplet.id}"
        puts "  Name: #{droplet.name}"
        puts "  Status: #{Rainbow(droplet.status).cyan}"
        puts "  Memory: #{droplet.memory} MB"
        puts "  VCPUs: #{droplet.vcpus}"
        puts "  Disk: #{droplet.disk} GB"
        puts "  Locked: #{droplet.locked}"
        puts "  Created: #{format_datetime(droplet.created_at)}"

        if droplet.image
          puts "  Image: #{droplet.image.name} (#{droplet.image.distribution})"
        end

        if droplet.size
          puts "  Size: #{droplet.size.slug} ($#{'%.2f' % droplet.size.price_monthly}/month)"
        end

        if droplet.region
          puts "  Region: #{droplet.region.name} (#{droplet.region.slug})"
        end

        if droplet.networks
          puts '  Networks:'
          droplet.networks.v4.each do |net|
            puts "    #{net.type}: #{net.ip_address}"
          end
          droplet.networks.v6.each do |net|
            puts "    #{net.type} (v6): #{net.ip_address}"
          end
        end

        if droplet.tags && droplet.tags.any?
          puts "  Tags: #{droplet.tags.join(', ')}"
        end
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      desc 'create', 'Create a new droplet'
      option :name, aliases: '-n', type: :string, required: true, desc: 'Droplet name'
      option :region, aliases: '-r', type: :string, required: true, desc: 'Region slug'
      option :size, aliases: '-s', type: :string, required: true, desc: 'Size slug'
      option :image, aliases: '-i', type: :string, required: true, desc: 'Image slug or ID'
      option :tags, aliases: '-t', type: :array, desc: 'Tags for the droplet'
      def create
        request = DigitalOcean::Models::CreateDropletRequest.new(
          name: options[:name],
          region: options[:region],
          size: options[:size],
          image: options[:image],
          tags: options[:tags] || []
        )

        droplet = client.droplets.create(request)

        success 'Droplet created successfully!'
        puts "  ID: #{droplet.id}"
        puts "  Name: #{droplet.name}"
        puts "  Status: #{Rainbow(droplet.status).cyan}"
        puts "  Region: #{options[:region]}"
        puts "  Size: #{options[:size]}"
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      desc 'delete ID', 'Delete a droplet'
      def delete(id)
        unless confirm("Are you sure you want to delete droplet #{id}?")
          info 'Cancelled'
          return
        end

        client.droplets.delete(id.to_i)
        success "Droplet #{id} deleted successfully"
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end
    end
  end
end