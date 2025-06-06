use crate::{Config, Error, Result};
use do_types::*;
use reqwest::{Client as HttpClient, Method, RequestBuilder};
use serde::de::DeserializeOwned;
use serde::Serialize;
use url::Url;

pub struct Client {
    http_client: HttpClient,
    config: Config,
}

impl Client {
    pub fn new(config: Config) -> Result<Self> {
        config.validate()?;
        
        let http_client = HttpClient::builder()
            .user_agent("do-client/0.1.0")
            .build()?;

        Ok(Self {
            http_client,
            config,
        })
    }

    pub async fn from_config() -> Result<Self> {
        let config = Config::load().await?;
        Self::new(config)
    }

    async fn request<T, B>(&self, method: Method, endpoint: &str, body: Option<&B>) -> Result<T>
    where
        T: DeserializeOwned,
        B: Serialize,
    {
        let url = Url::parse(&self.config.base_url)?
            .join(endpoint)?;

        let mut request = self.http_client
            .request(method, url)
            .header("Authorization", format!("Bearer {}", self.config.token))
            .header("Content-Type", "application/json");

        if let Some(body) = body {
            request = request.json(body);
        }

        let response = request.send().await?;
        let status = response.status();

        if !status.is_success() {
            let error_text = response.text().await.unwrap_or_else(|_| "Unknown error".to_string());
            
            // Try to parse as API error response
            if let Ok(error_response) = serde_json::from_str::<ErrorResponse>(&error_text) {
                if let Some(first_error) = error_response.errors.first() {
                    return Err(Error::Api {
                        status: status.as_u16(),
                        message: first_error.message.clone(),
                    });
                }
            }

            return Err(Error::Api {
                status: status.as_u16(),
                message: error_text,
            });
        }

        let text = response.text().await?;
        if text.is_empty() {
            // For DELETE requests that return no content
            return Ok(serde_json::from_str("{}")?);
        }

        Ok(serde_json::from_str(&text)?)
    }

    // Account methods
    pub async fn get_account(&self) -> Result<Account> {
        let response: AccountResponse = self.request(Method::GET, "/v2/account", None::<&()>).await?;
        Ok(response.account)
    }

    // Droplet methods
    pub async fn list_droplets(&self) -> Result<Vec<Droplet>> {
        let response: DropletsResponse = self.request(Method::GET, "/v2/droplets", None::<&()>).await?;
        Ok(response.droplets)
    }

    pub async fn get_droplet(&self, id: u32) -> Result<Droplet> {
        let endpoint = format!("/v2/droplets/{}", id);
        let response: DropletResponse = self.request(Method::GET, &endpoint, None::<&()>).await?;
        Ok(response.droplet)
    }

    pub async fn create_droplet(&self, request: &CreateDropletRequest) -> Result<Droplet> {
        let response: CreateDropletResponse = self.request(Method::POST, "/v2/droplets", Some(request)).await?;
        Ok(response.droplet)
    }

    pub async fn delete_droplet(&self, id: u32) -> Result<()> {
        let endpoint = format!("/v2/droplets/{}", id);
        let _: serde_json::Value = self.request(Method::DELETE, &endpoint, None::<&()>).await?;
        Ok(())
    }

    // Configuration methods
    pub fn config(&self) -> &Config {
        &self.config
    }

    pub fn update_config(&mut self, config: Config) -> Result<()> {
        config.validate()?;
        self.config = config;
        Ok(())
    }
}