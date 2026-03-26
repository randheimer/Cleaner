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

class Logger {
public:
    static Logger& GetInstance();
    
    void Initialize(const std::wstring& logPath);
    void Log(LogLevel level, const std::wstring& message);
    void LogToConsole(LogLevel level, const std::wstring& message);
    void Close();
    std::wstring GetTimestamp();
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;
    ~Logger();
    
    std::wofstream logFile;
    std::mutex logMutex;
    bool initialized = false;
    
    std::wstring LogLevelToString(LogLevel level);
};

} // namespace WindowsCleaner
