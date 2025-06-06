#!/usr/bin/env ruby

require_relative '../lib/digitalocean'

# Example: Basic usage of the DigitalOcean Ruby client

begin
  # Initialize client (uses DIGITALOCEAN_TOKEN env var or config file)
  client = DigitalOcean::Client.from_config

  # Get account information
  puts "=== Account Information ==="
  account = client.account.info
  puts "Email: #{account.email}"
  puts "UUID: #{account.uuid}"
  puts "Status: #{account.status}"
  puts "Droplet Limit: #{account.droplet_limit}"
  puts

  # List droplets
  puts "=== Droplets ==="
  droplets = client.droplets.list
  
  if droplets.empty?
    puts "No droplets found"
  else
    droplets.each do |droplet|
      puts "ID: #{droplet.id}, Name: #{droplet.name}, Status: #{droplet.status}"
      puts "  IP: #{droplet.public_ip || 'N/A'}"
      puts "  Region: #{droplet.region&.slug || 'N/A'}"
      puts "  Size: #{droplet.size_slug}"
      puts
    end
  end

  # Example: Create a new droplet (uncomment to test)
  # puts "=== Creating Droplet ==="
  # new_droplet = client.droplets.create(
  #   name: 'example-droplet',
  #   region: 'nyc1',
  #   size: 's-1vcpu-1gb',
  #   image: 'ubuntu-22-04-x64',
  #   tags: ['example', 'ruby-client']
  # )
  # puts "Created droplet: #{new_droplet.name} (ID: #{new_droplet.id})"

rescue DigitalOcean::AuthenticationError => e
  puts "Authentication error: #{e.message}"
  puts "Make sure to set DIGITALOCEAN_TOKEN environment variable"
  exit 1
rescue DigitalOcean::APIError => e
  puts "API error: #{e.message}"
  exit 1
rescue DigitalOcean::Error => e
  puts "Error: #{e.message}"
  exit 1
end