use anyhow::Result;
use clap::Subcommand;
use do_client::Config;

#[derive(Subcommand)]
pub enum ConfigCommand {
    /// Set a configuration value
    Set {
        /// Configuration key (token, base-url)
        key: String,
        /// Configuration value
        value: String,
    },
    /// Get configuration value(s)
    Get {
        /// Configuration key (optional)
        key: Option<String>,
    },
}

pub async fn handle_command(command: ConfigCommand) -> Result<()> {
    match command {
        ConfigCommand::Set { key, value } => set_config(key, value).await,
        ConfigCommand::Get { key } => get_config(key).await,
    }
}

async fn set_config(key: String, value: String) -> Result<()> {
    let mut config = Config::load().await.unwrap_or_else(|_| {
        Config::new(String::new())
    });

    match key.as_str() {
        "token" => config.token = value.clone(),
        "base-url" => config.base_url = value.clone(),
        _ => {
            eprintln!("Error: Unknown config key: {}", key);
            std::process::exit(1);
        }
    }

    config.save().await?;
    println!("Configuration updated: {} = {}", key, value);
    Ok(())
}

async fn get_config(key: Option<String>) -> Result<()> {
    let config = Config::load().await?;

    match key {
        Some(key) => match key.as_str() {
            "token" => println!("token: {}", mask_token(&config.token)),
            "base-url" => println!("base-url: {}", config.base_url),
            _ => {
                eprintln!("Error: Unknown config key: {}", key);
                std::process::exit(1);
            }
        },
        None => {
            println!("Configuration:");
            println!("  token: {}", mask_token(&config.token));
            println!("  base-url: {}", config.base_url);
        }
    }

    Ok(())
}

fn mask_token(token: &str) -> String {
    if token.is_empty() {
        "<not set>".to_string()
    } else if token.len() <= 8 {
        "***".to_string()
    } else {
        format!("{}...{}", &token[..4], &token[token.len() - 4..])
    }
}