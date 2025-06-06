use serde::{Deserialize, Serialize};
use chrono::{DateTime, Utc};
use super::{Region, Size, Links, Meta, Action};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Droplet {
    pub id: u32,
    pub name: String,
    pub memory: u32,
    pub vcpus: u32,
    pub disk: u32,
    pub locked: bool,
    pub status: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub kernel: Option<Kernel>,
    pub created_at: DateTime<Utc>,
    pub features: Vec<String>,
    pub backup_ids: Vec<u32>,
    pub snapshot_ids: Vec<u32>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub image: Option<Image>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub size: Option<Size>,
    pub size_slug: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub networks: Option<Networks>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub region: Option<Region>,
    pub tags: Vec<String>,
    pub volume_ids: Vec<String>,
    pub vpc_uuid: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Kernel {
    pub id: u32,
    pub name: String,
    pub version: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Image {
    pub id: u32,
    pub name: String,
    #[serde(rename = "type")]
    pub image_type: String,
    pub distribution: String,
    pub slug: String,
    pub public: bool,
    pub regions: Vec<String>,
    pub min_disk_size: u32,
    pub size_gigabytes: f64,
    pub created_at: DateTime<Utc>,
    pub description: String,
    pub tags: Vec<String>,
    pub status: String,
    pub error_message: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Networks {
    pub v4: Vec<NetworkV4>,
    pub v6: Vec<NetworkV6>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct NetworkV4 {
    pub ip_address: String,
    pub netmask: String,
    pub gateway: String,
    #[serde(rename = "type")]
    pub network_type: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct NetworkV6 {
    pub ip_address: String,
    pub netmask: u32,
    pub gateway: String,
    #[serde(rename = "type")]
    pub network_type: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct CreateDropletRequest {
    pub name: String,
    pub region: String,
    pub size: String,
    pub image: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ssh_keys: Option<Vec<String>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub backups: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub ipv6: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub monitoring: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub tags: Option<Vec<String>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub user_data: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub private_networking: Option<bool>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub volumes: Option<Vec<String>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub vpc_uuid: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DropletsResponse {
    pub droplets: Vec<Droplet>,
    pub links: Links,
    pub meta: Meta,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DropletResponse {
    pub droplet: Droplet,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub links: Option<Links>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub meta: Option<Meta>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct CreateDropletResponse {
    pub droplet: Droplet,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub links: Option<Links>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub meta: Option<Meta>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub actions: Option<Vec<Action>>,
}