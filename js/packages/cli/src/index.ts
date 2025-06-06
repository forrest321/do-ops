#!/usr/bin/env node

import { Command } from 'commander';
import { createAccountCommand } from './commands/account.js';
import { createDropletsCommand } from './commands/droplets.js';
import { createConfigCommand } from './commands/config.js';

const program = new Command();

program
  .name('do-cli')
  .description('DigitalOcean CLI client')
  .version('1.0.0');

program.addCommand(createAccountCommand());
program.addCommand(createDropletsCommand());
program.addCommand(createConfigCommand());

program.parse();