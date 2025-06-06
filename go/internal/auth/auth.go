package auth

import (
	"fmt"
	"net/http"
)

type Transport struct {
	Token     string
	Transport http.RoundTripper
}

func (t *Transport) RoundTrip(req *http.Request) (*http.Response, error) {
	if t.Token == "" {
		return nil, fmt.Errorf("no authentication token provided")
	}

	req = req.Clone(req.Context())
	req.Header.Set("Authorization", "Bearer "+t.Token)
	req.Header.Set("Content-Type", "application/json")

	transport := t.Transport
	if transport == nil {
		transport = http.DefaultTransport
	}

	return transport.RoundTrip(req)
}

func NewHTTPClient(token string) *http.Client {
	return &http.Client{
		Transport: &Transport{
			Token:     token,
			Transport: http.DefaultTransport,
		},
	}
}