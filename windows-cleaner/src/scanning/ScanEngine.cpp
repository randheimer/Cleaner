#include "../../include/scanning/CleanTargets.h"
#include "../../include/scanning/ScanEngine.h"
#include "../../include/logging/ActivityLog.h"
#include "../../include/filesystem/FileOperations.h"
#include "../../include/statistics/CleaningHistory.h"
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

namespace WindowsCleaner {

ScanEngine::ScanEngine() {
    stats = CleanStats();
}

void ScanEngine::DetectApplications() {
    ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Scanning for installed applications...");
    
    auto targets = CleanTargets::GetAllTargets();
    
    for (const auto& target : targets) {
        std::wstring expandedPath = CleanTargets::ExpandPath(target.path);
        if (FileOperations::DirectoryExists(expandedPath)) {
            detectedApps.push_back(target.name);
            ActivityLog::GetInstance().Log(LogLevel::INFO, L"Detected: " + target.name);
        }
    }
    
    ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, 
        L"Detection complete: " + std::to_wstring(detectedApps.size()) + L" items found");
}

void ScanEngine::CleanAll() {
    ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Starting full system clean...");
    
    auto targets = CleanTargets::GetAllTargets();
    int current = 0;
    int total = static_cast<int>(targets.size());
    
    for (const auto& target : targets) {
        current++;
        std::wcout << L"[" << current << L"/" << total << L"] Cleaning " << target.name << L"..." << std::endl;
        CleanTarget(target);
    }
    
    // Additional system operations
    FlushDNSCache();
    CleanRecycleBin();
    CleanCitizenFX();
    
    ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, L"Full system clean completed!");
}

void ScanEngine::CleanTarget(const WindowsCleaner::CleanTarget& target) {
    std::wstring expandedPath = CleanTargets::ExpandPath(target.path);
    
    if (!FileOperations::DirectoryExists(expandedPath)) {
        ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Skipping " + target.name + L" (not found)");
        return;
    }
    
    ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Cleaning: " + target.name);
    
    // Calculate size before deletion
    uint64_t sizeBefore = FileOperations::GetDirectorySize(expandedPath);
    int filesBefore = FileOperations::CountFiles(expandedPath, target.isRecursive);
    int foldersBefore = FileOperations::CountDirectories(expandedPath, target.isRecursive);
    
    try {
        if (target.deleteFolder) {
            // Delete entire folder
            if (FileOperations::DeleteDirectory(expandedPath, true)) {
                stats.bytesFreed += sizeBefore;
                stats.filesDeleted += filesBefore;
                stats.foldersDeleted += foldersBefore;
                ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, L"Cleaned: " + target.name);
            } else {
                stats.warnings++;
                ActivityLog::GetInstance().LogToConsole(LogLevel::WARNING, L"Could not fully clean: " + target.name);
            }
        } else {
            // Delete contents only
            try {
                for (const auto& entry : fs::directory_iterator(expandedPath)) {
                    try {
                        if (fs::is_directory(entry.status())) {
                            fs::remove_all(entry.path());
                            stats.foldersDeleted++;
                        } else {
                            fs::remove(entry.path());
                            stats.filesDeleted++;
                        }
                    } catch (...) {
                        stats.warnings++;
                    }
                }
                stats.bytesFreed += sizeBefore;
                ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, L"Cleaned contents: " + target.name);
            } catch (...) {
                stats.warnings++;
                ActivityLog::GetInstance().LogToConsole(LogLevel::WARNING, L"Could not clean contents: " + target.name);
            }
        }
    } catch (...) {
        stats.errors++;
        ActivityLog::GetInstance().LogToConsole(LogLevel::ERR, L"Error cleaning: " + target.name);
    }
}

void ScanEngine::CleanRecycleBin() {
    ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Emptying Recycle Bin...");
    
    HRESULT hr = SHEmptyRecycleBinW(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
    
    if (SUCCEEDED(hr)) {
        ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, L"Recycle Bin emptied successfully");
    } else {
        stats.warnings++;
        ActivityLog::GetInstance().LogToConsole(LogLevel::WARNING, L"Could not empty Recycle Bin");
    }
}

