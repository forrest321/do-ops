export interface Meta {
    total: number;
}
export interface Links {
    pages?: Pages;
}
export interface Pages {
    first?: string;
    prev?: string;
    next?: string;
    last?: string;
}
export interface ListResponse<T> {
    data: T[];
    links: Links;
    meta: Meta;
}
export interface ErrorDetail {
    id: string;
    message: string;
    detail?: string;
}
export interface ErrorResponse {
    errors: ErrorDetail[];
}
export interface Action {
    id: number;
    status: string;
    type: string;
    started_at: string;
    completed_at?: string;
    resource_id: number;
    resource_type: string;
    region?: Region;
    region_slug: string;
}
export interface Region {
    name: string;
    slug: string;
    features: string[];
    available: boolean;
    sizes: string[];
}
export interface Size {
    slug: string;
    memory: number;
    vcpus: number;
    disk: number;
    transfer: number;
    price_monthly: number;
    price_hourly: number;
    regions: string[];
    available: boolean;
}
export interface Kernel {
    id: number;
    name: string;
    version: string;
}
export interface Image {
    id: number;
    name: string;
    type: string;
    distribution: string;
    slug: string;
    public: boolean;
    regions: string[];
    min_disk_size: number;
    size_gigabytes: number;
    created_at: string;
    description: string;
    tags: string[];
    status: string;
    error_message: string;
}
export interface NetworkV4 {
    ip_address: string;
    netmask: string;
    gateway: string;
    type: string;
}
export interface NetworkV6 {
    ip_address: string;
    netmask: number;
    gateway: string;
    type: string;
}
export interface Networks {
    v4: NetworkV4[];
    v6: NetworkV6[];
}
export interface Droplet {
    id: number;
    name: string;
    memory: number;
    vcpus: number;
    disk: number;
    locked: boolean;
    status: string;
    kernel?: Kernel;
    created_at: string;
    features: string[];
    backup_ids: number[];
    snapshot_ids: number[];
    image?: Image;
    size?: Size;
    size_slug: string;
    networks?: Networks;
    region?: Region;
    tags: string[];
    volume_ids: string[];
    vpc_uuid: string;
}
export interface CreateDropletRequest {
    name: string;
    region: string;
    size: string;
    image: string;
    ssh_keys?: string[];
    backups?: boolean;
    ipv6?: boolean;
    monitoring?: boolean;
    tags?: string[];
    user_data?: string;
    private_networking?: boolean;
    volumes?: string[];
    vpc_uuid?: string;
}
export interface DropletsResponse {
    droplets: Droplet[];
    links: Links;
    meta: Meta;
}
export interface DropletResponse {
    droplet: Droplet;
    links?: Links;
    meta?: Meta;
}
export interface CreateDropletResponse {
    droplet: Droplet;
    links?: Links;
    meta?: Meta;
    actions?: Action[];
}
export interface Account {
    droplet_limit: number;
    floating_ip_limit: number;
    volume_limit: number;
    email: string;
    uuid: string;
    email_verified: boolean;
    status: string;
    status_message: string;
    team?: Team;
}
export interface Team {
    uuid: string;
    name: string;
}
export interface AccountResponse {
    account: Account;
}
//# sourceMappingURL=index.d.ts.map