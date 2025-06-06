# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This project analyzes the public DigitalOcean API specification and generates tools to use it in various programming languages. The core asset is the DigitalOcean OpenAPI v3.0 specification file (`DigitalOcean-public.v2.yaml`).

## Key Files

- `DigitalOcean-public.v2.yaml` - Complete DigitalOcean API v2.0 OpenAPI specification (2MB+ file)
- `README.md` - Basic project description
- `best-practices.txt` - Comprehensive guide for effective Claude Code usage
- `claude-instance-instructions.md` - Instructions for concurrent development with multiple Claude instances

## Development Architecture

This appears to be an early-stage project focused on API specification analysis and code generation. The project will likely involve:

1. **API Specification Parsing** - Processing the large OpenAPI YAML file
2. **Code Generation** - Creating client libraries for multiple programming languages
3. **Tool Generation** - Building utilities to interact with DigitalOcean services

## Working with the OpenAPI Specification

The DigitalOcean API specification is extensive (2MB+). When working with it:
- Use `Grep` tool to search for specific endpoints or schemas
- Use `Read` tool with offset/limit parameters for large file sections
- The API covers all DigitalOcean services: Droplets, Volumes, Load Balancers, etc.

## Multi-Instance Development

This repository includes comprehensive instructions for concurrent development with multiple Claude instances. Key practices:
- Use git worktrees for parallel development
- Follow branch naming conventions (instance-1/feature-name)
- Include instance identifiers in commit messages
- Coordinate file-level work to avoid conflicts

## Best Practices Integration

The repository includes detailed best practices documentation covering:
- CLAUDE.md file optimization
- Tool usage and workflow patterns
- Multi-Claude coordination strategies
- Headless mode automation

When developing new features, consult `best-practices.txt` for workflow guidance and `claude-instance-instructions.md` for coordination protocols.