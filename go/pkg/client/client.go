package client

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"path"

	"github.com/fo/do-op/go/internal/auth"
	"github.com/fo/do-op/go/pkg/config"
	"github.com/fo/do-op/go/pkg/types"
)

type Client struct {
	httpClient *http.Client
	baseURL    string
}

func New(cfg *config.Config) (*Client, error) {
	if cfg.Token == "" {
		return nil, fmt.Errorf("authentication token is required")
	}

	return &Client{
		httpClient: auth.NewHTTPClient(cfg.Token),
		baseURL:    cfg.BaseURL,
	}, nil
}

func (c *Client) doRequest(ctx context.Context, method, endpoint string, body interface{}, result interface{}) error {
	var reqBody io.Reader
	if body != nil {
		jsonData, err := json.Marshal(body)
		if err != nil {
			return fmt.Errorf("failed to marshal request body: %w", err)
		}
		reqBody = bytes.NewReader(jsonData)
	}

	url, err := url.JoinPath(c.baseURL, endpoint)
	if err != nil {
		return fmt.Errorf("failed to build URL: %w", err)
	}

	req, err := http.NewRequestWithContext(ctx, method, url, reqBody)
	if err != nil {
		return fmt.Errorf("failed to create request: %w", err)
	}

	resp, err := c.httpClient.Do(req)
	if err != nil {
		return fmt.Errorf("request failed: %w", err)
	}
	defer resp.Body.Close()

	respBody, err := io.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("failed to read response body: %w", err)
	}

	if resp.StatusCode < 200 || resp.StatusCode >= 300 {
		var errResp types.ErrorResponse
		if err := json.Unmarshal(respBody, &errResp); err != nil {
			return fmt.Errorf("API error (status %d): %s", resp.StatusCode, string(respBody))
		}
		
		if len(errResp.Errors) > 0 {
			return fmt.Errorf("API error: %s", errResp.Errors[0].Message)
		}
		return fmt.Errorf("API error (status %d)", resp.StatusCode)
	}

	if result != nil {
		if err := json.Unmarshal(respBody, result); err != nil {
			return fmt.Errorf("failed to unmarshal response: %w", err)
		}
	}

	return nil
}

func (c *Client) GetAccount(ctx context.Context) (*types.Account, error) {
	var resp types.AccountResponse
	if err := c.doRequest(ctx, "GET", "/v2/account", nil, &resp); err != nil {
		return nil, err
	}
	return &resp.Account, nil
}

func (c *Client) ListDroplets(ctx context.Context) ([]types.Droplet, error) {
	var resp types.DropletsResponse
	if err := c.doRequest(ctx, "GET", "/v2/droplets", nil, &resp); err != nil {
		return nil, err
	}
	return resp.Droplets, nil
}

func (c *Client) GetDroplet(ctx context.Context, id int) (*types.Droplet, error) {
	endpoint := path.Join("/v2/droplets", fmt.Sprintf("%d", id))
	var resp types.DropletResponse
	if err := c.doRequest(ctx, "GET", endpoint, nil, &resp); err != nil {
		return nil, err
	}
	return &resp.Droplet, nil
}

func (c *Client) CreateDroplet(ctx context.Context, req *types.CreateDropletRequest) (*types.Droplet, error) {
	var resp types.CreateDropletResponse
	if err := c.doRequest(ctx, "POST", "/v2/droplets", req, &resp); err != nil {
		return nil, err
	}
	return &resp.Droplet, nil
}

func (c *Client) DeleteDroplet(ctx context.Context, id int) error {
	endpoint := path.Join("/v2/droplets", fmt.Sprintf("%d", id))
	return c.doRequest(ctx, "DELETE", endpoint, nil, nil)
}