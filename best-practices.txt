Claude Code: Best practices for agentic coding

This document contains best practices extracted from the Anthropic documentation for effectively using Claude Code in development workflows.

## 1. Customize your setup

### Create CLAUDE.md files for documentation
- Document project-specific information, conventions, and context
- Include setup instructions and development workflows
- Provide context about the codebase structure and patterns

### Tune your CLAUDE.md files
- Keep documentation up-to-date and relevant
- Include specific examples and use cases
- Document any custom processes or requirements

### Curate Claude's list of allowed tools
- Enable only the tools you need for your workflow
- Consider security implications of tool permissions
- Regularly review and update tool access

### Install the gh CLI for GitHub integration
- Enables direct GitHub operations from Claude
- Streamlines PR creation and issue management
- Facilitates repository interactions

## 2. Give Claude more tools

### Use Claude with bash tools
- Leverage command-line tools for enhanced capabilities
- Automate common development tasks
- Integrate with existing shell scripts and workflows

### Use Claude with MCP (Model Context Protocol)
- Extend Claude's capabilities with custom integrations
- Connect to external services and APIs
- Create domain-specific tool extensions

### Use custom slash commands
- Create shortcuts for frequently used operations
- Standardize common workflows across projects
- Improve efficiency and consistency

## 3. Try common workflows

### Explore, plan, code, commit
- A versatile workflow suitable for many development problems
- Start by exploring the codebase to understand context
- Plan the implementation approach
- Write code following the plan
- Commit changes with meaningful messages

### Write tests, commit; code, iterate, commit
- Test-driven development approach
- Write tests first to define expected behavior
- Implement code to satisfy the tests
- Iterate and refine based on test feedback
- Commit frequently with atomic changes

### Write code, screenshot result, iterate
- Visual feedback workflow for UI/UX development
- Implement changes and capture visual results
- Review screenshots for correctness
- Iterate based on visual feedback
- Particularly useful for frontend development

### Safe YOLO mode
- Uninterrupted execution with appropriate safety measures
- Allow Claude to work autonomously on well-defined tasks
- Implement safeguards and checkpoints
- Monitor progress and intervene when necessary

### Codebase Q&A
- Learning and exploration workflow
- Ask questions about code structure and functionality
- Understand existing patterns and conventions
- Document findings for future reference

### Use Claude to interact with git
- Leverage Claude for git operations and history analysis
- Automate common git workflows
- Get insights from commit history and branch analysis
- Streamline merge conflict resolution

### Use Claude to interact with GitHub
- Automate PR creation and management
- Handle issue triage and responses
- Integrate with GitHub workflows and actions
- Streamline code review processes

### Use Claude to work with Jupyter notebooks
- Data science and analysis workflows
- Interactive development and experimentation
- Documentation and visualization creation
- Collaborative research and development

## 4. Optimize your workflow

### Be specific in your instructions
- Provide clear, detailed requirements
- Include context and constraints
- Specify expected outcomes and formats
- Avoid ambiguous or vague requests

### Give Claude images for visual context
- Include screenshots for UI/UX work
- Provide diagrams for architectural discussions
- Share visual references for design requirements
- Use images to clarify complex concepts

### Mention files you want Claude to work on
- Explicitly reference specific files and directories
- Provide file paths and line numbers when relevant
- Help Claude focus on the right parts of the codebase
- Reduce unnecessary exploration time

### Give Claude URLs for reference
- Provide links to documentation and specifications
- Reference external resources and examples
- Include API documentation and guides
- Share relevant Stack Overflow or blog posts

### Course correct early and often
- Provide feedback during the development process
- Redirect when Claude is going off-track
- Clarify requirements as they become clearer
- Prevent extensive rework by catching issues early

### Use /clear to keep context focused
- Clear conversation history when switching contexts
- Prevent confusion from previous discussions
- Start fresh for unrelated tasks
- Maintain clean conversation boundaries

### Use checklists and scratchpads for complex workflows
- Break down complex tasks into manageable steps
- Track progress through multi-step processes
- Document intermediate results and decisions
- Maintain organization during long workflows

### Pass data into Claude through various methods
- Use file uploads for large datasets
- Copy-paste relevant code snippets
- Reference external resources and documentation
- Provide multiple input formats as needed

## 5. Use headless mode to automate your infra

### Issue triage automation
- Automatically categorize and prioritize issues
- Route issues to appropriate team members
- Generate initial responses and suggestions
- Streamline issue management workflows

### Custom linting capabilities
- Implement project-specific code quality checks
- Automate style and convention enforcement
- Generate reports on code quality metrics
- Integrate with CI/CD pipelines

## 6. Uplevel with multi-Claude workflows

### Have one Claude write code; use another to verify
- Implement separation of concerns in development
- Use different Claude instances for writing and reviewing
- Improve code quality through independent verification
- Reduce bias from single-perspective development

### Have multiple checkouts of your repo
- Enable parallel development workflows
- Test different approaches simultaneously
- Reduce conflicts in collaborative development
- Facilitate feature branch experimentation

### Use git worktrees for parallel development
- Work on multiple features simultaneously
- Maintain separate working directories
- Switch between different code states efficiently
- Reduce context switching overhead

### Use headless mode with custom harness for large-scale operations
- Automate complex, multi-step processes
- Handle large-scale refactoring and migrations
- Implement custom orchestration logic
- Scale development operations efficiently

## Summary

These best practices provide a comprehensive framework for effectively using Claude Code in various development scenarios. By following these guidelines, developers can:

- Optimize their development workflow and productivity
- Leverage Claude's capabilities more effectively
- Implement robust and scalable development processes
- Collaborate more efficiently with AI assistance
- Maintain high code quality and consistency

The key to success is experimenting with different workflows and finding the approaches that work best for your specific use cases and development environment.