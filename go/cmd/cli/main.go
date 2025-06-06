package main

import (
	"fmt"
	"os"

	"github.com/fo/do-op/go/pkg/client"
	"github.com/fo/do-op/go/pkg/config"
	"github.com/spf13/cobra"
)

var (
	cfgFile string
	cfg     *config.Config
)

func main() {
	if err := rootCmd.Execute(); err != nil {
		fmt.Fprintf(os.Stderr, "Error: %v\n", err)
		os.Exit(1)
	}
}

var rootCmd = &cobra.Command{
	Use:   "do-cli",
	Short: "DigitalOcean CLI client",
	Long:  "A command line interface for the DigitalOcean API",
	PersistentPreRunE: func(cmd *cobra.Command, args []string) error {
		var err error
		cfg, err = config.Load()
		if err != nil {
			return fmt.Errorf("failed to load config: %w", err)
		}
		return nil
	},
}

func init() {
	rootCmd.AddCommand(accountCmd)
	rootCmd.AddCommand(dropletsCmd)
	rootCmd.AddCommand(configCmd)
}

func getClient() (*client.Client, error) {
	return client.New(cfg)
}

func handleError(err error) {
	fmt.Fprintf(os.Stderr, "Error: %v\n", err)
	os.Exit(1)
}