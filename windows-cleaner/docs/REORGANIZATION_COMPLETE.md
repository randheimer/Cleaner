# C++ Architecture Reorganization - Complete

## Project: Windows Cleaner

**Date:** April 11, 2026  
**Type:** Windows system maintenance utility  
**Language:** C++17/C++20

---

## What This Project Does

Windows Cleaner is a system maintenance tool that scans and removes temporary files, caches, and tracking data from:
- Windows system (temp files, prefetch, shader cache, error reports)
- Browsers (Chrome, Edge, Firefox, Brave, Opera)
- Development tools (VS Code, Visual Studio, npm, pip, Maven, Gradle, etc.)
- Gaming platforms (Steam, Epic, Discord, FiveM, Battle.net, etc.)
- GPU caches (NVIDIA, AMD)
- FiveM/CitizenFX anticheat tracking detection and removal

It maintains a persistent database of cleaning statistics and logs all operations.

---

## Folder Structure Rationale

### `/src/scanning/` - Scan and Clean Operations
**What it does:** Core engine that detects installed applications, scans for cleanable items, and performs deletion operations.

**Why this name:** This code actively scans the filesystem and performs cleaning. The name directly describes the primary responsibility in this specific application context.

**Files:**
- `ScanEngine.cpp/h` - Main cleaning orchestrator
- `CleanTargets.cpp/h` - Defines all cleanable targets (system, browsers, dev tools, gaming)

### `/src/filesystem/` - File and Directory Operations
**What it does:** Low-level filesystem manipulation - deleting files/folders, checking existence, calculating sizes, counting items.

**Why this name:** This is the abstraction layer between the scanning engine and Windows filesystem APIs. It handles all direct filesystem interaction.

**Files:**
- `FileOperations.cpp/h` - Filesystem utilities

### `/src/statistics/` - Cleaning History Tracking
**What it does:** Persistent database that stores cleaning session data and provides aggregated statistics over time periods (24h, 7d, 31d, lifetime).

**Why this name:** This subsystem tracks statistics about cleaning operations - bytes freed, files deleted, errors encountered. It provides historical analysis.

**Files:**
- `CleaningHistory.cpp/h` - Database management and aggregation

### `/src/logging/` - Activity Logging
**What it does:** File-based logging system that records all operations, warnings, and errors to disk for debugging and auditing.

**Why this name:** This code logs all cleaning activities to persistent log files. The name emphasizes activity tracking rather than generic logging.

**Files:**
- `ActivityLog.cpp/h` - Logging system

### `/src/console/` - User Interface
**What it does:** Console-based UI with menu system, admin privilege checking, statistics display, and user interaction.

**Why this name:** This is the console interface layer - the entry point where users interact with the application.

**Files:**
- `main.cpp` - Entry point and UI

### `/include/` - Public Headers
Mirrors the exact structure of `/src/` for easy navigation. Each subsystem's headers are in matching subfolders.

### `/build/` - Build System
Contains all Visual Studio project files (.sln, .vcxproj, .vcxproj.filters). Keeps build configuration separate from source code.

### `/bin/` - Compiled Output
Output directory for Release and Debug executables.

### `/tests/` - Unit Tests
Empty, ready for future test implementation.

### `/docs/` - Documentation
Architecture documentation, migration summary, and design decisions.

---

## Class Renames

| Old Name | New Name | Reasoning |
|----------|----------|-----------|
| `Cleaner` | `ScanEngine` | More accurately describes its role as the scanning/cleaning engine |
| `CleanerConfig` | `CleanTargets` | Clarifies that it defines what to clean, not general configuration |
| `FileUtils` | `FileOperations` | More specific about performing operations vs. generic utilities |
| `StatsDatabase` | `CleaningHistory` | Better domain concept - emphasizes historical cleaning data |
| `Logger` | `ActivityLog` | Distinguishes from generic logging, emphasizes activity tracking |

---

## Changes Made

### Structure
✅ Moved all source files from flat `src/` to purpose-driven subfolders  
✅ Moved all headers from flat `include/` to matching subfolder structure  
✅ Moved build files from project root to `build/` folder  
✅ Created `docs/` folder with architecture documentation  
✅ Created `README.md` with project overview  

### Code
✅ Renamed all classes to better reflect their purpose  
✅ Updated all include paths to reflect new folder structure  
✅ Updated all class references throughout the codebase  
✅ Updated Visual Studio project file with new paths  
✅ Fixed output directories to use `bin/` folder  

### What Was NOT Changed
❌ No logic or functionality modifications  
❌ No algorithm changes  
❌ No behavior changes  
❌ Only structure, organization, and naming were changed  

---

## Key Principles Applied

1. **Purpose-Driven Naming:** Every folder name describes what the code does in this specific project context
2. **No Generic Categories:** Avoided names like "common", "utils", "helpers", "misc"
3. **Immediate Clarity:** Folder names are clear to someone who has never seen the codebase
4. **Responsibility-Based:** Each folder represents a distinct system responsibility
5. **Mirrored Structure:** Include folder structure mirrors src structure exactly

---

## Build Instructions

1. Open `build/WindowsCleaner.sln` in Visual Studio
2. Select Release or Debug configuration
3. Build (Ctrl+Shift+B)
4. Output in `bin/Release/` or `bin/Debug/`

---

## Result

The project now has a clean, intuitive structure where:
- Any developer can immediately understand what each folder contains
- Related code is grouped by system responsibility
- The architecture is self-documenting through folder names
- Navigation is intuitive and logical
- The codebase is ready for future expansion

**Status:** ✅ Complete - All files moved, renamed, and updated successfully
