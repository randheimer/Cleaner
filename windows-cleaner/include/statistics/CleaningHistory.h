#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>

namespace WindowsCleaner {

struct CleaningSession {
    time_t timestamp;
    uint64_t bytesFreed;
    int filesDeleted;
    int foldersDeleted;
    int errors;
    int warnings;
};

struct AggregatedStats {
    uint64_t totalBytesFreed = 0;
    int totalFilesDeleted = 0;
    int totalFoldersDeleted = 0;
    int totalErrors = 0;
    int totalWarnings = 0;
    int sessionCount = 0;
};

class CleaningHistory {
public:
    static CleaningHistory& GetInstance();
    
    void Initialize(const std::wstring& dbPath);
    void SaveSession(const CleaningSession& session);
    
    AggregatedStats GetStats24Hours();
    AggregatedStats GetStats7Days();
    AggregatedStats GetStats31Days();
    AggregatedStats GetStatsOverall();
    
    std::vector<CleaningSession> GetRecentSessions(int count = 10);
    
private:
    CleaningHistory() = default;
    std::wstring databasePath;
    
    void LoadDatabase();
    void SaveDatabase();
    std::vector<CleaningSession> sessions;
    
    AggregatedStats AggregateSessionsInTimeRange(time_t startTime, time_t endTime);
    AggregatedStats AggregateAllSessions();
};

} // namespace WindowsCleaner
