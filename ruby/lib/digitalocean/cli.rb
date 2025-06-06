require 'thor'
require 'rainbow'
require 'terminal-table'
require 'tty-prompt'

require_relative 'cli/base'
require_relative 'cli/account'
require_relative 'cli/droplets'
require_relative 'cli/config'

module DigitalOcean
  module CLI
    class Main < Thor
      def self.exit_on_failure?
        true
      end

      desc 'account SUBCOMMAND', 'Account management commands'
      subcommand 'account', Account

      desc 'droplets SUBCOMMAND', 'Droplet management commands'
      subcommand 'droplets', Droplets

      desc 'config SUBCOMMAND', 'Configuration management commands'
      subcommand 'config', Config
    end
  end
end