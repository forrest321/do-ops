use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Account {
    pub droplet_limit: u32,
    pub floating_ip_limit: u32,
    pub volume_limit: u32,
    pub email: String,
    pub uuid: String,
    pub email_verified: bool,
    pub status: String,
    pub status_message: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub team: Option<Team>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Team {
    pub uuid: String,
    pub name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AccountResponse {
    pub account: Account,
}