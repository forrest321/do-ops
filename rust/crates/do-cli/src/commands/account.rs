use anyhow::Result;
use clap::Subcommand;
use do_client::Client;

#[derive(Subcommand)]
pub enum AccountCommand {
    /// Show account information
    Info,
}

pub async fn handle_command(command: AccountCommand) -> Result<()> {
    match command {
        AccountCommand::Info => show_account_info().await,
    }
}

async fn show_account_info() -> Result<()> {
    let client = Client::from_config().await?;
    let account = client.get_account().await?;

    println!("Account Information:");
    println!("  Email: {}", account.email);
    println!("  UUID: {}", account.uuid);
    println!("  Status: {}", account.status);
    println!("  Email Verified: {}", account.email_verified);
    println!("  Droplet Limit: {}", account.droplet_limit);
    println!("  Floating IP Limit: {}", account.floating_ip_limit);
    println!("  Volume Limit: {}", account.volume_limit);

    if let Some(team) = &account.team {
        println!("  Team: {} ({})", team.name, team.uuid);
    }

    Ok(())
}