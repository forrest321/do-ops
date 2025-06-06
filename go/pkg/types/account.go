package types

type Account struct {
	DropletLimit        int    `json:"droplet_limit"`
	FloatingIPLimit     int    `json:"floating_ip_limit"`
	VolumeLimit         int    `json:"volume_limit"`
	Email               string `json:"email"`
	UUID                string `json:"uuid"`
	EmailVerified       bool   `json:"email_verified"`
	Status              string `json:"status"`
	StatusMessage       string `json:"status_message"`
	Team                *Team  `json:"team,omitempty"`
}

type Team struct {
	UUID string `json:"uuid"`
	Name string `json:"name"`
}

type AccountResponse struct {
	Account Account `json:"account"`
}