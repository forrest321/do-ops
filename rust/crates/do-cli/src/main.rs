mod commands;

use anyhow::Result;
use clap::{Parser, Subcommand};
use commands::{account, config, droplets};

#[derive(Parser)]
#[command(name = "do-cli")]
#[command(about = "DigitalOcean CLI client")]
#[command(version)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Account management commands
    Account {
        #[command(subcommand)]
        command: account::AccountCommand,
    },
    /// Droplet management commands
    Droplets {
        #[command(subcommand)]
        command: droplets::DropletsCommand,
    },
    /// Configuration management commands
    Config {
        #[command(subcommand)]
        command: config::ConfigCommand,
    },
}

#[tokio::main]
async fn main() -> Result<()> {
    let cli = Cli::parse();

    match cli.command {
        Commands::Account { command } => account::handle_command(command).await,
        Commands::Droplets { command } => droplets::handle_command(command).await,
        Commands::Config { command } => config::handle_command(command).await,
    }
}