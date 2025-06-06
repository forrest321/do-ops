export interface ClientConfig {
    token: string;
    baseUrl: string;
}
export interface ConfigOptions {
    token?: string;
    baseUrl?: string;
}
export declare class Config {
    private config;
    constructor(options?: ConfigOptions);
    get token(): string;
    get baseUrl(): string;
    set token(value: string);
    set baseUrl(value: string);
    static load(): Promise<Config>;
    save(): Promise<void>;
    toJSON(): ClientConfig;
}
//# sourceMappingURL=config.d.ts.map