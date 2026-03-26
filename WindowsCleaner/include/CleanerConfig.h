#pragma once
#include <string>
#include <vector>

namespace WindowsCleaner {

struct CleanTarget {
    std::wstring name;
    std::wstring path;
    bool isRecursive;
    bool deleteFolder;
};

class CleanerConfig {
public:
    static std::vector<CleanTarget> GetSystemTargets();
    static std::vector<CleanTarget> GetBrowserTargets();
    static std::vector<CleanTarget> GetDevToolTargets();
    static std::vector<CleanTarget> GetGamingTargets();
    static std::vector<CleanTarget> GetMediaTargets();
    static std::vector<CleanTarget> GetAllTargets();
    
    static std::wstring ExpandPath(const std::wstring& path);
};

} // namespace WindowsCleaner
