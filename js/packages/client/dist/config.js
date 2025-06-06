import { readFile, writeFile, mkdir } from 'fs/promises';
import { homedir } from 'os';
import { join, dirname } from 'path';
const DEFAULT_BASE_URL = 'https://api.digitalocean.com';
const CONFIG_FILE_NAME = 'config.json';
export class Config {
    config;
    constructor(options = {}) {
        this.config = {
            token: options.token || process.env.DIGITALOCEAN_TOKEN || '',
            baseUrl: options.baseUrl || process.env.DIGITALOCEAN_BASE_URL || DEFAULT_BASE_URL,
        };
    }
    get token() {
        return this.config.token;
    }
    get baseUrl() {
        return this.config.baseUrl;
    }
    set token(value) {
        this.config.token = value;
    }
    set baseUrl(value) {
        this.config.baseUrl = value;
    }
    static async load() {
        const config = new Config();
        // If environment variable is set, use it
        if (process.env.DIGITALOCEAN_TOKEN) {
            return config;
        }
        try {
            const configPath = await getConfigPath();
            const data = await readFile(configPath, 'utf-8');
            const fileConfig = JSON.parse(data);
            if (fileConfig.token)
                config.token = fileConfig.token;
            if (fileConfig.baseUrl)
                config.baseUrl = fileConfig.baseUrl;
        }
        catch (error) {
            // Config file doesn't exist or is invalid, use defaults
        }
        return config;
    }
    async save() {
        const configPath = await getConfigPath();
        const configDir = dirname(configPath);
        await mkdir(configDir, { recursive: true });
        await writeFile(configPath, JSON.stringify(this.config, null, 2));
    }
    toJSON() {
        return { ...this.config };
    }
}
async function getConfigPath() {
    return join(homedir(), '.config', 'do-cli', CONFIG_FILE_NAME);
}
//# sourceMappingURL=config.js.map