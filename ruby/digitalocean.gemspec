require_relative 'lib/digitalocean/version'

Gem::Specification.new do |spec|
  spec.name          = 'digitalocean-client'
  spec.version       = DigitalOcean::VERSION
  spec.authors       = ['DigitalOcean CLI Team']
  spec.email         = ['support@digitalocean.com']

  spec.summary       = 'DigitalOcean API client for Ruby'
  spec.description   = 'A Ruby client library and CLI for the DigitalOcean API v2.0'
  spec.homepage      = 'https://github.com/fo/do-op'
  spec.license       = 'MIT'
  spec.required_ruby_version = '>= 3.0.0'

  spec.metadata['homepage_uri'] = spec.homepage
  spec.metadata['source_code_uri'] = spec.homepage
  spec.metadata['changelog_uri'] = "#{spec.homepage}/blob/main/ruby/CHANGELOG.md"

  # Specify which files should be added to the gem when it is released.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = 'bin'
  spec.executables   = spec.files.grep(%r{\Abin/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']

  # Dependencies
  spec.add_dependency 'faraday', '~> 2.0'
  spec.add_dependency 'faraday-multipart', '~> 1.0'
  spec.add_dependency 'thor', '~> 1.0'
  spec.add_dependency 'rainbow', '~> 3.0'
  spec.add_dependency 'terminal-table', '~> 3.0'
  spec.add_dependency 'tty-prompt', '~> 0.23'
  
  # Development dependencies
  spec.add_development_dependency 'rspec', '~> 3.12'
  spec.add_development_dependency 'webmock', '~> 3.18'
  spec.add_development_dependency 'rubocop', '~> 1.0'
  spec.add_development_dependency 'yard', '~> 0.9'
end