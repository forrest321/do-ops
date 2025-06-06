package types

import "time"

type Meta struct {
	Total int `json:"total"`
}

type Links struct {
	Pages *Pages `json:"pages,omitempty"`
}

type Pages struct {
	First string `json:"first,omitempty"`
	Prev  string `json:"prev,omitempty"`
	Next  string `json:"next,omitempty"`
	Last  string `json:"last,omitempty"`
}

type ListResponse[T any] struct {
	Data  []T   `json:"data"`
	Links Links `json:"links"`
	Meta  Meta  `json:"meta"`
}

type Error struct {
	ID      string `json:"id"`
	Message string `json:"message"`
	Detail  string `json:"detail,omitempty"`
}

type ErrorResponse struct {
	Errors []Error `json:"errors"`
}

type Action struct {
	ID           int       `json:"id"`
	Status       string    `json:"status"`
	Type         string    `json:"type"`
	StartedAt    time.Time `json:"started_at"`
	CompletedAt  *time.Time `json:"completed_at"`
	ResourceID   int       `json:"resource_id"`
	ResourceType string    `json:"resource_type"`
	Region       *Region   `json:"region"`
	RegionSlug   string    `json:"region_slug"`
}

type Region struct {
	Name      string   `json:"name"`
	Slug      string   `json:"slug"`
	Features  []string `json:"features"`
	Available bool     `json:"available"`
	Sizes     []string `json:"sizes"`
}

type Size struct {
	Slug         string  `json:"slug"`
	Memory       int     `json:"memory"`
	Vcpus        int     `json:"vcpus"`
	Disk         int     `json:"disk"`
	Transfer     float64 `json:"transfer"`
	PriceMonthly float64 `json:"price_monthly"`
	PriceHourly  float64 `json:"price_hourly"`
	Regions      []string `json:"regions"`
	Available    bool    `json:"available"`
}