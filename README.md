# Windows Cleaner

A high-performance C++ system maintenance utility for Windows that removes temporary files, caches, and tracking data from the operating system, browsers, development tools, and gaming applications.

## Features

- **System Cleaning**: Removes Windows temp files, prefetch, cache, error reports, and more
- **Browser Cache Cleaning**: Supports Chrome, Edge, Firefox, Brave, Opera (preserves cookies/passwords/history)
- **Development Tools**: Cleans VS Code, Visual Studio, npm, pip, Gradle, Maven, Cargo, and more
- **Gaming Platforms**: Cleans Steam, Epic Games, Discord, FiveM, Battle.net, Riot Games, and more
- **GPU Cache**: Removes NVIDIA and AMD shader caches
- **Anticheat Detection**: Detects and removes FiveM/CitizenFX anticheat tracking files
- **Statistics Tracking**: Persistent database tracking cleaning history over 24h, 7d, 31d, and lifetime
- **Activity Logging**: Detailed logs of all operations for debugging and auditing

## Requirements

- Windows 10/11
- Administrator privileges (required for some operations)
- Visual Studio 2019 or later (for building)
- C++17 or later

## Building

1. Open `WindowsCleaner.sln` in Visual Studio
2. Select Release or Debug configuration
3. Build the solution
4. Executable will be in `bin/Release/` or `bin/Debug/`

## Usage

1. Right-click the executable and select "Run as administrator"
2. Choose from the menu:
   - **[1] Scan** - Detect cleanable items without removing them
   - **[2] Clean** - Perform full system clean
   - **[3] View Stats** - Show current session statistics
   - **[4] View History** - Show historical cleaning statistics
   - **[5] Exit** - Close the application

## Project Structure

```
windows-cleaner/
├── src/
│   ├── scanning/          # Core scanning and cleaning engine
│   ├── filesystem/        # Low-level file/directory operations
│   ├── statistics/        # Cleaning history database
│   ├── logging/           # Activity logging system
│   └── console/           # User interface and entry point
├── include/               # Public headers (mirrors src structure)
├── build/                 # Visual Studio project files
├── bin/                   # Compiled executables
├── tests/                 # Unit tests (future)
└── docs/                  # Architecture documentation
```

See `docs/ARCHITECTURE.md` for detailed system design information.

## Safety

- **Preserves user data**: Browser cleaning only removes cache, not cookies, passwords, or history
- **Non-destructive**: Only removes temporary and cache files
- **Logging**: All operations are logged to `%USERPROFILE%\CleanerLogs\`
- **Statistics**: Cleaning history saved to `%USERPROFILE%\CleanerLogs\stats.db`

## What Gets Cleaned

### System
- Temporary files (user and system)
- Windows Update cache
- Prefetch files
- Shader caches
- Thumbnail caches
- Error reports and crash dumps
- DNS cache (flushed)
- Recycle Bin

### Browsers
- Cache files (Chrome, Edge, Firefox, Brave, Opera)
- GPU cache
- Service worker cache

### Development
- VS Code cache and logs
- Visual Studio cache
- Package manager caches (npm, pip, Maven, Gradle, Cargo, etc.)

### Gaming
- Game launcher caches (Steam, Epic, Origin, Battle.net, etc.)
- Discord cache
- FiveM/CitizenFX cache and anticheat tracking
- GPU shader caches (NVIDIA, AMD)

## License

This project is provided as-is for educational and personal use.

## Contributing

This is a personal project, but suggestions and improvements are welcome.

## Disclaimer

Use at your own risk. Always ensure you have backups of important data. While this tool is designed to only remove temporary and cache files, the author is not responsible for any data loss.
