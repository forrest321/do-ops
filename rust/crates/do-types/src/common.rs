use serde::{Deserialize, Serialize};
use chrono::{DateTime, Utc};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Meta {
    pub total: u32,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Links {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub pages: Option<Pages>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Pages {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub first: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub prev: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub next: Option<String>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub last: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ListResponse<T> {
    pub data: Vec<T>,
    pub links: Links,
    pub meta: Meta,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ErrorDetail {
    pub id: String,
    pub message: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub detail: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ErrorResponse {
    pub errors: Vec<ErrorDetail>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Action {
    pub id: u32,
    pub status: String,
    #[serde(rename = "type")]
    pub action_type: String,
    pub started_at: DateTime<Utc>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub completed_at: Option<DateTime<Utc>>,
    pub resource_id: u32,
    pub resource_type: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub region: Option<Region>,
    pub region_slug: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Region {
    pub name: String,
    pub slug: String,
    pub features: Vec<String>,
    pub available: bool,
    pub sizes: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Size {
    pub slug: String,
    pub memory: u32,
    pub vcpus: u32,
    pub disk: u32,
    pub transfer: f64,
    pub price_monthly: f64,
    pub price_hourly: f64,
    pub regions: Vec<String>,
    pub available: bool,
}