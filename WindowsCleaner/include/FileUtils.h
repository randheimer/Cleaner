#pragma once
#include <string>
#include <cstdint>

namespace WindowsCleaner {

class FileUtils {
public:
    static bool DeleteDirectory(const std::wstring& path, bool recursive = true);
    static bool RemoveFile(const std::wstring& path);
    static bool DirectoryExists(const std::wstring& path);
    static bool FileExists(const std::wstring& path);
    static uint64_t GetDirectorySize(const std::wstring& path);
    static uint64_t GetFileSize(const std::wstring& path);
    static int CountFiles(const std::wstring& path, bool recursive = true);
    static int CountDirectories(const std::wstring& path, bool recursive = true);
    static bool CreateDirectoryRecursive(const std::wstring& path);
    static std::wstring GetEnvVar(const std::wstring& name);
};

} // namespace WindowsCleaner
