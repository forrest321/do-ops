import { readFile, writeFile, mkdir } from 'fs/promises';
import { homedir } from 'os';
import { join, dirname } from 'path';

export interface ClientConfig {
  token: string;
  baseUrl: string;
}

export interface ConfigOptions {
  token?: string;
  baseUrl?: string;
}

const DEFAULT_BASE_URL = 'https://api.digitalocean.com';
const CONFIG_FILE_NAME = 'config.json';

export class Config {
  private config: ClientConfig;

  constructor(options: ConfigOptions = {}) {
    this.config = {
      token: options.token || process.env.DIGITALOCEAN_TOKEN || '',
      baseUrl: options.baseUrl || process.env.DIGITALOCEAN_BASE_URL || DEFAULT_BASE_URL,
    };
  }

  get token(): string {
    return this.config.token;
  }

  get baseUrl(): string {
    return this.config.baseUrl;
  }

  set token(value: string) {
    this.config.token = value;
  }

  set baseUrl(value: string) {
    this.config.baseUrl = value;
  }

  static async load(): Promise<Config> {
    const config = new Config();
    
    // If environment variable is set, use it
    if (process.env.DIGITALOCEAN_TOKEN) {
      return config;
    }

    try {
      const configPath = await getConfigPath();
      const data = await readFile(configPath, 'utf-8');
      const fileConfig = JSON.parse(data) as Partial<ClientConfig>;
      
      if (fileConfig.token) config.token = fileConfig.token;
      if (fileConfig.baseUrl) config.baseUrl = fileConfig.baseUrl;
    } catch (error) {
      // Config file doesn't exist or is invalid, use defaults
    }

    return config;
  }

  async save(): Promise<void> {
    const configPath = await getConfigPath();
    const configDir = dirname(configPath);
    
    await mkdir(configDir, { recursive: true });
    await writeFile(configPath, JSON.stringify(this.config, null, 2));
  }

  toJSON(): ClientConfig {
    return { ...this.config };
  }
}

async function getConfigPath(): Promise<string> {
  return join(homedir(), '.config', 'do-cli', CONFIG_FILE_NAME);
}