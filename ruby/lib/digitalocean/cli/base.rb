module DigitalOcean
  module CLI
    class Base < Thor
      protected

      def client
        @client ||= DigitalOcean::Client.from_config
      rescue DigitalOcean::Error => e
        error e.message
        exit 1
      end

      def error(message)
        puts Rainbow("Error: #{message}").red
      end

      def success(message)
        puts Rainbow(message).green
      end

      def info(message)
        puts Rainbow(message).blue
      end

      def format_table(headers, rows)
        table = Terminal::Table.new do |t|
          t.headings = headers
          rows.each { |row| t.add_row(row) }
        end
        puts table
      end

      def format_datetime(time)
        return 'N/A' unless time
        time.strftime('%Y-%m-%d %H:%M:%S UTC')
      end

      def format_date(time)
        return 'N/A' unless time
        time.strftime('%Y-%m-%d')
      end

      def confirm(message, default: false)
        prompt = TTY::Prompt.new
        prompt.yes?(message, default: default)
      end
    end
  end
end