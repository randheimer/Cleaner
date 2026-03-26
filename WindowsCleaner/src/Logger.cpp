#include "../include/Logger.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <windows.h>

namespace WindowsCleaner {

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

void Logger::Initialize(const std::wstring& logPath) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (initialized) {
        logFile.close();
    }
    
    logFile.open(logPath, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        initialized = true;
        logFile.imbue(std::locale(""));
        // Write initialization message directly to avoid deadlock
        logFile << L"[" << GetTimestamp() << L"] "
                << L"[INFO] "
                << L"Logger initialized" << std::endl;
        logFile.flush();
    }
}

void Logger::Log(LogLevel level, const std::wstring& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (!initialized || !logFile.is_open()) return;
    
    logFile << L"[" << GetTimestamp() << L"] "
            << L"[" << LogLevelToString(level) << L"] "
            << message << std::endl;
    logFile.flush();
}

void Logger::LogToConsole(LogLevel level, const std::wstring& message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    
    switch (level) {
        case LogLevel::INFO:
            color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case LogLevel::WARNING:
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case LogLevel::ERR:
            color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case LogLevel::SUCCESS:
            color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
    }
    
    SetConsoleTextAttribute(hConsole, color);
    std::wcout << L"[" << LogLevelToString(level) << L"] " << message << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    // Also write to log file
    std::lock_guard<std::mutex> lock(logMutex);
    if (initialized && logFile.is_open()) {
        logFile << L"[" << GetTimestamp() << L"] "
                << L"[" << LogLevelToString(level) << L"] "
                << message << std::endl;
        logFile.flush();
    }
}

void Logger::Close() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
    initialized = false;
}

Logger::~Logger() {
    Close();
}

std::wstring Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::tm tm;
    localtime_s(&tm, &time);
    
    std::wstringstream ss;
    ss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::wstring Logger::LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return L"INFO";
        case LogLevel::WARNING: return L"WARNING";
        case LogLevel::ERR:     return L"ERROR";
        case LogLevel::SUCCESS: return L"SUCCESS";
        default:                return L"UNKNOWN";
    }
}

} // namespace WindowsCleaner
