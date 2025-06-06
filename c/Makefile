# DigitalOcean C Client Makefile

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -fPIC
LDFLAGS = -shared
LIBS = -lcurl -lcjson

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
LIBDIR = lib

# Source files
LIB_SOURCES = $(SRCDIR)/client.c $(SRCDIR)/config.c $(SRCDIR)/http.c $(SRCDIR)/json.c
CLI_SOURCES = $(SRCDIR)/cli/main.c $(SRCDIR)/cli/account.c $(SRCDIR)/cli/droplets.c $(SRCDIR)/cli/config.c

# Object files
LIB_OBJECTS = $(LIB_SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
CLI_OBJECTS = $(CLI_SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Targets
LIBRARY = $(LIBDIR)/libdigitalocean.so
STATIC_LIB = $(LIBDIR)/libdigitalocean.a
CLI_BINARY = $(BINDIR)/do-cli

# Default target
all: $(LIBRARY) $(CLI_BINARY)

# Create directories
$(BUILDDIR) $(BINDIR) $(LIBDIR):
	mkdir -p $@

# Shared library
$(LIBRARY): $(LIB_OBJECTS) | $(LIBDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Static library
$(STATIC_LIB): $(LIB_OBJECTS) | $(LIBDIR)
	ar rcs $@ $^

# CLI binary
$(CLI_BINARY): $(CLI_OBJECTS) $(LIBRARY) | $(BINDIR)
	$(CC) -o $@ $(CLI_OBJECTS) -L$(LIBDIR) -ldigitalocean $(LIBS)

# Compile library objects
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Debug build
debug: CFLAGS += -g -O0 -DDEBUG
debug: all

# Release build
release: CFLAGS += -O3 -DNDEBUG
release: all

# Examples
examples: $(LIBRARY)
	$(CC) $(CFLAGS) -I$(INCDIR) examples/basic_usage.c -L$(LIBDIR) -ldigitalocean $(LIBS) -o $(BINDIR)/example_basic

# Install
install: all
	install -d /usr/local/lib /usr/local/include/digitalocean /usr/local/bin
	install -m 644 $(LIBRARY) /usr/local/lib/
	install -m 644 $(STATIC_LIB) /usr/local/lib/
	install -m 644 $(INCDIR)/digitalocean/*.h /usr/local/include/digitalocean/
	install -m 755 $(CLI_BINARY) /usr/local/bin/
	ldconfig

# Uninstall
uninstall:
	rm -f /usr/local/lib/libdigitalocean.*
	rm -rf /usr/local/include/digitalocean
	rm -f /usr/local/bin/do-cli

# Clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(LIBDIR)

# Tests
test: $(LIBRARY)
	$(CC) $(CFLAGS) -I$(INCDIR) tests/test_client.c -L$(LIBDIR) -ldigitalocean $(LIBS) -o $(BINDIR)/test_client
	./$(BINDIR)/test_client

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build library and CLI (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release"
	@echo "  examples - Build example programs"
	@echo "  install  - Install to system"
	@echo "  uninstall- Remove from system"
	@echo "  clean    - Clean build artifacts"
	@echo "  test     - Build and run tests"
	@echo "  help     - Show this help"

.PHONY: all debug release examples install uninstall clean test help