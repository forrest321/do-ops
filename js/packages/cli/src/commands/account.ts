import { Command } from 'commander';
import { DigitalOceanClient } from '@do-op/client';

export function createAccountCommand(): Command {
  const account = new Command('account')
    .description('Account management commands');

  account
    .command('info')
    .description('Show account information')
    .action(async () => {
      try {
        const client = await DigitalOceanClient.fromConfig();
        const accountInfo = await client.getAccount();

        console.log('Account Information:');
        console.log(`  Email: ${accountInfo.email}`);
        console.log(`  UUID: ${accountInfo.uuid}`);
        console.log(`  Status: ${accountInfo.status}`);
        console.log(`  Email Verified: ${accountInfo.email_verified}`);
        console.log(`  Droplet Limit: ${accountInfo.droplet_limit}`);
        console.log(`  Floating IP Limit: ${accountInfo.floating_ip_limit}`);
        console.log(`  Volume Limit: ${accountInfo.volume_limit}`);

        if (accountInfo.team) {
          console.log(`  Team: ${accountInfo.team.name} (${accountInfo.team.uuid})`);
        }
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  return account;
}