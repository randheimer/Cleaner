#pragma once
#include <string>
#include <fstream>
#include <mutex>

namespace WindowsCleaner {

enum class LogLevel {
    INFO,
    WARNING,
    ERR,
    SUCCESS
};

class ActivityLog {
public:
    static ActivityLog& GetInstance();
    
    void Initialize(const std::wstring& logPath);
    void Log(LogLevel level, const std::wstring& message);
    void LogToConsole(LogLevel level, const std::wstring& message);
    void Close();
    std::wstring GetTimestamp();
    
    ActivityLog(const ActivityLog&) = delete;
    ActivityLog& operator=(const ActivityLog&) = delete;

private:
    ActivityLog() = default;
    ~ActivityLog();
    
    std::wofstream logFile;
    std::mutex logMutex;
    bool initialized = false;
    
    std::wstring LogLevelToString(LogLevel level);
};

} // namespace WindowsCleaner
