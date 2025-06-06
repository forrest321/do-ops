import type { Account, Droplet, CreateDropletRequest, ErrorResponse } from '@do-op/types';
import { Config, ConfigOptions } from './config.js';
export declare class DigitalOceanClientError extends Error {
    status?: number | undefined;
    errors?: ErrorResponse["errors"] | undefined;
    constructor(message: string, status?: number | undefined, errors?: ErrorResponse["errors"] | undefined);
}
export declare class DigitalOceanClient {
    private config;
    constructor(options?: ConfigOptions);
    static fromConfig(): Promise<DigitalOceanClient>;
    private request;
    getAccount(): Promise<Account>;
    listDroplets(): Promise<Droplet[]>;
    getDroplet(id: number): Promise<Droplet>;
    createDroplet(request: CreateDropletRequest): Promise<Droplet>;
    deleteDroplet(id: number): Promise<void>;
    getConfig(): Config;
    updateConfig(options: ConfigOptions): void;
}
//# sourceMappingURL=client.d.ts.map