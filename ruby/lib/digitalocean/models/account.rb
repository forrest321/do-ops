module DigitalOcean
  module Models
    class Account < Base
      attr_accessor :droplet_limit, :floating_ip_limit, :volume_limit,
                    :email, :uuid, :email_verified, :status, :status_message,
                    :team

      def initialize(attributes = {})
        super
        @team = Team.new(attributes[:team]) if attributes[:team]
      end
    end

    class Team < Base
      attr_accessor :uuid, :name
    end
  end
end