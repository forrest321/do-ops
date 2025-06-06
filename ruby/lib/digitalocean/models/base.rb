module DigitalOcean
  module Models
    class Base
      def initialize(attributes = {})
        attributes.each do |key, value|
          setter_method = "#{key}="
          send(setter_method, value) if respond_to?(setter_method)
        end
      end

      def to_h
        instance_variables.each_with_object({}) do |var, hash|
          key = var.to_s.delete('@')
          value = instance_variable_get(var)
          hash[key] = value.respond_to?(:to_h) ? value.to_h : value
        end
      end

      def to_json(*args)
        require 'json'
        to_h.to_json(*args)
      end

      private

      def parse_datetime(value)
        return nil if value.nil?
        return value if value.is_a?(Time)
        
        require 'time'
        Time.parse(value)
      rescue ArgumentError
        value
      end
    end
  end
end