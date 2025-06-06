package types

import "time"

type Droplet struct {
	ID               int               `json:"id"`
	Name             string            `json:"name"`
	Memory           int               `json:"memory"`
	Vcpus            int               `json:"vcpus"`
	Disk             int               `json:"disk"`
	Locked           bool              `json:"locked"`
	Status           string            `json:"status"`
	Kernel           *Kernel           `json:"kernel"`
	CreatedAt        time.Time         `json:"created_at"`
	Features         []string          `json:"features"`
	BackupIDs        []int             `json:"backup_ids"`
	SnapshotIDs      []int             `json:"snapshot_ids"`
	Image            *Image            `json:"image"`
	Size             *Size             `json:"size"`
	SizeSlug         string            `json:"size_slug"`
	Networks         *Networks         `json:"networks"`
	Region           *Region           `json:"region"`
	Tags             []string          `json:"tags"`
	VolumeIDs        []string          `json:"volume_ids"`
	VPCUUID          string            `json:"vpc_uuid"`
}

type Kernel struct {
	ID      int    `json:"id"`
	Name    string `json:"name"`
	Version string `json:"version"`
}

type Image struct {
	ID            int       `json:"id"`
	Name          string    `json:"name"`
	Type          string    `json:"type"`
	Distribution  string    `json:"distribution"`
	Slug          string    `json:"slug"`
	Public        bool      `json:"public"`
	Regions       []string  `json:"regions"`
	MinDiskSize   int       `json:"min_disk_size"`
	SizeGigabytes float64   `json:"size_gigabytes"`
	CreatedAt     time.Time `json:"created_at"`
	Description   string    `json:"description"`
	Tags          []string  `json:"tags"`
	Status        string    `json:"status"`
	ErrorMessage  string    `json:"error_message"`
}

type Networks struct {
	V4 []NetworkV4 `json:"v4"`
	V6 []NetworkV6 `json:"v6"`
}

type NetworkV4 struct {
	IPAddress string `json:"ip_address"`
	Netmask   string `json:"netmask"`
	Gateway   string `json:"gateway"`
	Type      string `json:"type"`
}

type NetworkV6 struct {
	IPAddress string `json:"ip_address"`
	Netmask   int    `json:"netmask"`
	Gateway   string `json:"gateway"`
	Type      string `json:"type"`
}

type CreateDropletRequest struct {
	Name              string   `json:"name"`
	Region            string   `json:"region"`
	Size              string   `json:"size"`
	Image             string   `json:"image"`
	SSHKeys           []string `json:"ssh_keys,omitempty"`
	Backups           bool     `json:"backups,omitempty"`
	IPv6              bool     `json:"ipv6,omitempty"`
	Monitoring        bool     `json:"monitoring,omitempty"`
	Tags              []string `json:"tags,omitempty"`
	UserData          string   `json:"user_data,omitempty"`
	PrivateNetworking bool     `json:"private_networking,omitempty"`
	Volumes           []string `json:"volumes,omitempty"`
	VPCUUID           string   `json:"vpc_uuid,omitempty"`
}

type DropletsResponse struct {
	Droplets []Droplet `json:"droplets"`
	Links    Links     `json:"links"`
	Meta     Meta      `json:"meta"`
}

type DropletResponse struct {
	Droplet Droplet `json:"droplet"`
	Links   Links   `json:"links,omitempty"`
	Meta    Meta    `json:"meta,omitempty"`
}

type CreateDropletResponse struct {
	Droplet Droplet   `json:"droplet"`
	Links   Links     `json:"links,omitempty"`
	Meta    Meta      `json:"meta,omitempty"`
	Actions []Action  `json:"actions,omitempty"`
}