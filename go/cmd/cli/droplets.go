package main

import (
	"context"
	"fmt"
	"strconv"
	"strings"
	"time"

	"github.com/fo/do-op/go/pkg/types"
	"github.com/spf13/cobra"
)

var dropletsCmd = &cobra.Command{
	Use:   "droplets",
	Short: "Droplet management commands",
}

var dropletsListCmd = &cobra.Command{
	Use:   "list",
	Short: "List all droplets",
	RunE: func(cmd *cobra.Command, args []string) error {
		client, err := getClient()
		if err != nil {
			return err
		}

		droplets, err := client.ListDroplets(context.Background())
		if err != nil {
			return err
		}

		if len(droplets) == 0 {
			fmt.Println("No droplets found")
			return nil
		}

		fmt.Printf("%-8s %-20s %-10s %-12s %-10s %-15s %s\n", 
			"ID", "NAME", "STATUS", "SIZE", "REGION", "IP", "CREATED")
		fmt.Printf("%-8s %-20s %-10s %-12s %-10s %-15s %s\n", 
			"--", "----", "------", "----", "------", "--", "-------")

		for _, droplet := range droplets {
			ip := ""
			if droplet.Networks != nil && len(droplet.Networks.V4) > 0 {
				for _, net := range droplet.Networks.V4 {
					if net.Type == "public" {
						ip = net.IPAddress
						break
					}
				}
			}

			created := droplet.CreatedAt.Format("2006-01-02")
			region := ""
			if droplet.Region != nil {
				region = droplet.Region.Slug
			}

			fmt.Printf("%-8d %-20s %-10s %-12s %-10s %-15s %s\n",
				droplet.ID, droplet.Name, droplet.Status, droplet.SizeSlug,
				region, ip, created)
		}

		return nil
	},
}

var dropletsGetCmd = &cobra.Command{
	Use:   "get <id>",
	Short: "Get droplet details",
	Args:  cobra.ExactArgs(1),
	RunE: func(cmd *cobra.Command, args []string) error {
		id, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("invalid droplet ID: %s", args[0])
		}

		client, err := getClient()
		if err != nil {
			return err
		}

		droplet, err := client.GetDroplet(context.Background(), id)
		if err != nil {
			return err
		}

		fmt.Printf("Droplet Details:\n")
		fmt.Printf("  ID: %d\n", droplet.ID)
		fmt.Printf("  Name: %s\n", droplet.Name)
		fmt.Printf("  Status: %s\n", droplet.Status)
		fmt.Printf("  Memory: %d MB\n", droplet.Memory)
		fmt.Printf("  VCPUs: %d\n", droplet.Vcpus)
		fmt.Printf("  Disk: %d GB\n", droplet.Disk)
		fmt.Printf("  Locked: %t\n", droplet.Locked)
		fmt.Printf("  Created: %s\n", droplet.CreatedAt.Format(time.RFC3339))

		if droplet.Image != nil {
			fmt.Printf("  Image: %s (%s)\n", droplet.Image.Name, droplet.Image.Distribution)
		}

		if droplet.Size != nil {
			fmt.Printf("  Size: %s ($%.2f/month)\n", droplet.Size.Slug, droplet.Size.PriceMonthly)
		}

		if droplet.Region != nil {
			fmt.Printf("  Region: %s (%s)\n", droplet.Region.Name, droplet.Region.Slug)
		}

		if droplet.Networks != nil {
			fmt.Printf("  Networks:\n")
			for _, net := range droplet.Networks.V4 {
				fmt.Printf("    %s: %s\n", net.Type, net.IPAddress)
			}
			for _, net := range droplet.Networks.V6 {
				fmt.Printf("    %s (v6): %s\n", net.Type, net.IPAddress)
			}
		}

		if len(droplet.Tags) > 0 {
			fmt.Printf("  Tags: %s\n", strings.Join(droplet.Tags, ", "))
		}

		return nil
	},
}

var dropletsCreateCmd = &cobra.Command{
	Use:   "create",
	Short: "Create a new droplet",
	RunE: func(cmd *cobra.Command, args []string) error {
		name, _ := cmd.Flags().GetString("name")
		region, _ := cmd.Flags().GetString("region")
		size, _ := cmd.Flags().GetString("size")
		image, _ := cmd.Flags().GetString("image")
		tags, _ := cmd.Flags().GetStringSlice("tags")

		if name == "" || region == "" || size == "" || image == "" {
			return fmt.Errorf("name, region, size, and image are required")
		}

		client, err := getClient()
		if err != nil {
			return err
		}

		req := &types.CreateDropletRequest{
			Name:   name,
			Region: region,
			Size:   size,
			Image:  image,
			Tags:   tags,
		}

		droplet, err := client.CreateDroplet(context.Background(), req)
		if err != nil {
			return err
		}

		fmt.Printf("Droplet created successfully!\n")
		fmt.Printf("  ID: %d\n", droplet.ID)
		fmt.Printf("  Name: %s\n", droplet.Name)
		fmt.Printf("  Status: %s\n", droplet.Status)
		fmt.Printf("  Region: %s\n", region)
		fmt.Printf("  Size: %s\n", size)

		return nil
	},
}

var dropletsDeleteCmd = &cobra.Command{
	Use:   "delete <id>",
	Short: "Delete a droplet",
	Args:  cobra.ExactArgs(1),
	RunE: func(cmd *cobra.Command, args []string) error {
		id, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("invalid droplet ID: %s", args[0])
		}

		client, err := getClient()
		if err != nil {
			return err
		}

		fmt.Printf("Are you sure you want to delete droplet %d? (y/N): ", id)
		var confirm string
		fmt.Scanln(&confirm)

		if strings.ToLower(confirm) != "y" && strings.ToLower(confirm) != "yes" {
			fmt.Println("Cancelled")
			return nil
		}

		if err := client.DeleteDroplet(context.Background(), id); err != nil {
			return err
		}

		fmt.Printf("Droplet %d deleted successfully\n", id)
		return nil
	},
}

func init() {
	dropletsCmd.AddCommand(dropletsListCmd)
	dropletsCmd.AddCommand(dropletsGetCmd)
	dropletsCmd.AddCommand(dropletsCreateCmd)
	dropletsCmd.AddCommand(dropletsDeleteCmd)

	dropletsCreateCmd.Flags().StringP("name", "n", "", "Droplet name (required)")
	dropletsCreateCmd.Flags().StringP("region", "r", "", "Region slug (required)")
	dropletsCreateCmd.Flags().StringP("size", "s", "", "Size slug (required)")
	dropletsCreateCmd.Flags().StringP("image", "i", "", "Image slug or ID (required)")
	dropletsCreateCmd.Flags().StringSliceP("tags", "t", []string{}, "Tags for the droplet")
}