use anyhow::Result;
use clap::Subcommand;
use comfy_table::{Table, presets::UTF8_FULL};
use console::style;
use dialoguer::Confirm;
use do_client::Client;
use do_types::CreateDropletRequest;

#[derive(Subcommand)]
pub enum DropletsCommand {
    /// List all droplets
    List,
    /// Get droplet details
    Get {
        /// Droplet ID
        id: u32,
    },
    /// Create a new droplet
    Create {
        /// Droplet name
        #[arg(short, long)]
        name: String,
        /// Region slug
        #[arg(short, long)]
        region: String,
        /// Size slug
        #[arg(short, long)]
        size: String,
        /// Image slug or ID
        #[arg(short, long)]
        image: String,
        /// Tags for the droplet
        #[arg(short, long)]
        tags: Vec<String>,
    },
    /// Delete a droplet
    Delete {
        /// Droplet ID
        id: u32,
    },
}

pub async fn handle_command(command: DropletsCommand) -> Result<()> {
    match command {
        DropletsCommand::List => list_droplets().await,
        DropletsCommand::Get { id } => get_droplet(id).await,
        DropletsCommand::Create { name, region, size, image, tags } => {
            create_droplet(name, region, size, image, tags).await
        }
        DropletsCommand::Delete { id } => delete_droplet(id).await,
    }
}

async fn list_droplets() -> Result<()> {
    let client = Client::from_config().await?;
    let droplets = client.list_droplets().await?;

    if droplets.is_empty() {
        println!("No droplets found");
        return Ok(());
    }

    let mut table = Table::new();
    table.load_preset(UTF8_FULL);
    table.set_header(vec!["ID", "NAME", "STATUS", "SIZE", "REGION", "IP", "CREATED"]);

    for droplet in droplets {
        let ip = droplet.networks
            .as_ref()
            .and_then(|networks| networks.v4.iter().find(|net| net.network_type == "public"))
            .map(|net| net.ip_address.clone())
            .unwrap_or_else(|| "-".to_string());

        let region = droplet.region
            .as_ref()
            .map(|r| r.slug.clone())
            .unwrap_or_else(|| "-".to_string());

        let created = droplet.created_at.format("%Y-%m-%d").to_string();

        table.add_row(vec![
            droplet.id.to_string(),
            droplet.name,
            droplet.status,
            droplet.size_slug,
            region,
            ip,
            created,
        ]);
    }

    println!("{}", table);
    Ok(())
}

async fn get_droplet(id: u32) -> Result<()> {
    let client = Client::from_config().await?;
    let droplet = client.get_droplet(id).await?;

    println!("{}", style("Droplet Details:").bold());
    println!("  ID: {}", droplet.id);
    println!("  Name: {}", droplet.name);
    println!("  Status: {}", style(&droplet.status).cyan());
    println!("  Memory: {} MB", droplet.memory);
    println!("  VCPUs: {}", droplet.vcpus);
    println!("  Disk: {} GB", droplet.disk);
    println!("  Locked: {}", droplet.locked);
    println!("  Created: {}", droplet.created_at.format("%Y-%m-%d %H:%M:%S UTC"));

    if let Some(image) = &droplet.image {
        println!("  Image: {} ({})", image.name, image.distribution);
    }

    if let Some(size) = &droplet.size {
        println!("  Size: {} (${:.2}/month)", size.slug, size.price_monthly);
    }

    if let Some(region) = &droplet.region {
        println!("  Region: {} ({})", region.name, region.slug);
    }

    if let Some(networks) = &droplet.networks {
        println!("  Networks:");
        for net in &networks.v4 {
            println!("    {}: {}", net.network_type, net.ip_address);
        }
        for net in &networks.v6 {
            println!("    {} (v6): {}", net.network_type, net.ip_address);
        }
    }

    if !droplet.tags.is_empty() {
        println!("  Tags: {}", droplet.tags.join(", "));
    }

    Ok(())
}

async fn create_droplet(name: String, region: String, size: String, image: String, tags: Vec<String>) -> Result<()> {
    let client = Client::from_config().await?;

    let request = CreateDropletRequest {
        name: name.clone(),
        region: region.clone(),
        size: size.clone(),
        image: image.clone(),
        tags: if tags.is_empty() { None } else { Some(tags) },
        ssh_keys: None,
        backups: None,
        ipv6: None,
        monitoring: None,
        user_data: None,
        private_networking: None,
        volumes: None,
        vpc_uuid: None,
    };

    let droplet = client.create_droplet(&request).await?;

    println!("{}", style("Droplet created successfully!").green().bold());
    println!("  ID: {}", droplet.id);
    println!("  Name: {}", droplet.name);
    println!("  Status: {}", style(&droplet.status).cyan());
    println!("  Region: {}", region);
    println!("  Size: {}", size);

    Ok(())
}

async fn delete_droplet(id: u32) -> Result<()> {
    let confirm = Confirm::new()
        .with_prompt(format!("Are you sure you want to delete droplet {}?", id))
        .default(false)
        .interact()?;

    if !confirm {
        println!("Cancelled");
        return Ok(());
    }

    let client = Client::from_config().await?;
    client.delete_droplet(id).await?;

    println!("{}", style(format!("Droplet {} deleted successfully", id)).green());
    Ok(())
}