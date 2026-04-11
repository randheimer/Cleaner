#include "../../include/filesystem/FileOperations.h"
#include <windows.h>
#include <shlobj.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace WindowsCleaner {

bool FileOperations::DeleteDirectory(const std::wstring& path, bool recursive) {
    try {
        if (!FileOperations::DirectoryExists(path)) return true;
        
        if (recursive) {
            fs::remove_all(path);
        } else {
            fs::remove(path);
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool FileOperations::RemoveFile(const std::wstring& path) {
    try {
        if (!FileOperations::FileExists(path)) return true;
        fs::remove(path);
        return true;
    } catch (...) {
        return false;
    }
}

bool FileOperations::DirectoryExists(const std::wstring& path) {
    try {
        return fs::exists(path) && fs::is_directory(path);
    } catch (...) {
        return false;
    }
}

bool FileOperations::FileExists(const std::wstring& path) {
    try {
        return fs::exists(path) && fs::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

uint64_t FileOperations::GetDirectorySize(const std::wstring& path) {
    uint64_t size = 0;
    try {
        if (!DirectoryExists(path)) return 0;
        
        for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
            try {
                if (fs::is_regular_file(entry.status())) {
                    size += fs::file_size(entry.path());
                }
            } catch (...) {
                continue;
            }
        }
    } catch (...) {
        return size;
    }
    return size;
}

uint64_t FileOperations::GetFileSize(const std::wstring& path) {
    try {
        if (!FileExists(path)) return 0;
        return fs::file_size(path);
    } catch (...) {
        return 0;
    }
}

int FileOperations::CountFiles(const std::wstring& path, bool recursive) {
    int count = 0;
    try {
        if (!DirectoryExists(path)) return 0;
        
        if (recursive) {
            for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
                try {
                    if (fs::is_regular_file(entry.status())) {
                        count++;
                    }
                } catch (...) {
                    continue;
                }
            }
        } else {
            for (const auto& entry : fs::directory_iterator(path)) {
                try {
                    if (fs::is_regular_file(entry.status())) {
                        count++;
                    }
                } catch (...) {
                    continue;
                }
            }
        }
    } catch (...) {
        return count;
    }
    return count;
}

int FileOperations::CountDirectories(const std::wstring& path, bool recursive) {
    int count = 0;
    try {
        if (!DirectoryExists(path)) return 0;
        
        if (recursive) {
            for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
                try {
                    if (fs::is_directory(entry.status())) {
                        count++;
                    }
                } catch (...) {
                    continue;
                }
            }
        } else {
            for (const auto& entry : fs::directory_iterator(path)) {
                try {
                    if (fs::is_directory(entry.status())) {
                        count++;
                    }
                } catch (...) {
                    continue;
                }
            }
        }
    } catch (...) {
        return count;
    }
    return count;
}

bool FileOperations::CreateDirectoryRecursive(const std::wstring& path) {
    try {
        fs::create_directories(path);
        return true;
    } catch (...) {
        return false;
    }
}

std::wstring FileOperations::GetEnvVar(const std::wstring& name) {
    wchar_t buffer[32767];
    DWORD result = ::GetEnvironmentVariableW(name.c_str(), buffer, 32767);
    if (result > 0 && result < 32767) {
        return std::wstring(buffer);
    }
    return L"";
}

} // namespace WindowsCleaner
