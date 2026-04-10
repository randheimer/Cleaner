#include "../include/StatsDatabase.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace WindowsCleaner {

StatsDatabase& StatsDatabase::GetInstance() {
    static StatsDatabase instance;
    return instance;
}

void StatsDatabase::Initialize(const std::wstring& dbPath) {
    databasePath = dbPath;
    LoadDatabase();
}

void StatsDatabase::LoadDatabase() {
    sessions.clear();
    
    if (!fs::exists(databasePath)) {
        Logger::GetInstance().Log(LogLevel::INFO, L"No existing stats database found, creating new one");
        return;
    }
    
    std::ifstream file(databasePath);
    if (!file.is_open()) {
        Logger::GetInstance().Log(LogLevel::WARNING, L"Could not open stats database for reading");
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        CleaningSession session;
        char delimiter;
        
        // Format: timestamp|bytesFreed|filesDeleted|foldersDeleted|errors|warnings
        if (iss >> session.timestamp >> delimiter >> session.bytesFreed >> delimiter 
            >> session.filesDeleted >> delimiter >> session.foldersDeleted >> delimiter 
            >> session.errors >> delimiter >> session.warnings) {
            sessions.push_back(session);
        }
    }
    
    file.close();
    Logger::GetInstance().Log(LogLevel::INFO, 
        L"Loaded " + std::to_wstring(sessions.size()) + L" sessions from database");
}

void StatsDatabase::SaveDatabase() {
    std::ofstream file(databasePath, std::ios::trunc);
    if (!file.is_open()) {
        Logger::GetInstance().Log(LogLevel::ERR, L"Could not open stats database for writing");
        return;
    }
    
    for (const auto& session : sessions) {
        file << session.timestamp << "|" 
             << session.bytesFreed << "|" 
             << session.filesDeleted << "|" 
             << session.foldersDeleted << "|" 
             << session.errors << "|" 
             << session.warnings << "\n";
    }
    
    file.close();
    Logger::GetInstance().Log(LogLevel::INFO, L"Stats database saved successfully");
}

void StatsDatabase::SaveSession(const CleaningSession& session) {
    sessions.push_back(session);
    SaveDatabase();
    Logger::GetInstance().Log(LogLevel::INFO, L"New cleaning session saved to database");
}

AggregatedStats StatsDatabase::AggregateSessionsInTimeRange(time_t startTime, time_t endTime) {
    AggregatedStats stats;
    
    for (const auto& session : sessions) {
        if (session.timestamp >= startTime && session.timestamp <= endTime) {
            stats.totalBytesFreed += session.bytesFreed;
            stats.totalFilesDeleted += session.filesDeleted;
            stats.totalFoldersDeleted += session.foldersDeleted;
            stats.totalErrors += session.errors;
            stats.totalWarnings += session.warnings;
            stats.sessionCount++;
        }
    }
    
    return stats;
}

AggregatedStats StatsDatabase::AggregateAllSessions() {
    AggregatedStats stats;
    
    for (const auto& session : sessions) {
        stats.totalBytesFreed += session.bytesFreed;
        stats.totalFilesDeleted += session.filesDeleted;
        stats.totalFoldersDeleted += session.foldersDeleted;
        stats.totalErrors += session.errors;
        stats.totalWarnings += session.warnings;
        stats.sessionCount++;
    }
    
    return stats;
}

AggregatedStats StatsDatabase::GetStats24Hours() {
    time_t now = time(nullptr);
    time_t dayAgo = now - (24 * 60 * 60);
    return AggregateSessionsInTimeRange(dayAgo, now);
}

AggregatedStats StatsDatabase::GetStats7Days() {
    time_t now = time(nullptr);
    time_t weekAgo = now - (7 * 24 * 60 * 60);
    return AggregateSessionsInTimeRange(weekAgo, now);
}

AggregatedStats StatsDatabase::GetStats31Days() {
    time_t now = time(nullptr);
    time_t monthAgo = now - (31 * 24 * 60 * 60);
    return AggregateSessionsInTimeRange(monthAgo, now);
}

AggregatedStats StatsDatabase::GetStatsOverall() {
    return AggregateAllSessions();
}

std::vector<CleaningSession> StatsDatabase::GetRecentSessions(int count) {
    std::vector<CleaningSession> recent;
    
    // Sort by timestamp descending
    std::vector<CleaningSession> sorted = sessions;
    std::sort(sorted.begin(), sorted.end(), 
        [](const CleaningSession& a, const CleaningSession& b) {
            return a.timestamp > b.timestamp;
        });
    
    int limit = std::min(count, static_cast<int>(sorted.size()));
    for (int i = 0; i < limit; i++) {
        recent.push_back(sorted[i]);
    }
    
    return recent;
}

} // namespace WindowsCleaner
