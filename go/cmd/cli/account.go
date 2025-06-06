package main

import (
	"context"
	"fmt"

	"github.com/spf13/cobra"
)

var accountCmd = &cobra.Command{
	Use:   "account",
	Short: "Account management commands",
}

var accountInfoCmd = &cobra.Command{
	Use:   "info",
	Short: "Show account information",
	RunE: func(cmd *cobra.Command, args []string) error {
		client, err := getClient()
		if err != nil {
			return err
		}

		account, err := client.GetAccount(context.Background())
		if err != nil {
			return err
		}

		fmt.Printf("Account Information:\n")
		fmt.Printf("  Email: %s\n", account.Email)
		fmt.Printf("  UUID: %s\n", account.UUID)
		fmt.Printf("  Status: %s\n", account.Status)
		fmt.Printf("  Email Verified: %t\n", account.EmailVerified)
		fmt.Printf("  Droplet Limit: %d\n", account.DropletLimit)
		fmt.Printf("  Floating IP Limit: %d\n", account.FloatingIPLimit)
		fmt.Printf("  Volume Limit: %d\n", account.VolumeLimit)

		if account.Team != nil {
			fmt.Printf("  Team: %s (%s)\n", account.Team.Name, account.Team.UUID)
		}

		return nil
	},
}

func init() {
	accountCmd.AddCommand(accountInfoCmd)
}