import type {
  Account,
  AccountResponse,
  Droplet,
  DropletsResponse,
  DropletResponse,
  CreateDropletRequest,
  CreateDropletResponse,
  ErrorResponse
} from '@do-op/types';
import { Config, ConfigOptions } from './config.js';

export class DigitalOceanClientError extends Error {
  constructor(
    message: string,
    public status?: number,
    public errors?: ErrorResponse['errors']
  ) {
    super(message);
    this.name = 'DigitalOceanClientError';
  }
}

export class DigitalOceanClient {
  private config: Config;

  constructor(options: ConfigOptions = {}) {
    this.config = new Config(options);
  }

  static async fromConfig(): Promise<DigitalOceanClient> {
    const config = await Config.load();
    return new DigitalOceanClient({
      token: config.token,
      baseUrl: config.baseUrl
    });
  }

  private async request<T>(
    method: string,
    endpoint: string,
    body?: unknown
  ): Promise<T> {
    if (!this.config.token) {
      throw new DigitalOceanClientError('Authentication token is required');
    }

    const url = new URL(endpoint, this.config.baseUrl);
    
    const headers: Record<string, string> = {
      'Authorization': `Bearer ${this.config.token}`,
      'Content-Type': 'application/json',
      'User-Agent': '@do-op/client/1.0.0'
    };

    const options: RequestInit = {
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
      let errors: ErrorResponse['errors'] | undefined;

      try {
        const errorResponse = JSON.parse(responseText) as ErrorResponse;
        if (errorResponse.errors && errorResponse.errors.length > 0) {
          errorMessage = errorResponse.errors[0].message;
          errors = errorResponse.errors;
        }
      } catch {
        // Response is not JSON, use status text
      }

      throw new DigitalOceanClientError(errorMessage, response.status, errors);
    }

    if (!responseText) {
      return {} as T;
    }

    try {
      return JSON.parse(responseText) as T;
    } catch (error) {
      throw new DigitalOceanClientError('Invalid JSON response from API');
    }
  }

  // Account methods
  async getAccount(): Promise<Account> {
    const response = await this.request<AccountResponse>('GET', '/v2/account');
    return response.account;
  }

  // Droplet methods
  async listDroplets(): Promise<Droplet[]> {
    const response = await this.request<DropletsResponse>('GET', '/v2/droplets');
    return response.droplets;
  }

  async getDroplet(id: number): Promise<Droplet> {
    const response = await this.request<DropletResponse>('GET', `/v2/droplets/${id}`);
    return response.droplet;
  }

  async createDroplet(request: CreateDropletRequest): Promise<Droplet> {
    const response = await this.request<CreateDropletResponse>('POST', '/v2/droplets', request);
    return response.droplet;
  }

  async deleteDroplet(id: number): Promise<void> {
    await this.request<void>('DELETE', `/v2/droplets/${id}`);
  }

  // Configuration methods
  getConfig(): Config {
    return this.config;
  }

  updateConfig(options: ConfigOptions): void {
    if (options.token !== undefined) this.config.token = options.token;
    if (options.baseUrl !== undefined) this.config.baseUrl = options.baseUrl;
  }
}