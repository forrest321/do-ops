# Claude Instance Instructions for Concurrent Development

This document provides comprehensive instructions for Claude Code instances working concurrently on software development projects. These instructions should be referenced when multiple Claude instances collaborate on the same codebase.

## Core Multi-Instance Workflows

### Git Worktree Setup for Concurrent Development

Git worktrees enable multiple Claude instances to work on different features simultaneously without conflicts:

```bash
# Primary instance sets up main worktree
git worktree add ../feature-branch-name origin/feature-branch-name

# Each instance works in separate worktree directories
git worktree add ../instance-2-feature feature-branch-2
git worktree add ../instance-3-bugfix bugfix-branch
```

**Worktree Best Practices:**
- Each Claude instance should work in its own worktree directory
- Use descriptive branch names that indicate the instance and task
- Coordinate branch creation to avoid naming conflicts
- Regularly sync with remote to share progress between instances

### Instance Specialization Patterns

#### Code Writer + Code Reviewer Pattern
- **Writer Instance**: Focuses on implementation, feature development
- **Reviewer Instance**: Performs code review, quality checks, testing

```bash
# Writer instance workflow
git checkout -b feature/new-component
# Implement feature
git add . && git commit -m "Implement new component"
git push origin feature/new-component

# Reviewer instance workflow  
git fetch origin
git checkout feature/new-component
# Review code, run tests, suggest improvements
git checkout -b review/new-component-feedback
# Make review comments or fixes
```

#### Domain Separation Pattern
- **Frontend Instance**: Works on UI components, styling, client-side logic
- **Backend Instance**: Handles API endpoints, database, server-side logic
- **Testing Instance**: Writes tests, handles CI/CD, quality assurance

### Communication and Coordination

#### Branch Naming Conventions
Use consistent naming to identify which instance is working on what:
```
instance-1/feature-name
instance-2/bugfix-description
frontend/component-update
backend/api-enhancement
testing/integration-tests
```

#### Commit Message Coordination
Include instance identifier in commit messages:
```
[Instance-1] Add user authentication component
[Backend] Implement JWT token validation
[Frontend] Update login form styling
[Testing] Add authentication integration tests
```

#### Handoff Protocols
When passing work between instances:
1. Commit all changes with clear messages
2. Push to remote branch
3. Tag the commit with handoff information
4. Document current state and next steps in commit message

## Development Workflow Instructions

### Exploration and Planning Phase
Before starting implementation:
1. Use `Grep` and `Glob` tools to understand codebase structure
2. Read existing CLAUDE.md for project-specific context
3. Check for similar patterns in the codebase
4. Plan implementation to avoid conflicts with concurrent work

### Safe Concurrent Development
1. **Always fetch before starting work**: `git fetch origin`
2. **Check for active branches**: `git branch -r` to see what others are working on
3. **Coordinate file-level work**: Avoid multiple instances editing the same files
4. **Use atomic commits**: Make small, focused commits that can be easily merged
5. **Test before pushing**: Run tests and linting before sharing work

### Conflict Resolution Strategies
When merge conflicts occur:
1. **Communication first**: Check if another instance is working on conflicting code
2. **Understand changes**: Use `git log` and `git diff` to understand conflicting changes
3. **Collaborate on resolution**: Use shared branch for complex conflict resolution
4. **Test merged result**: Ensure functionality works after conflict resolution

### Quality Assurance Workflows

#### Cross-Instance Code Review
```bash
# Instance A completes feature
git push origin feature/new-functionality

# Instance B reviews and tests
git fetch origin
git checkout feature/new-functionality
# Run tests, review code, suggest changes

# If changes needed, create review branch
git checkout -b review/new-functionality-fixes
# Make necessary changes
git push origin review/new-functionality-fixes
```

#### Continuous Integration Coordination
- Only one instance should manage CI/CD updates at a time
- Coordinate test suite changes to avoid conflicts
- Use separate branches for CI configuration changes

## Tool Usage for Concurrent Work

### Essential Tools for Multi-Instance Work
1. **Git operations**: Master git worktrees, branching, merging
2. **GitHub CLI (gh)**: Coordinate PRs and issues across instances
3. **File search tools**: Use `Grep` and `Glob` to understand current state
4. **Todo management**: Track progress and coordinate tasks

### Communication Through Code
- Use detailed commit messages with context
- Add code comments when coordination is needed
- Document decisions in commit descriptions
- Use descriptive branch names for clarity

### Monitoring and Synchronization
```bash
# Check what other instances are working on
git branch -r
git log --oneline --graph --all

# Sync with remote frequently
git fetch origin
git status

# Check for conflicts before pushing
git pull --dry-run
```

## Headless Mode for Large-Scale Operations

### Automated Coordination
When using headless mode for concurrent operations:
1. **Lock coordination**: Implement simple file-based locks for critical operations
2. **Progress tracking**: Use shared progress indicators
3. **Error handling**: Implement rollback strategies for failed operations
4. **Logging**: Maintain detailed logs for debugging concurrent issues

### Custom Harness Development
Build coordination logic for:
- Task distribution across instances
- Progress monitoring and reporting
- Error recovery and retry logic
- Result aggregation from multiple instances

## Instance-Specific Best Practices

### For Primary Coordination Instance
- Manages main branch and releases
- Coordinates work distribution
- Handles complex merge conflicts
- Maintains project documentation

### For Feature Development Instances
- Focus on specific feature domains
- Coordinate with other instances on dependencies
- Write comprehensive tests for new features
- Document changes for other instances

### For Quality Assurance Instances
- Review code from other instances
- Maintain and enhance test suites
- Monitor CI/CD pipeline health
- Enforce coding standards and conventions

## Emergency Procedures

### When Conflicts Arise
1. **Stop and communicate**: Halt work and assess the situation
2. **Backup current work**: Stash or commit work in progress
3. **Coordinate resolution**: Work together on conflict resolution
4. **Verify resolution**: Test that resolved code works correctly

### When Instance Goes Offline
1. **Document current state**: Clear commit messages and branch state
2. **Handoff protocols**: Tag work for easy pickup by another instance
3. **Progress tracking**: Update shared todo lists or tracking systems

## Optimization Guidelines

### Minimize Context Switching
- Use git worktrees to maintain separate environments
- Keep instance-specific documentation current
- Use `/clear` command when switching between major tasks
- Maintain focused conversation boundaries

### Efficient Collaboration
- Batch related changes in single commits
- Use descriptive branch names and commit messages
- Coordinate timing of pushes and pulls
- Share context through well-documented code

### Performance Considerations
- Avoid simultaneous large operations (like major refactoring)
- Coordinate resource-intensive tasks (builds, tests)
- Use parallel development for independent features
- Balance load across available instances

This framework enables effective concurrent development while maintaining code quality and minimizing conflicts between Claude instances.