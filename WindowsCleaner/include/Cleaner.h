#pragma once
#include <string>
#include <vector>
#include "CleanerConfig.h"

namespace WindowsCleaner {

struct CleanStats {
    uint64_t bytesFreed = 0;
    int filesDeleted = 0;
    int foldersDeleted = 0;
    int errors = 0;
    int warnings = 0;
};

class Cleaner {
public:
    Cleaner();
    
    void DetectApplications();
    void CleanAll();
    void CleanTarget(const CleanTarget& target);
    
    const CleanStats& GetStats() const { return stats; }
    const std::vector<std::wstring>& GetDetectedApps() const { return detectedApps; }
    
    void GenerateReport(const std::wstring& reportPath);
    void SaveStatsToDatabase();

private:
    CleanStats stats;
    std::vector<std::wstring> detectedApps;
    
    void CleanSystemTemp();
    void CleanWindowsTemp();
    void CleanPrefetch();
    void CleanRecycleBin();
    void FlushDNSCache();
    void CleanRecentFiles();
    void CleanWindowsUpdate();
    void CleanShaderCache();
    void CleanThumbnails();
    void CleanErrorReports();
    
    void CleanBrowsers();
    void CleanDevTools();
    void CleanGaming();
    void CleanGameLaunchers();
    void CleanCitizenFX();
    
    bool IsApplicationInstalled(const std::wstring& path);
    bool DetectAnticheatTracking(const std::wstring& kvsPath, std::wstring& detectedType);
};

} // namespace WindowsCleaner
