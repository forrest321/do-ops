import { Config } from './config.js';
export class DigitalOceanClientError extends Error {
    status;
    errors;
    constructor(message, status, errors) {
        super(message);
        this.status = status;
        this.errors = errors;
        this.name = 'DigitalOceanClientError';
    }
}
export class DigitalOceanClient {
    config;
    constructor(options = {}) {
        this.config = new Config(options);
    }
    static async fromConfig() {
        const config = await Config.load();
        return new DigitalOceanClient({
            token: config.token,
            baseUrl: config.baseUrl
        });
    }
    async request(method, endpoint, body) {
        if (!this.config.token) {
            throw new DigitalOceanClientError('Authentication token is required');
        }
        const url = new URL(endpoint, this.config.baseUrl);
        const headers = {
            'Authorization': `Bearer ${this.config.token}`,
            'Content-Type': 'application/json',
            'User-Agent': '@do-op/client/1.0.0'
        };
        const options = {
            method,
            headers,
        };
        if (body) {
            options.body = JSON.stringify(body);
        }
        const response = await fetch(url.toString(), options);
        const responseText = await response.text();
        if (!response.ok) {
            let errorMessage = `HTTP ${response.status}: ${response.statusText}`;
            let errors;
            try {
                const errorResponse = JSON.parse(responseText);
                if (errorResponse.errors && errorResponse.errors.length > 0) {
                    errorMessage = errorResponse.errors[0].message;
                    errors = errorResponse.errors;
                }
            }
            catch {
                // Response is not JSON, use status text
            }
            throw new DigitalOceanClientError(errorMessage, response.status, errors);
        }
        if (!responseText) {
            return {};
        }
        try {
            return JSON.parse(responseText);
        }
        catch (error) {
            throw new DigitalOceanClientError('Invalid JSON response from API');
        }
    }
    // Account methods
    async getAccount() {
        const response = await this.request('GET', '/v2/account');
        return response.account;
    }
    // Droplet methods
    async listDroplets() {
        const response = await this.request('GET', '/v2/droplets');
        return response.droplets;
    }
    async getDroplet(id) {
        const response = await this.request('GET', `/v2/droplets/${id}`);
        return response.droplet;
    }
    async createDroplet(request) {
        const response = await this.request('POST', '/v2/droplets', request);
        return response.droplet;
    }
    async deleteDroplet(id) {
        await this.request('DELETE', `/v2/droplets/${id}`);
    }
    // Configuration methods
    getConfig() {
        return this.config;
    }
    updateConfig(options) {
        if (options.token !== undefined)
            this.config.token = options.token;
        if (options.baseUrl !== undefined)
            this.config.baseUrl = options.baseUrl;
    }
}
//# sourceMappingURL=client.js.map