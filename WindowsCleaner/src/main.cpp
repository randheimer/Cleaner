#include <iostream>
#include <windows.h>
#include <string>
#include <filesystem>
#include <iomanip>
#include "../include/Logger.h"
#include "../include/Cleaner.h"
#include "../include/FileUtils.h"
#include "../include/StatsDatabase.h"

namespace fs = std::filesystem;

void SetConsoleTitle() {
    SetConsoleTitleW(L"Windows Cleaner");
}

void SetConsoleSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, 99, 39};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void SetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

bool CheckAdminPrivileges() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

void PrintHeader() {
    std::wcout << L"+============================================================+\n";
    std::wcout << L"|              WINDOWS CLEANER PRO - C++ EDITION             |\n";
    std::wcout << L"|                        Version 1.0.0                       |\n";
    std::wcout << L"+============================================================+\n";
    std::wcout << L"\n";
}

void PrintMenu() {
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"| OPTIONS:                                                   |\n";
    std::wcout << L"| [1] Scan for cleanable items                               |\n";
    std::wcout << L"| [2] Clean all detected items                               |\n";
    std::wcout << L"| [3] View current session statistics                        |\n";
    std::wcout << L"| [4] View historical statistics                             |\n";
    std::wcout << L"| [5] Exit                                                   |\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"\n";
}

void PrintStats(const WindowsCleaner::CleanStats& stats) {
    std::wcout << L"\n";
    std::wcout << L"+============================================================+\n";
    std::wcout << L"|                CURRENT SESSION STATISTICS                  |\n";
    std::wcout << L"+============================================================+\n";
    std::wcout << L"\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"| Space Freed:        " << (stats.bytesFreed / 1048576) << L" MB\n";
    std::wcout << L"| Files Deleted:      " << stats.filesDeleted << L"\n";
    std::wcout << L"| Folders Deleted:    " << stats.foldersDeleted << L"\n";
    std::wcout << L"| Errors:             " << stats.errors << L"\n";
    std::wcout << L"| Warnings:           " << stats.warnings << L"\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"\n";
}

void PrintAggregatedStats(const WindowsCleaner::AggregatedStats& stats, const std::wstring& title) {
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"| " << title;
    int padding = 58 - static_cast<int>(title.length());
    for (int i = 0; i < padding; i++) std::wcout << L" ";
    std::wcout << L"|\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"| Sessions:           " << stats.sessionCount << L"\n";
    std::wcout << L"| Total Space Freed:  " << (stats.totalBytesFreed / 1048576) << L" MB\n";
    std::wcout << L"| Total Files:        " << stats.totalFilesDeleted << L"\n";
    std::wcout << L"| Total Folders:      " << stats.totalFoldersDeleted << L"\n";
    std::wcout << L"| Total Errors:       " << stats.totalErrors << L"\n";
    std::wcout << L"| Total Warnings:     " << stats.totalWarnings << L"\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"\n";
}

void PrintHistoricalStats() {
    std::wcout << L"\n";
    std::wcout << L"+============================================================+\n";
    std::wcout << L"|                  HISTORICAL STATISTICS                     |\n";
    std::wcout << L"+============================================================+\n";
    std::wcout << L"\n";
    
    auto stats24h = WindowsCleaner::StatsDatabase::GetInstance().GetStats24Hours();
    auto stats7d = WindowsCleaner::StatsDatabase::GetInstance().GetStats7Days();
    auto stats31d = WindowsCleaner::StatsDatabase::GetInstance().GetStats31Days();
    auto statsAll = WindowsCleaner::StatsDatabase::GetInstance().GetStatsOverall();
    
    PrintAggregatedStats(stats24h, L"LAST 24 HOURS");
    PrintAggregatedStats(stats7d, L"LAST 7 DAYS");
    PrintAggregatedStats(stats31d, L"LAST 31 DAYS");
    PrintAggregatedStats(statsAll, L"ALL TIME");
    
    // Show recent sessions
    auto recentSessions = WindowsCleaner::StatsDatabase::GetInstance().GetRecentSessions(5);
    if (!recentSessions.empty()) {
        std::wcout << L"+------------------------------------------------------------+\n";
        std::wcout << L"|                    RECENT SESSIONS                         |\n";
        std::wcout << L"+------------------------------------------------------------+\n";
        
        for (const auto& session : recentSessions) {
            tm timeInfo;
            localtime_s(&timeInfo, &session.timestamp);
            wchar_t timeStr[64];
            wcsftime(timeStr, 64, L"%Y-%m-%d %H:%M:%S", &timeInfo);
            
            std::wcout << L"| " << timeStr << L" - " 
                      << (session.bytesFreed / 1048576) << L" MB freed";
            std::wcout << L"\n";
        }
        std::wcout << L"+------------------------------------------------------------+\n";
    }
    
    std::wcout << L"\n";
}

