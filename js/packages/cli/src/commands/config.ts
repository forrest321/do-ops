import { Command } from 'commander';
import { Config } from '@do-op/client';

export function createConfigCommand(): Command {
  const config = new Command('config')
    .description('Configuration management commands');

  config
    .command('set')
    .description('Set a configuration value')
    .argument('<key>', 'Configuration key (token, base-url)')
    .argument('<value>', 'Configuration value')
    .action(async (key: string, value: string) => {
      try {
        const cfg = await Config.load();

        switch (key) {
          case 'token':
            cfg.token = value;
            break;
          case 'base-url':
            cfg.baseUrl = value;
            break;
          default:
            console.error(`Error: Unknown config key: ${key}`);
            process.exit(1);
        }

        await cfg.save();
        console.log(`Configuration updated: ${key} = ${value}`);
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  config
    .command('get')
    .description('Get configuration value(s)')
    .argument('[key]', 'Configuration key (optional)')
    .action(async (key?: string) => {
      try {
        const cfg = await Config.load();

        if (!key) {
          console.log('Configuration:');
          console.log(`  token: ${maskToken(cfg.token)}`);
          console.log(`  base-url: ${cfg.baseUrl}`);
          return;
        }

        switch (key) {
          case 'token':
            console.log(`token: ${maskToken(cfg.token)}`);
            break;
          case 'base-url':
            console.log(`base-url: ${cfg.baseUrl}`);
            break;
          default:
            console.error(`Error: Unknown config key: ${key}`);
            process.exit(1);
        }
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  return config;
}

function maskToken(token: string): string {
  if (!token) {
    return '<not set>';
  }
  if (token.length <= 8) {
    return '***';
  }
  return token.slice(0, 4) + '...' + token.slice(-4);
}