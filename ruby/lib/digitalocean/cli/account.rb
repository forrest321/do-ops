module DigitalOcean
  module CLI
    class Account < Base
      desc 'info', 'Show account information'
      def info
        account = client.account.info

        puts Rainbow('Account Information:').bold
        puts "  Email: #{account.email}"
        puts "  UUID: #{account.uuid}"
        puts "  Status: #{Rainbow(account.status).cyan}"
        puts "  Email Verified: #{account.email_verified}"
        puts "  Droplet Limit: #{account.droplet_limit}"
        puts "  Floating IP Limit: #{account.floating_ip_limit}"
        puts "  Volume Limit: #{account.volume_limit}"

        if account.team
          puts "  Team: #{account.team.name} (#{account.team.uuid})"
        end
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end
    end
  end
end