void ScanEngine::FlushDNSCache() {
    ActivityLog::GetInstance().LogToConsole(LogLevel::INFO, L"Flushing DNS cache...");
    
    HINSTANCE hDnsApi = LoadLibraryW(L"dnsapi.dll");
    if (hDnsApi) {
        typedef BOOL (WINAPI *DnsFlushResolverCacheFunc)();
        DnsFlushResolverCacheFunc DnsFlushResolverCache = 
            (DnsFlushResolverCacheFunc)GetProcAddress(hDnsApi, "DnsFlushResolverCache");
        
        if (DnsFlushResolverCache && DnsFlushResolverCache()) {
            ActivityLog::GetInstance().LogToConsole(LogLevel::SUCCESS, L"DNS cache flushed successfully");
        } else {
            stats.warnings++;
            ActivityLog::GetInstance().LogToConsole(LogLevel::WARNING, L"Could not flush DNS cache");
        }
        
        FreeLibrary(hDnsApi);
    } else {
        stats.errors++;
        ActivityLog::GetInstance().LogToConsole(LogLevel::ERR, L"Could not load dnsapi.dll");
    }
}

void ScanEngine::GenerateReport(const std::wstring& reportPath) {
    std::wofstream report(reportPath);
    if (!report.is_open()) {
        ActivityLog::GetInstance().Log(LogLevel::ERR, L"Could not create report file");
        return;
    }
    
    report.imbue(std::locale(""));
    
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t timestamp[64];
    swprintf_s(timestamp, L"%04d-%02d-%02d %02d:%02d:%02d", 
               st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    
    report << L"{\n";
    report << L"  \"version\": \"1.0.0\",\n";
    report << L"  \"timestamp\": \"" << timestamp << L"\",\n";
    report << L"  \"summary\": {\n";
    report << L"    \"bytes_freed\": " << stats.bytesFreed << L",\n";
    report << L"    \"mb_freed\": " << (stats.bytesFreed / 1048576) << L",\n";
    report << L"    \"files_deleted\": " << stats.filesDeleted << L",\n";
    report << L"    \"folders_deleted\": " << stats.foldersDeleted << L",\n";
    report << L"    \"errors\": " << stats.errors << L",\n";
    report << L"    \"warnings\": " << stats.warnings << L"\n";
    report << L"  },\n";
    report << L"  \"detected_apps\": [\n";
    
    for (size_t i = 0; i < detectedApps.size(); i++) {
        report << L"    \"" << detectedApps[i] << L"\"";
        if (i < detectedApps.size() - 1) report << L",";
        report << L"\n";
    }
    
    report << L"  ],\n";
    report << L"  \"status\": \"completed\"\n";
    report << L"}\n";
    
    report.close();
    ActivityLog::GetInstance().Log(LogLevel::SUCCESS, L"Report generated: " + reportPath);
}

bool ScanEngine::IsApplicationInstalled(const std::wstring& path) {
    std::wstring expandedPath = CleanTargets::ExpandPath(path);
    return FileOperations::DirectoryExists(expandedPath);
}

void ScanEngine::CleanCitizenFX() {
    std::wstring citizenFXPath = CleanTargets::ExpandPath(L"%APPDATA%\\CitizenFX");
    
    if (!FileOperations::DirectoryExists(citizenFXPath)) {
        return;
    }
    
    ActivityLog::GetInstance().Log(LogLevel::INFO, L"Cleaning CitizenFX cache and temporary data");
    
    std::wstring kvsPath = citizenFXPath + L"\\kvs";
    std::wstring detectedType;
    
    // Check for anticheat tracking
    if (DetectAnticheatTracking(kvsPath, detectedType)) {
        std::wcout << L"\n";
        std::wcout << L"╔════════════════════════════════════════════════════════════╗\n";
        std::wcout << L"║  [!] ANTICHEAT TRACKING DETECTED                           ║\n";
        std::wcout << L"╚════════════════════════════════════════════════════════════╝\n";
        std::wcout << L"\n";
        std::wcout << L"  " << detectedType << L" anticheat tracking files found!\n";
        std::wcout << L"  Don't worry - removing them now...\n";
        std::wcout << L"\n";
        
        ActivityLog::GetInstance().Log(LogLevel::WARNING, detectedType + L" anticheat tracking detected and will be removed");
        Sleep(3000);
    }
    
    // Delete kvs and kvs_cl2 folders
    std::wstring kvsPath2 = citizenFXPath + L"\\kvs_cl2";
    
    if (FileOperations::DirectoryExists(kvsPath)) {
        uint64_t size = FileOperations::GetDirectorySize(kvsPath);
        if (FileOperations::DeleteDirectory(kvsPath, true)) {
            stats.bytesFreed += size;
            stats.foldersDeleted++;
            ActivityLog::GetInstance().Log(LogLevel::SUCCESS, L"Deleted CitizenFX kvs folder");
        }
    }
    
    if (FileOperations::DirectoryExists(kvsPath2)) {
        uint64_t size = FileOperations::GetDirectorySize(kvsPath2);
        if (FileOperations::DeleteDirectory(kvsPath2, true)) {
            stats.bytesFreed += size;
            stats.foldersDeleted++;
            ActivityLog::GetInstance().Log(LogLevel::SUCCESS, L"Deleted CitizenFX kvs_cl2 folder");
        }
    }
    
    // Clean cache and logs in subdirectories
    try {
        for (const auto& entry : fs::directory_iterator(citizenFXPath)) {
            if (fs::is_directory(entry.status())) {
                std::wstring cachePath = entry.path().wstring() + L"\\cache";
                std::wstring logsPath = entry.path().wstring() + L"\\logs";
                
                if (FileOperations::DirectoryExists(cachePath)) {
                    FileOperations::DeleteDirectory(cachePath, true);
                }
                if (FileOperations::DirectoryExists(logsPath)) {
                    FileOperations::DeleteDirectory(logsPath, true);
                }
            }
        }
    } catch (...) {
        stats.warnings++;
    }
    
    ActivityLog::GetInstance().Log(LogLevel::SUCCESS, L"CitizenFX cleaned");
}

bool ScanEngine::DetectAnticheatTracking(const std::wstring& kvsPath, std::wstring& detectedType) {
    if (!FileOperations::DirectoryExists(kvsPath)) {
        return false;
    }
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(kvsPath)) {
            if (fs::is_regular_file(entry.status())) {
                std::wstring filename = entry.path().filename().wstring();
                
                // Convert to lowercase for case-insensitive comparison
                std::wstring filenameLower = filename;
                std::transform(filenameLower.begin(), filenameLower.end(), filenameLower.begin(), ::towlower);
                
                if (filenameLower.find(L"waveshield") != std::wstring::npos) {
                    detectedType = L"WaveShield";
                    ActivityLog::GetInstance().Log(LogLevel::WARNING, L"WaveShield anticheat tracking detected: " + filename);
                    return true;
                }
                
                if (filenameLower.find(L"electron") != std::wstring::npos) {
                    detectedType = L"Electron AC";
                    ActivityLog::GetInstance().Log(LogLevel::WARNING, L"Electron AC anticheat tracking detected: " + filename);
                    return true;
                }
            }
        }
    } catch (...) {
        return false;
    }
    
    return false;
}

void ScanEngine::SaveStatsToDatabase() {
    CleaningSession session;
    session.timestamp = time(nullptr);
    session.bytesFreed = stats.bytesFreed;
    session.filesDeleted = stats.filesDeleted;
    session.foldersDeleted = stats.foldersDeleted;
    session.errors = stats.errors;
    session.warnings = stats.warnings;
    
    CleaningHistory::GetInstance().SaveSession(session);
    ActivityLog::GetInstance().Log(LogLevel::INFO, L"Cleaning session saved to database");
}

} // namespace WindowsCleaner
