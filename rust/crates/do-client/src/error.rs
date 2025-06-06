use thiserror::Error;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Error, Debug)]
pub enum Error {
    #[error("HTTP request failed: {0}")]
    Http(#[from] reqwest::Error),

    #[error("JSON serialization failed: {0}")]
    Json(#[from] serde_json::Error),

    #[error("IO error: {0}")]
    Io(#[from] std::io::Error),

    #[error("Configuration error: {0}")]
    Config(String),

    #[error("API error ({status}): {message}")]
    Api { status: u16, message: String },

    #[error("Authentication token is required")]
    MissingToken,

    #[error("Invalid URL: {0}")]
    InvalidUrl(#[from] url::ParseError),

    #[error("{0}")]
    Other(String),
}

impl Error {
    pub fn config<T: Into<String>>(msg: T) -> Self {
        Self::Config(msg.into())
    }

    pub fn other<T: Into<String>>(msg: T) -> Self {
        Self::Other(msg.into())
    }
}