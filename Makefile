# Makefile for RE4 Save Redirect Plugin
# This plugin redirects Resident Evil 4 (2005) save files to a custom location

# For Windows with MinGW-w64
CC = g++
CFLAGS = -shared -O2 -Wall -Wextra -D_WIN32_WINNT=0x0501
LIBS = -ldetours -static -lshlwapi
TARGET = RE4SaveRedirect.dll
SOURCES = dllmain.cpp

# Default target
all: $(TARGET)

# Build the DLL
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

# Clean build artifacts
clean:
	del $(TARGET)

# Install dependencies
install-deps:
	@echo "Please install MinGW-w64 and Microsoft Detours library manually"
	@echo "For Detours: Download from https://github.com/microsoft/Detours and install"
	@echo "Make sure detours.lib is in your library path"

.PHONY: all clean install-deps