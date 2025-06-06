package main

import (
	"fmt"

	"github.com/spf13/cobra"
)

var configCmd = &cobra.Command{
	Use:   "config",
	Short: "Configuration management commands",
}

var configSetCmd = &cobra.Command{
	Use:   "set <key> <value>",
	Short: "Set a configuration value",
	Args:  cobra.ExactArgs(2),
	RunE: func(cmd *cobra.Command, args []string) error {
		key := args[0]
		value := args[1]

		switch key {
		case "token":
			cfg.Token = value
		case "base-url":
			cfg.BaseURL = value
		default:
			return fmt.Errorf("unknown config key: %s", key)
		}

		if err := cfg.Save(); err != nil {
			return fmt.Errorf("failed to save config: %w", err)
		}

		fmt.Printf("Configuration updated: %s = %s\n", key, value)
		return nil
	},
}

var configGetCmd = &cobra.Command{
	Use:   "get [key]",
	Short: "Get configuration value(s)",
	Args:  cobra.MaximumNArgs(1),
	RunE: func(cmd *cobra.Command, args []string) error {
		if len(args) == 0 {
			fmt.Printf("Configuration:\n")
			fmt.Printf("  token: %s\n", maskToken(cfg.Token))
			fmt.Printf("  base-url: %s\n", cfg.BaseURL)
			return nil
		}

		key := args[0]
		switch key {
		case "token":
			fmt.Printf("token: %s\n", maskToken(cfg.Token))
		case "base-url":
			fmt.Printf("base-url: %s\n", cfg.BaseURL)
		default:
			return fmt.Errorf("unknown config key: %s", key)
		}

		return nil
	},
}

func maskToken(token string) string {
	if token == "" {
		return "<not set>"
	}
	if len(token) <= 8 {
		return "***"
	}
	return token[:4] + "..." + token[len(token)-4:]
}

func init() {
	configCmd.AddCommand(configSetCmd)
	configCmd.AddCommand(configGetCmd)
}