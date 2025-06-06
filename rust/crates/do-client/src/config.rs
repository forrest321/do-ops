use crate::{Error, Result};
use serde::{Deserialize, Serialize};
use std::path::PathBuf;

const DEFAULT_BASE_URL: &str = "https://api.digitalocean.com";
const CONFIG_FILE_NAME: &str = "config.toml";

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Config {
    pub token: String,
    pub base_url: String,
}

impl Config {
    pub fn new(token: String) -> Self {
        Self {
            token,
            base_url: DEFAULT_BASE_URL.to_string(),
        }
    }

    pub fn with_base_url(mut self, base_url: String) -> Self {
        self.base_url = base_url;
        self
    }

    pub fn from_env() -> Result<Self> {
        let token = std::env::var("DIGITALOCEAN_TOKEN")
            .map_err(|_| Error::config("DIGITALOCEAN_TOKEN environment variable not set"))?;
        
        let base_url = std::env::var("DIGITALOCEAN_BASE_URL")
            .unwrap_or_else(|_| DEFAULT_BASE_URL.to_string());

        Ok(Self { token, base_url })
    }

    pub async fn load() -> Result<Self> {
        // Try environment variables first
        if let Ok(config) = Self::from_env() {
            return Ok(config);
        }

        // Try config file
        match Self::load_from_file().await {
            Ok(mut config) => {
                // Still check for environment overrides
                if let Ok(token) = std::env::var("DIGITALOCEAN_TOKEN") {
                    config.token = token;
                }
                if let Ok(base_url) = std::env::var("DIGITALOCEAN_BASE_URL") {
                    config.base_url = base_url;
                }
                Ok(config)
            }
            Err(_) => Err(Error::config(
                "No configuration found. Set DIGITALOCEAN_TOKEN environment variable or create config file."
            )),
        }
    }

    async fn load_from_file() -> Result<Self> {
        let config_path = get_config_path()?;
        let content = tokio::fs::read_to_string(config_path).await?;
        let mut config: Self = toml::from_str(&content)
            .map_err(|e| Error::config(format!("Invalid config file: {}", e)))?;

        if config.base_url.is_empty() {
            config.base_url = DEFAULT_BASE_URL.to_string();
        }

        Ok(config)
    }

    pub async fn save(&self) -> Result<()> {
        let config_path = get_config_path()?;
        
        // Create config directory if it doesn't exist
        if let Some(parent) = config_path.parent() {
            tokio::fs::create_dir_all(parent).await?;
        }

        let content = toml::to_string_pretty(self)
            .map_err(|e| Error::config(format!("Failed to serialize config: {}", e)))?;
        
        tokio::fs::write(config_path, content).await?;
        Ok(())
    }

    pub fn validate(&self) -> Result<()> {
        if self.token.is_empty() {
            return Err(Error::MissingToken);
        }
        
        url::Url::parse(&self.base_url)
            .map_err(|_| Error::config("Invalid base URL"))?;
        
        Ok(())
    }
}

fn get_config_path() -> Result<PathBuf> {
    let config_dir = dirs::config_dir()
        .ok_or_else(|| Error::config("Unable to determine config directory"))?;
    
    Ok(config_dir.join("do-cli").join(CONFIG_FILE_NAME))
}