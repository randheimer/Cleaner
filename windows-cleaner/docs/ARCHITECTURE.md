# Windows Cleaner - Architecture Overview

## Project Purpose

Windows Cleaner is a system maintenance utility that removes temporary files, caches, and tracking data from Windows, browsers, development tools, and gaming applications. It includes specialized detection for FiveM/CitizenFX anticheat tracking and maintains a persistent database of cleaning statistics.

## Folder Structure

### `/src/scanning/`
**Purpose:** Core scanning and cleaning engine that detects and removes files

**Files:**
- `ScanEngine.cpp/h` - Main cleaning engine that orchestrates scan and clean operations
- `CleanTargets.cpp/h` - Configuration of all cleanable targets (system, browsers, dev tools, gaming)

**Why this name:** This folder contains the code that actively scans the filesystem and performs cleaning operations. The name directly describes what this subsystem does in the context of this application.

### `/src/filesystem/`
**Purpose:** Low-level file and directory operations

**Files:**
- `FileOperations.cpp/h` - Utilities for deleting files/folders, checking existence, calculating sizes

**Why this name:** This code handles all direct filesystem manipulation. It's the abstraction layer between the scanning engine and the Windows filesystem APIs.

### `/src/statistics/`
**Purpose:** Persistent tracking of cleaning history

**Files:**
- `CleaningHistory.cpp/h` - Database management for storing and aggregating cleaning session data

**Why this name:** This subsystem tracks statistics about what was cleaned, when, and how much space was freed. It provides historical analysis over 24 hours, 7 days, 31 days, and lifetime.

### `/src/logging/`
**Purpose:** Activity logging to file

**Files:**
- `ActivityLog.cpp/h` - File-based logging system for tracking operations and errors

**Why this name:** This code logs all cleaning activities, warnings, and errors to a persistent log file for debugging and audit purposes.

### `/src/console/`
**Purpose:** User interface and program entry point

**Files:**
- `main.cpp` - Console UI, menu system, admin privilege checking, and user interaction

**Why this name:** This is the console-based user interface layer that presents menus, displays statistics, and handles user input.

### `/include/`
**Purpose:** Public header files mirroring the src structure

All headers are organized in subfolders matching their corresponding source files for easy navigation.

### `/build/`
**Purpose:** Build system files

**Files:**
- `WindowsCleaner.vcxproj` - Visual Studio project file
- `WindowsCleaner.vcxproj.filters` - Visual Studio filters
- `WindowsCleaner.sln` - Visual Studio solution file

**Why this name:** Contains all files related to building the project. Keeps build configuration separate from source code.

### `/bin/`
**Purpose:** Compiled executables and DLLs

Output directory for Release and Debug builds.

### `/tests/`
**Purpose:** Unit and integration tests

Currently empty, ready for future test implementation.

### `/docs/`
**Purpose:** Architecture and design documentation

Contains this file and any future documentation about the system design.

## Key Design Decisions

### Class Renaming
- `Cleaner` → `ScanEngine` - More accurately describes its role as the scanning/cleaning engine
- `CleanerConfig` → `CleanTargets` - Clarifies that it defines what to clean, not general configuration
- `FileUtils` → `FileOperations` - More specific about performing operations vs. general utilities
- `StatsDatabase` → `CleaningHistory` - Better describes the domain concept of historical cleaning data
- `Logger` → `ActivityLog` - Distinguishes from generic logging, emphasizes activity tracking

### Folder Organization
Each folder represents a distinct system responsibility:
- **Scanning** - What to clean and how to clean it
- **Filesystem** - How to interact with files and directories
- **Statistics** - How to track and analyze cleaning history
- **Logging** - How to record activity for debugging
- **Console** - How users interact with the system

This organization makes it immediately clear where to find code related to any specific concern.

## What Was Changed

### Structure
- Moved all source files from flat `src/` to purpose-driven subfolders
- Moved all headers from flat `include/` to matching subfolder structure
- Moved build files from project root to `build/` folder
- Created `docs/` folder for architecture documentation

### Naming
- Renamed all classes to better reflect their actual purpose
- Updated all include paths to reflect new folder structure
- Updated Visual Studio project file with new paths and output directories

### What Was NOT Changed
- No logic or functionality was modified
- No algorithms were altered
- All behavior remains identical to the original code
- Only structure, organization, and naming were changed

## Building the Project

Open `build/WindowsCleaner.sln` in Visual Studio and build as normal. Output will be in `bin/Release/` or `bin/Debug/`.