void PrintDetectedApps(const std::vector<std::wstring>& apps) {
    std::wcout << L"\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"| DETECTED APPLICATIONS:                                     |\n";
    std::wcout << L"+------------------------------------------------------------+\n";
    
    if (apps.empty()) {
        std::wcout << L"| No applications detected                                   |\n";
    } else {
        for (const auto& app : apps) {
            std::wcout << L"| [X] " << app;
            // Pad to align
            int padding = 56 - static_cast<int>(app.length());
            for (int i = 0; i < padding; i++) std::wcout << L" ";
            std::wcout << L"|\n";
        }
    }
    
    std::wcout << L"+------------------------------------------------------------+\n";
    std::wcout << L"\n";
}

int main() {
    try {
        // Set console properties first
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hConsole, &mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, mode);
        
        // Set UTF-8 for console
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        
        // Set locale for wide character output
        std::locale::global(std::locale(""));
        std::wcout.imbue(std::locale());
        
        SetConsoleTitle();
        SetConsoleColor();
        
        // Initialize logger
        std::wstring userProfile = WindowsCleaner::FileUtils::GetEnvVar(L"USERPROFILE");
        std::wstring logDir = userProfile + L"\\CleanerLogs";
        WindowsCleaner::FileUtils::CreateDirectoryRecursive(logDir);
    
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t timestamp[64];
    swprintf_s(timestamp, L"%04d%02d%02d_%02d%02d%02d", 
               st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    
    std::wstring logFile = logDir + L"\\clean_" + timestamp + L".log";
    std::wstring reportFile = logDir + L"\\report_" + timestamp + L".json";
    std::wstring statsDbFile = logDir + L"\\stats.db";
    
    WindowsCleaner::Logger::GetInstance().Initialize(logFile);
    WindowsCleaner::StatsDatabase::GetInstance().Initialize(statsDbFile);
    
    // Check admin privileges
    if (!CheckAdminPrivileges()) {
        WindowsCleaner::Logger::GetInstance().LogToConsole(
            WindowsCleaner::LogLevel::WARNING, 
            L"Not running as Administrator. Some operations may fail.");
        std::wcout << L"Right-click and select 'Run as administrator' for full functionality.\n\n";
        system("pause");
    } else {
        WindowsCleaner::Logger::GetInstance().LogToConsole(
            WindowsCleaner::LogLevel::SUCCESS, 
            L"Running with Administrator privileges.");
    }
    
    // Create cleaner instance
    WindowsCleaner::Cleaner cleaner;
    
    // Main loop
    bool running = true;
    while (running) {
        system("cls");
        PrintHeader();
        
        std::wcout << L"Log file: " << logFile << L"\n\n";
        
        PrintMenu();
        
        std::wcout << L"Enter your choice: ";
        int choice;
        std::wcin >> choice;
        
        switch (choice) {
            case 1: {
                system("cls");
                PrintHeader();
                cleaner.DetectApplications();
                PrintDetectedApps(cleaner.GetDetectedApps());
                system("pause");
                break;
            }
            
            case 2: {
                system("cls");
                PrintHeader();
                
                std::wcout << L"Proceed with cleaning? (y/n): ";
                wchar_t confirm;
                std::wcin >> confirm;
                
                if (confirm == L'y' || confirm == L'Y') {
                    std::wcout << L"\n";
                    cleaner.CleanAll();
                    cleaner.GenerateReport(reportFile);
                    cleaner.SaveStatsToDatabase();
                    PrintStats(cleaner.GetStats());
                    std::wcout << L"Report saved to: " << reportFile << L"\n";
                    std::wcout << L"Stats saved to database\n\n";
                } else {
                    WindowsCleaner::Logger::GetInstance().LogToConsole(
                        WindowsCleaner::LogLevel::INFO, L"Cleaning cancelled by user");
                }
                
                system("pause");
                break;
            }
            
            case 3: {
                system("cls");
                PrintHeader();
                PrintStats(cleaner.GetStats());
                system("pause");
                break;
            }
            
            case 4: {
                system("cls");
                PrintHeader();
                PrintHistoricalStats();
                system("pause");
                break;
            }
            
            case 5: {
                WindowsCleaner::Logger::GetInstance().LogToConsole(
                    WindowsCleaner::LogLevel::INFO, L"Application closed by user");
                running = false;
                break;
            }
            
            default: {
                WindowsCleaner::Logger::GetInstance().LogToConsole(
                    WindowsCleaner::LogLevel::WARNING, L"Invalid option selected");
                system("pause");
                break;
            }
        }
    }
    
    WindowsCleaner::Logger::GetInstance().Close();
    
    std::wcout << L"\nThank you for using Windows Cleaner Pro!\n";
    Sleep(2000);
    
    return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        system("pause");
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred!" << std::endl;
        system("pause");
        return 1;
    }
}
