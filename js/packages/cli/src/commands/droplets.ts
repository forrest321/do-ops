import { Command } from 'commander';
import { DigitalOceanClient, CreateDropletRequest } from '@do-op/client';
import readline from 'readline';

export function createDropletsCommand(): Command {
  const droplets = new Command('droplets')
    .description('Droplet management commands');

  droplets
    .command('list')
    .description('List all droplets')
    .action(async () => {
      try {
        const client = await DigitalOceanClient.fromConfig();
        const dropletList = await client.listDroplets();

        if (dropletList.length === 0) {
          console.log('No droplets found');
          return;
        }

        console.log('%-8s %-20s %-10s %-12s %-10s %-15s %s',
          'ID', 'NAME', 'STATUS', 'SIZE', 'REGION', 'IP', 'CREATED');
        console.log('%-8s %-20s %-10s %-12s %-10s %-15s %s',
          '--', '----', '------', '----', '------', '--', '-------');

        for (const droplet of dropletList) {
          let ip = '';
          if (droplet.networks?.v4) {
            const publicNet = droplet.networks.v4.find(net => net.type === 'public');
            if (publicNet) ip = publicNet.ip_address;
          }

          const created = new Date(droplet.created_at).toISOString().split('T')[0];
          const region = droplet.region?.slug || '';

          console.log('%-8d %-20s %-10s %-12s %-10s %-15s %s',
            droplet.id, droplet.name, droplet.status, droplet.size_slug,
            region, ip, created);
        }
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  droplets
    .command('get')
    .description('Get droplet details')
    .argument('<id>', 'Droplet ID')
    .action(async (id: string) => {
      try {
        const dropletId = parseInt(id, 10);
        if (isNaN(dropletId)) {
          console.error('Error: Invalid droplet ID');
          process.exit(1);
        }

        const client = await DigitalOceanClient.fromConfig();
        const droplet = await client.getDroplet(dropletId);

        console.log('Droplet Details:');
        console.log(`  ID: ${droplet.id}`);
        console.log(`  Name: ${droplet.name}`);
        console.log(`  Status: ${droplet.status}`);
        console.log(`  Memory: ${droplet.memory} MB`);
        console.log(`  VCPUs: ${droplet.vcpus}`);
        console.log(`  Disk: ${droplet.disk} GB`);
        console.log(`  Locked: ${droplet.locked}`);
        console.log(`  Created: ${new Date(droplet.created_at).toISOString()}`);

        if (droplet.image) {
          console.log(`  Image: ${droplet.image.name} (${droplet.image.distribution})`);
        }

        if (droplet.size) {
          console.log(`  Size: ${droplet.size.slug} ($${droplet.size.price_monthly}/month)`);
        }

        if (droplet.region) {
          console.log(`  Region: ${droplet.region.name} (${droplet.region.slug})`);
        }

        if (droplet.networks) {
          console.log('  Networks:');
          droplet.networks.v4.forEach(net => {
            console.log(`    ${net.type}: ${net.ip_address}`);
          });
          droplet.networks.v6.forEach(net => {
            console.log(`    ${net.type} (v6): ${net.ip_address}`);
          });
        }

        if (droplet.tags.length > 0) {
          console.log(`  Tags: ${droplet.tags.join(', ')}`);
        }
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  droplets
    .command('create')
    .description('Create a new droplet')
    .requiredOption('-n, --name <name>', 'Droplet name')
    .requiredOption('-r, --region <region>', 'Region slug')
    .requiredOption('-s, --size <size>', 'Size slug')
    .requiredOption('-i, --image <image>', 'Image slug or ID')
    .option('-t, --tags <tags...>', 'Tags for the droplet')
    .action(async (options) => {
      try {
        const client = await DigitalOceanClient.fromConfig();

        const request: CreateDropletRequest = {
          name: options.name,
          region: options.region,
          size: options.size,
          image: options.image,
          tags: options.tags || []
        };

        const droplet = await client.createDroplet(request);

        console.log('Droplet created successfully!');
        console.log(`  ID: ${droplet.id}`);
        console.log(`  Name: ${droplet.name}`);
        console.log(`  Status: ${droplet.status}`);
        console.log(`  Region: ${options.region}`);
        console.log(`  Size: ${options.size}`);
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  droplets
    .command('delete')
    .description('Delete a droplet')
    .argument('<id>', 'Droplet ID')
    .action(async (id: string) => {
      try {
        const dropletId = parseInt(id, 10);
        if (isNaN(dropletId)) {
          console.error('Error: Invalid droplet ID');
          process.exit(1);
        }

        const rl = readline.createInterface({
          input: process.stdin,
          output: process.stdout
        });

        const answer = await new Promise<string>((resolve) => {
          rl.question(`Are you sure you want to delete droplet ${dropletId}? (y/N): `, resolve);
        });

        rl.close();

        if (answer.toLowerCase() !== 'y' && answer.toLowerCase() !== 'yes') {
          console.log('Cancelled');
          return;
        }

        const client = await DigitalOceanClient.fromConfig();
        await client.deleteDroplet(dropletId);

        console.log(`Droplet ${dropletId} deleted successfully`);
      } catch (error) {
        console.error('Error:', error instanceof Error ? error.message : error);
        process.exit(1);
      }
    });

  return droplets;
}