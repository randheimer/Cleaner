#include "../../include/scanning/CleanTargets.h"
#include "../../include/filesystem/FileOperations.h"

namespace WindowsCleaner {

std::wstring CleanTargets::ExpandPath(const std::wstring& path) {
    std::wstring expanded = path;
    
    size_t pos = 0;
    while ((pos = expanded.find(L'%', pos)) != std::wstring::npos) {
        size_t endPos = expanded.find(L'%', pos + 1);
        if (endPos == std::wstring::npos) break;
        
        std::wstring varName = expanded.substr(pos + 1, endPos - pos - 1);
        std::wstring varValue = FileOperations::GetEnvVar(varName);
        
        if (!varValue.empty()) {
            expanded.replace(pos, endPos - pos + 1, varValue);
            pos += varValue.length();
        } else {
            pos = endPos + 1;
        }
    }
    
    return expanded;
}

std::vector<CleanTarget> CleanTargets::GetSystemTargets() {
    return {
        // Temporary Files
        {L"User TEMP", L"%TEMP%", true, true},
        {L"Windows TEMP", L"C:\\Windows\\Temp", true, false},
        {L"Prefetch", L"C:\\Windows\\Prefetch", true, false},
        
        // Windows Cache
        {L"Recent Files", L"%APPDATA%\\Microsoft\\Windows\\Recent", true, false},
        {L"Windows Update Cache", L"C:\\Windows\\SoftwareDistribution\\Download", true, false},
        {L"Delivery Optimization", L"C:\\Windows\\SoftwareDistribution\\DeliveryOptimization", true, false},
        {L"Windows Installer Cache", L"C:\\Windows\\Installer\\$PatchCache$", true, false},
        
        // Graphics Cache
        {L"Shader Cache", L"%LOCALAPPDATA%\\D3DSCache", true, false},
        {L"DirectX Shader Cache", L"%LOCALAPPDATA%\\Microsoft\\DirectX", true, false},
        {L"Thumbnail Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Explorer", false, false},
        {L"Icon Cache", L"%LOCALAPPDATA%\\IconCache.db", false, false},
        
        // Error Reports & Logs
        {L"Error Reports", L"C:\\ProgramData\\Microsoft\\Windows\\WER", true, false},
        {L"Crash Dumps", L"%LOCALAPPDATA%\\CrashDumps", true, false},
        {L"Windows Logs", L"C:\\Windows\\Logs", true, false},
        {L"Minidump Files", L"C:\\Windows\\Minidump", true, false},
        
        // Windows Store
        {L"Windows Store Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsStore_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Windows Store Temp", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsStore_8wekyb3d8bbwe\\TempState", true, true},
        
        // Microsoft Edge WebView
        {L"Edge WebView Cache", L"%LOCALAPPDATA%\\Microsoft\\EdgeWebView\\Cache", true, true},
        
        // Windows Defender
        {L"Windows Defender Scans", L"C:\\ProgramData\\Microsoft\\Windows Defender\\Scans\\History", true, false},
        
        // Font Cache
        {L"Font Cache", L"C:\\Windows\\ServiceProfiles\\LocalService\\AppData\\Local\\FontCache", true, false},
        
        // Network Cache
        {L"Network List Cache", L"C:\\Windows\\ServiceProfiles\\NetworkService\\AppData\\Local", true, false},
        
        // Memory Dumps
        {L"System Memory Dumps", L"C:\\Windows\\MEMORY.DMP", false, false},
        
        // Downloaded Program Files
        {L"Downloaded Program Files", L"C:\\Windows\\Downloaded Program Files", true, false},
        
        // Offline Web Pages
        {L"Temporary Internet Files", L"%LOCALAPPDATA%\\Microsoft\\Windows\\INetCache", true, true}
    };
}

std::vector<CleanTarget> CleanTargets::GetBrowserTargets() {
    return {
        // Chrome - Cache only, preserves cookies/passwords/history
        {L"Chrome Cache", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Cache", true, true},
        {L"Chrome Code Cache", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Code Cache", true, true},
        {L"Chrome GPU Cache", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\GPUCache", true, true},
        {L"Chrome Service Worker Cache", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Service Worker", true, true},
        
        // Edge - Cache only, preserves cookies/passwords/history
        {L"Edge Cache", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Default\\Cache", true, true},
        {L"Edge Code Cache", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Default\\Code Cache", true, true},
        {L"Edge GPU Cache", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Default\\GPUCache", true, true},
        
        // Brave - Cache only, preserves cookies/passwords/history
        {L"Brave Cache", L"%LOCALAPPDATA%\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Cache", true, true},
        {L"Brave Code Cache", L"%LOCALAPPDATA%\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Code Cache", true, true},
        {L"Brave GPU Cache", L"%LOCALAPPDATA%\\BraveSoftware\\Brave-Browser\\User Data\\Default\\GPUCache", true, true},
        
        // Firefox - Cache only, preserves cookies/passwords/history
        {L"Firefox Cache", L"%LOCALAPPDATA%\\Mozilla\\Firefox\\Profiles", true, false},
        
        // Opera
        {L"Opera Cache", L"%APPDATA%\\Opera Software\\Opera Stable\\Cache", true, true},
        {L"Opera Code Cache", L"%APPDATA%\\Opera Software\\Opera Stable\\Code Cache", true, true}
    };
}

std::vector<CleanTarget> CleanTargets::GetDevToolTargets() {
    return {
        // VS Code
        {L"VS Code Cache", L"%APPDATA%\\Code\\Cache", true, true},
        {L"VS Code CachedData", L"%APPDATA%\\Code\\CachedData", true, true},
        {L"VS Code GPU Cache", L"%APPDATA%\\Code\\GPUCache", true, true},
        {L"VS Code Logs", L"%APPDATA%\\Code\\logs", true, true},
        {L"VS Code Workspace Storage", L"%APPDATA%\\Code\\User\\workspaceStorage", true, true},
        
        // VS Code Insiders
        {L"VS Code Insiders Cache", L"%APPDATA%\\Code - Insiders\\Cache", true, true},
        {L"VS Code Insiders GPU Cache", L"%APPDATA%\\Code - Insiders\\GPUCache", true, true},
        {L"VS Code Insiders Logs", L"%APPDATA%\\Code - Insiders\\logs", true, true},
        
        // Cursor IDE
        {L"Cursor Cache", L"%APPDATA%\\Cursor\\Cache", true, true},
        {L"Cursor GPU Cache", L"%APPDATA%\\Cursor\\GPUCache", true, true},
        {L"Cursor Logs", L"%APPDATA%\\Cursor\\logs", true, true},
        
        // Visual Studio
        {L"Visual Studio 2022 Cache", L"%LOCALAPPDATA%\\Microsoft\\VisualStudio\\17.0", true, false},
        {L"Visual Studio ComponentModelCache", L"%LOCALAPPDATA%\\Microsoft\\VisualStudio\\ComponentModelCache", true, true},
        
        // Package Managers
        {L"NuGet Packages", L"%USERPROFILE%\\.nuget\\packages", true, true},
        {L"npm Cache", L"%APPDATA%\\npm-cache", true, true},
        {L"Yarn Cache", L"%LOCALAPPDATA%\\Yarn\\Cache", true, true},
        {L"pnpm Cache", L"%LOCALAPPDATA%\\pnpm-cache", true, true},
        {L"pip Cache", L"%LOCALAPPDATA%\\pip\\Cache", true, true},
        {L"Composer Cache", L"%APPDATA%\\Composer\\cache", true, true},
        {L"Gradle Cache", L"%USERPROFILE%\\.gradle\\caches", true, true},
        {L"Maven Repository", L"%USERPROFILE%\\.m2\\repository", true, true},
        {L"Cargo Registry", L"%USERPROFILE%\\.cargo\\registry", true, true},
        {L"Go Module Cache", L"%USERPROFILE%\\go\\pkg\\mod", true, true},
        
        // Build Tools
        {L"MSBuild Logs", L"%LOCALAPPDATA%\\Microsoft\\MSBuild", true, true},
        {L"CMake Cache", L"%USERPROFILE%\\.cmake", true, true}
    };
}

std::vector<CleanTarget> CleanTargets::GetGamingTargets() {
    return {
        // Discord
        {L"Discord Cache", L"%APPDATA%\\Discord\\Cache", true, true},
        {L"Discord Code Cache", L"%APPDATA%\\Discord\\Code Cache", true, true},
        {L"Discord GPU Cache", L"%APPDATA%\\Discord\\GPUCache", true, true},
        {L"Discord Blob Storage", L"%APPDATA%\\Discord\\blob_storage", true, true},
        
        // Roblox
        {L"Roblox Logs", L"%LOCALAPPDATA%\\Roblox\\logs", true, true},
        {L"Roblox HTTP Cache", L"%LOCALAPPDATA%\\Roblox\\http", true, true},
        
        // Steam
        {L"Steam Cache", L"C:\\Program Files (x86)\\Steam\\appcache", true, false},
        {L"Steam HTML Cache", L"C:\\Program Files (x86)\\Steam\\config\\htmlcache", true, false},
        {L"Steam Shader Cache", L"C:\\Program Files (x86)\\Steam\\steamapps\\shadercache", true, false},
        {L"Steam Logs", L"C:\\Program Files (x86)\\Steam\\logs", true, false},
        
        // Ubisoft Connect
        {L"Ubisoft Cache", L"%LOCALAPPDATA%\\Ubisoft Game Launcher\\cache", true, true},
        {L"Ubisoft Logs", L"%LOCALAPPDATA%\\Ubisoft Game Launcher\\logs", true, true},
        
        // FiveM
        {L"FiveM Cache", L"%LOCALAPPDATA%\\FiveM\\FiveM.app\\cache", true, true},
        {L"FiveM Logs", L"%LOCALAPPDATA%\\FiveM\\FiveM.app\\logs", true, true},
        {L"FiveM Crashes", L"%LOCALAPPDATA%\\FiveM\\FiveM.app\\crashes", true, true},
        
        // Epic Games
        {L"Epic Games Logs", L"%LOCALAPPDATA%\\EpicGamesLauncher\\Saved\\Logs", true, true},
        {L"Epic Games Webcache", L"%LOCALAPPDATA%\\EpicGamesLauncher\\Saved\\webcache", true, true},
        
        // GOG Galaxy
        {L"GOG Galaxy Logs", L"%PROGRAMDATA%\\GOG.com\\Galaxy\\logs", true, true},
        {L"GOG Galaxy Webcache", L"%LOCALAPPDATA%\\GOG.com\\Galaxy\\webcache", true, true},
        
        // Origin/EA
        {L"Origin Logs", L"%APPDATA%\\Origin\\Logs", true, true},
        {L"EA App Logs", L"%LOCALAPPDATA%\\Electronic Arts\\EA Desktop\\Logs", true, true},
        
        // Battle.net
        {L"Battle.net Logs", L"%APPDATA%\\Battle.net\\Logs", true, true},
        {L"Battle.net Cache", L"%APPDATA%\\Battle.net\\Cache", true, true},
        
        // Riot Games
        {L"Riot Client Logs", L"%LOCALAPPDATA%\\Riot Games\\Riot Client\\Logs", true, true},
        {L"League of Legends Logs", L"%LOCALAPPDATA%\\Riot Games\\League of Legends\\Logs", true, true},
        {L"Valorant Logs", L"%LOCALAPPDATA%\\VALORANT\\Saved\\Logs", true, true},
        
        // Xbox
        {L"Xbox App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.GamingApp_8wekyb3d8bbwe\\LocalCache", true, true},
        
        // NVIDIA
        {L"NVIDIA Shader Cache", L"%LOCALAPPDATA%\\NVIDIA\\DXCache", true, true},
        {L"NVIDIA GL Cache", L"%LOCALAPPDATA%\\NVIDIA\\GLCache", true, true},
        {L"NVIDIA Optimus Cache", L"C:\\ProgramData\\NVIDIA Corporation\\Drs", true, true},
        {L"NVIDIA GeForce Experience Cache", L"%LOCALAPPDATA%\\NVIDIA Corporation\\GeForce Experience\\CefCache", true, true},
        {L"NVIDIA Logs", L"C:\\ProgramData\\NVIDIA Corporation\\NvBackend", true, false},
        
        // AMD
        {L"AMD Shader Cache", L"%LOCALAPPDATA%\\AMD\\DxCache", true, true},
        {L"AMD OpenGL Cache", L"%LOCALAPPDATA%\\AMD\\GLCache", true, true},
        {L"AMD VulkanCache", L"%LOCALAPPDATA%\\AMD\\VkCache", true, true},
        {L"AMD CN Cache", L"%LOCALAPPDATA%\\AMD\\CN", true, true},
        {L"Radeon Software Cache", L"C:\\ProgramData\\AMD\\PPC", true, true},
        {L"AMD Logs", L"C:\\ProgramData\\AMD\\AMD Software\\Logs", true, true},
        
        // Game Screenshots/Recordings
        {L"NVIDIA ShadowPlay", L"%USERPROFILE%\\Videos\\NVIDIA", true, false},
        {L"Xbox Game DVR", L"%USERPROFILE%\\Videos\\Captures", true, false},
        {L"Medal Clips", L"%USERPROFILE%\\Videos\\Medal", true, false},
        
        // Rainbow Six Siege
        {L"R6 Siege Backup Settings", L"%USERPROFILE%\\Documents\\My Games\\Rainbow Six - Siege", false, false}
    };
}

std::vector<CleanTarget> CleanTargets::GetMediaTargets() {
    return {
        // Media Players
        {L"VLC Cache", L"%APPDATA%\\vlc\\cache", true, true},
        {L"Windows Media Player Cache", L"%LOCALAPPDATA%\\Microsoft\\Media Player", true, false},
        {L"Spotify Cache", L"%APPDATA%\\Spotify\\Cache", true, true},
        {L"Spotify Data", L"%APPDATA%\\Spotify\\Data", true, true},
        
        // Communication Apps
        {L"Skype Cache", L"%APPDATA%\\Skype\\Cache", true, true},
        {L"Skype Media Cache", L"%APPDATA%\\Skype\\Media Cache", true, true},
        {L"Teams Cache", L"%APPDATA%\\Microsoft\\Teams\\Cache", true, true},
        {L"Teams GPU Cache", L"%APPDATA%\\Microsoft\\Teams\\GPUCache", true, true},
        {L"Teams Blob Storage", L"%APPDATA%\\Microsoft\\Teams\\blob_storage", true, true},
        {L"Zoom Cache", L"%APPDATA%\\Zoom\\logs", true, true},
        
        // Adobe Products
        {L"Adobe Cache", L"%LOCALAPPDATA%\\Adobe\\Common\\Media Cache Files", true, true},
        {L"Adobe Temp", L"%LOCALAPPDATA%\\Temp\\Adobe", true, true},
        {L"Photoshop Temp", L"%LOCALAPPDATA%\\Adobe\\Adobe Photoshop", true, false},
        
        // Office
        {L"Office Cache", L"%LOCALAPPDATA%\\Microsoft\\Office\\16.0\\OfficeFileCache", true, true},
        {L"Office Temp", L"%LOCALAPPDATA%\\Microsoft\\Office\\UnsavedFiles", true, true},
        {L"OneNote Cache", L"%LOCALAPPDATA%\\Microsoft\\OneNote", true, false},
        
        // Cloud Storage
        {L"OneDrive Temp", L"%LOCALAPPDATA%\\Microsoft\\OneDrive\\logs", true, true},
        {L"Dropbox Cache", L"%LOCALAPPDATA%\\Dropbox\\instance_db", true, true},
        {L"Google Drive Cache", L"%LOCALAPPDATA%\\Google\\DriveFS\\Logs", true, true},
        
        // Torrent Clients
        {L"qBittorrent Logs", L"%LOCALAPPDATA%\\qBittorrent\\logs", true, true},
        {L"uTorrent Cache", L"%APPDATA%\\uTorrent", true, false},
        
        // Other
        {L"Java Cache", L"%LOCALAPPDATA%\\Sun\\Java\\Deployment\\cache", true, true},
        {L"Flash Player Cache", L"%APPDATA%\\Adobe\\Flash Player", true, false},
        {L"Temp Setup Files", L"C:\\Windows\\Temp\\*.tmp", false, false},
        
        // Browsers - Additional
        {L"Chrome Crash Reports", L"%LOCALAPPDATA%\\Google\\CrashReports", true, true},
        {L"Edge Crash Reports", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Crashpad", true, true},
        {L"Firefox Crash Reports", L"%APPDATA%\\Mozilla\\Firefox\\Crash Reports", true, true},
        
        // Development - Additional
        {L"Android SDK Temp", L"%USERPROFILE%\\.android\\cache", true, true},
        {L"Android AVD Temp", L"%USERPROFILE%\\.android\\avd\\*.avd\\snapshots", true, false},
        {L"Flutter Pub Cache", L"%LOCALAPPDATA%\\Pub\\Cache", true, true},
        {L"Dart Pub Cache", L"%APPDATA%\\Pub\\Cache", true, true},
        {L"Ruby Gems Cache", L"%USERPROFILE%\\.gem", true, false},
        {L"Bundler Cache", L"%USERPROFILE%\\.bundle\\cache", true, true},
        {L"CocoaPods Cache", L"%USERPROFILE%\\Library\\Caches\\CocoaPods", true, true},
        {L"Homebrew Cache", L"%USERPROFILE%\\Library\\Caches\\Homebrew", true, true},
        
        // IDEs - Additional
        {L"JetBrains Logs", L"%LOCALAPPDATA%\\JetBrains", true, false},
        {L"IntelliJ IDEA Cache", L"%LOCALAPPDATA%\\JetBrains\\IntelliJIdea", true, false},
        {L"PyCharm Cache", L"%LOCALAPPDATA%\\JetBrains\\PyCharm", true, false},
        {L"WebStorm Cache", L"%LOCALAPPDATA%\\JetBrains\\WebStorm", true, false},
        {L"Android Studio Cache", L"%LOCALAPPDATA%\\Google\\AndroidStudio", true, false},
        {L"Eclipse Workspace Metadata", L"%USERPROFILE%\\eclipse-workspace\\.metadata\\.log", false, false},
        {L"NetBeans Cache", L"%APPDATA%\\NetBeans\\Cache", true, true},
        {L"Sublime Text Cache", L"%APPDATA%\\Sublime Text\\Cache", true, true},
        {L"Atom Cache", L"%APPDATA%\\Atom\\Cache", true, true},
        {L"Notepad++ Backup", L"%APPDATA%\\Notepad++\\backup", true, true},
        
        // Gaming - Additional
        {L"Rockstar Games Launcher Cache", L"%LOCALAPPDATA%\\Rockstar Games\\Launcher\\Cache", true, true},
        {L"Rockstar Social Club Cache", L"%LOCALAPPDATA%\\Rockstar Games\\Social Club\\Cache", true, true},
        {L"Bethesda Launcher Cache", L"%LOCALAPPDATA%\\Bethesda.net Launcher\\Cache", true, true},
        {L"Minecraft Logs", L"%APPDATA%\\.minecraft\\logs", true, true},
        {L"Minecraft Crash Reports", L"%APPDATA%\\.minecraft\\crash-reports", true, true},
        {L"Fortnite Logs", L"%LOCALAPPDATA%\\FortniteGame\\Saved\\Logs", true, true},
        {L"Apex Legends Crash Dumps", L"%USERPROFILE%\\Saved Games\\Respawn\\Apex\\local\\crash_data", true, true},
        {L"Call of Duty Cache", L"%PROGRAMDATA%\\Blizzard Entertainment\\Battle.net\\Cache", true, true},
        {L"Warzone Logs", L"%USERPROFILE%\\Documents\\Call of Duty Modern Warfare\\players\\logs", true, true},
        {L"GTA V Logs", L"%USERPROFILE%\\Documents\\Rockstar Games\\GTA V\\logs", true, true},
        {L"Red Dead Redemption 2 Logs", L"%USERPROFILE%\\Documents\\Rockstar Games\\Red Dead Redemption 2\\logs", true, true},
        {L"Overwatch Logs", L"%USERPROFILE%\\Documents\\Overwatch\\Logs", true, true},
        {L"World of Warcraft Cache", L"%PROGRAMDATA%\\Blizzard Entertainment\\World of Warcraft\\Cache", true, true},
        {L"Hearthstone Logs", L"%LOCALAPPDATA%\\Blizzard\\Hearthstone\\Logs", true, true},
        {L"Dota 2 Logs", L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\dota 2 beta\\game\\dota\\logs", true, true},
        {L"CS:GO Logs", L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\logs", true, true},
        {L"Rust Logs", L"%APPDATA%\\Rust\\logs", true, true},
        {L"Escape from Tarkov Logs", L"%APPDATA%\\Battlestate Games\\EscapeFromTarkov\\Logs", true, true},
        {L"Genshin Impact Logs", L"%USERPROFILE%\\AppData\\LocalLow\\miHoYo\\Genshin Impact\\output_log.txt", false, false},
        
        // Emulators
        {L"BlueStacks Logs", L"%PROGRAMDATA%\\BlueStacks\\Logs", true, true},
        {L"Nox Player Logs", L"%LOCALAPPDATA%\\Nox\\log", true, true},
        {L"PCSX2 Logs", L"%USERPROFILE%\\Documents\\PCSX2\\logs", true, true},
        {L"Dolphin Emulator Cache", L"%USERPROFILE%\\Documents\\Dolphin Emulator\\Cache", true, true},
        {L"Cemu Cache", L"%APPDATA%\\Cemu\\shaderCache", true, true},
        {L"Yuzu Cache", L"%APPDATA%\\yuzu\\cache", true, true},
        {L"Ryujinx Cache", L"%APPDATA%\\Ryujinx\\cache", true, true},
        
        // Streaming/Recording
        {L"OBS Studio Logs", L"%APPDATA%\\obs-studio\\logs", true, true},
        {L"OBS Studio Crash Reports", L"%APPDATA%\\obs-studio\\crashes", true, true},
        {L"Streamlabs OBS Logs", L"%APPDATA%\\slobs-client\\logs", true, true},
        {L"XSplit Logs", L"%APPDATA%\\SplitmediaLabs\\XSplit\\logs", true, true},
        {L"Bandicam Temp", L"%USERPROFILE%\\Documents\\Bandicam\\temp", true, true},
        {L"Fraps Logs", L"C:\\Fraps\\logs", true, true},
        
        // Design/3D Software
        {L"Autodesk Temp", L"%LOCALAPPDATA%\\Autodesk\\Temp", true, true},
        {L"3ds Max Temp", L"%LOCALAPPDATA%\\Autodesk\\3dsMax\\Temp", true, true},
        {L"Unity Cache", L"%LOCALAPPDATA%\\Unity\\cache", true, true},
        {L"Unreal Engine Cache", L"%LOCALAPPDATA%\\UnrealEngine\\Common\\DerivedDataCache", true, true},
        {L"Godot Cache", L"%APPDATA%\\Godot\\cache", true, true},
        
        // Databases
        {L"MongoDB Logs", L"%PROGRAMDATA%\\MongoDB\\log", true, true},
        {L"MySQL Temp", L"%PROGRAMDATA%\\MySQL\\MySQL Server 8.0\\Data\\#sql", true, false},
        {L"PostgreSQL Logs", L"%PROGRAMDATA%\\PostgreSQL\\logs", true, true},
        {L"Redis Dump", L"%PROGRAMDATA%\\Redis\\dump.rdb", false, false},
        
        // Virtualization
        {L"VirtualBox Logs", L"%USERPROFILE%\\.VirtualBox\\Logs", true, true},
        {L"VMware Logs", L"%APPDATA%\\VMware\\logs", true, true},
        {L"Hyper-V Logs", L"%PROGRAMDATA%\\Microsoft\\Windows\\Hyper-V\\Logs", true, true},
        {L"Docker Desktop Logs", L"%LOCALAPPDATA%\\Docker\\log", true, true},
        
        // Windows Features
        {L"Windows Search Index", L"%PROGRAMDATA%\\Microsoft\\Search\\Data\\Applications\\Windows", true, false},
        {L"Cortana Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.Cortana_cw5n1h2txyewy\\LocalCache", true, true},
        {L"Windows Spotlight Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets", true, true},
        {L"Windows Feedback Hub Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsFeedbackHub_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Windows Calculator Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsCalculator_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Windows Camera Roll", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.Photos_8wekyb3d8bbwe\\LocalState\\PhotosAppCache", true, true},
        
        // System Maintenance
        {L"CBS Logs", L"C:\\Windows\\Logs\\CBS", true, true},
        {L"DISM Logs", L"C:\\Windows\\Logs\\DISM", true, true},
        {L"Panther Logs", L"C:\\Windows\\Panther", true, false},
        {L"Setup Logs", L"C:\\Windows\\Setup", true, false},
        {L"Performance Logs", L"C:\\PerfLogs", true, true},
        {L"Event Viewer Logs", L"C:\\Windows\\System32\\winevt\\Logs", true, false},
        
        // Network
        {L"Network Setup Logs", L"C:\\Windows\\INF\\setupapi.dev.log", false, false},
        {L"WLAN Reports", L"C:\\ProgramData\\Microsoft\\Windows\\WlanReport", true, true},
        
        // Misc
        {L"7-Zip Temp", L"%LOCALAPPDATA%\\Temp\\7z", true, true},
        {L"WinRAR Temp", L"%LOCALAPPDATA%\\Temp\\Rar$", true, true},
        {L"WinZip Temp", L"%LOCALAPPDATA%\\Temp\\wz", true, true},
        {L"PeaZip Temp", L"%LOCALAPPDATA%\\Temp\\peazip", true, true},
        {L"Windows Installer Temp", L"C:\\Windows\\Installer\\$PatchCache$", true, true},
        {L"MSOCache", L"C:\\MSOCache", true, true},
        {L"Windows.old", L"C:\\Windows.old", true, true},
        {L"$Recycle.Bin", L"C:\\$Recycle.Bin", true, false},
        
        // Additional TEMP locations
        {L"INetCache Temp", L"%LOCALAPPDATA%\\Microsoft\\Windows\\INetCache\\IE", true, true},
        {L"WebCache Temp", L"%LOCALAPPDATA%\\Microsoft\\Windows\\WebCache", true, false},
        {L"CryptnetUrlCache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\INetCache\\CryptnetUrlCache", true, true},
        {L"Low Temp", L"%LOCALAPPDATA%\\Temp\\Low", true, true},
        
        // System Temp Folders
        {L"System32 Temp", L"C:\\Windows\\System32\\config\\systemprofile\\AppData\\Local\\Temp", true, true},
        {L"SysWOW64 Temp", L"C:\\Windows\\SysWOW64\\config\\systemprofile\\AppData\\Local\\Temp", true, true},
        {L"LocalService Temp", L"C:\\Windows\\ServiceProfiles\\LocalService\\AppData\\Local\\Temp", true, true},
        {L"NetworkService Temp", L"C:\\Windows\\ServiceProfiles\\NetworkService\\AppData\\Local\\Temp", true, true},
        
        // Browser Download Temp
        {L"Chrome Downloads Temp", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\File System", true, true},
        {L"Edge Downloads Temp", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Default\\File System", true, true},
        
        // Windows Update Temp
        {L"Windows Update Temp", L"C:\\Windows\\SoftwareDistribution\\DataStore\\Logs", true, true},
        {L"Windows Update Download Temp", L"C:\\Windows\\SoftwareDistribution\\Download\\*", true, false},
        {L"CBS Temp", L"C:\\Windows\\Logs\\CBS\\CbsPersist_*.log", false, false},
        
        // Installation Temp
        {L"Program Files Temp", L"C:\\Program Files\\Temp", true, true},
        {L"Program Files x86 Temp", L"C:\\Program Files (x86)\\Temp", true, true},
        {L"ProgramData Temp", L"C:\\ProgramData\\Temp", true, true},
        {L"Windows Temp Folders", L"C:\\Windows\\Temp\\*", true, false},
        
        // User Profile Temp
        {L"AppData Local Temp Low", L"%USERPROFILE%\\AppData\\LocalLow\\Temp", true, true},
        {L"Recent Items", L"%APPDATA%\\Microsoft\\Windows\\Recent\\AutomaticDestinations", true, true},
        {L"Recent Items Custom", L"%APPDATA%\\Microsoft\\Windows\\Recent\\CustomDestinations", true, true},
        {L"Jump Lists", L"%APPDATA%\\Microsoft\\Windows\\Recent\\*.automaticDestinations-ms", false, false},
        
        // Windows Logs
        {L"Windows Setup Logs", L"C:\\Windows\\Logs\\*.log", false, false},
        {L"Windows Panther Logs", L"C:\\Windows\\Panther\\*.log", false, false},
        {L"Windows INF Logs", L"C:\\Windows\\INF\\*.log", false, false},
        {L"Windows Debug Logs", L"C:\\Windows\\Debug", true, true},
        
        // Application Temp
        {L"Adobe Temp Files", L"%LOCALAPPDATA%\\Temp\\Adobe", true, true},
        
        // Media Temp
        
        // Download Managers
        {L"IDM Temp", L"%LOCALAPPDATA%\\IDM\\DwnlData", true, true},
        
        // Antivirus Temp
        
        // System Restore
        {L"System Restore Temp", L"C:\\System Volume Information\\Temp", true, true},
        
        // Windows Error Reporting Temp
        {L"WER Temp", L"%LOCALAPPDATA%\\Microsoft\\Windows\\WER\\Temp", true, true},
        {L"WER Report Queue", L"%PROGRAMDATA%\\Microsoft\\Windows\\WER\\Temp", true, true},
        
        // Misc System
        {L"Windows Prefetch Layout", L"C:\\Windows\\Prefetch\\Layout.ini", false, false},
        {L"Windows Prefetch Trace", L"C:\\Windows\\Prefetch\\*.pf", false, false},
        {L"ReadyBoot", L"C:\\Windows\\Prefetch\\ReadyBoot", true, true},
        {L"Superfetch", L"C:\\Windows\\Prefetch\\Ag*.db", false, false},
        
        // Intel
        {L"Intel Shader Cache", L"%LOCALAPPDATA%\\Intel\\ShaderCache", true, true},
        {L"Intel Graphics Cache", L"C:\\ProgramData\\Intel\\ShaderCache", true, true},
        
        // Windows Store Apps Cache
        {L"Microsoft Store Downloads", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsStore_8wekyb3d8bbwe\\AC\\INetCache", true, true},
        {L"Xbox Identity Provider Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.XboxIdentityProvider_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Xbox Game Bar Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.XboxGamingOverlay_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Microsoft Photos Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.Photos_8wekyb3d8bbwe\\TempState", true, true},
        {L"Microsoft People Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.People_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Microsoft Mail Cache", L"%LOCALAPPDATA%\\Packages\\microsoft.windowscommunicationsapps_8wekyb3d8bbwe\\LocalCache", true, true},
        
        // Windows Delivery Optimization
        {L"Delivery Optimization Files", L"C:\\Windows\\ServiceProfiles\\NetworkService\\AppData\\Local\\Microsoft\\Windows\\DeliveryOptimization\\Cache", true, true},
        
        // Windows Notification Cache
        {L"Notification Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Notifications", true, false},
        {L"Action Center Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\ActionCenterCache", true, true},
        
        // Windows Speech
        {L"Speech Recognition Temp", L"%LOCALAPPDATA%\\Microsoft\\Speech\\Files", true, true},
        
        // Windows Biometrics
        {L"Windows Hello Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Caches", true, false},
        
        // Windows Timeline
        {L"Activity History Cache", L"%LOCALAPPDATA%\\ConnectedDevicesPlatform", true, false},
        
        // Windows Clipboard
        {L"Clipboard History", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Clipboard", true, false},
        
        // Windows Shell Experience
        {L"Shell Experience Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Shell", true, false},
        
        // Windows Caches (Additional)
        {L"BITS Transfer Cache", L"C:\\Windows\\ServiceProfiles\\NetworkService\\AppData\\Local\\Microsoft\\Windows\\BITS", true, false},
        {L"Windows Installer Rollback", L"C:\\Windows\\Installer\\$PatchCache$\\Managed", true, true},
        
        // Browser Extensions Cache
        {L"Chrome Extensions Temp", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Extensions Temp", true, true},
        {L"Edge Extensions Temp", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Default\\Extensions Temp", true, true},
        
        // Windows Media Foundation
        {L"Media Foundation Cache", L"%LOCALAPPDATA%\\Microsoft\\Media Foundation", true, false},
        
        // Windows App Certification Kit
        {L"WACK Logs", L"%LOCALAPPDATA%\\Microsoft\\Windows App Certification Kit\\Logs", true, true},
        
        // Windows Performance Recorder
        {L"WPR Logs", L"%LOCALAPPDATA%\\Temp\\WPR", true, true},
        
        // Window Assessments
        {L"Windows Assessment Logs", L"C:\\Windows\\Performance\\WinSAT\\DataStore", true, false},
        
        // Crash Dumps (Additional)
        {L"User Crash Dumps", L"%LOCALAPPDATA%\\Microsoft\\Windows\\WER\\ReportArchive", true, true},
        {L"User Crash Queue", L"%LOCALAPPDATA%\\Microsoft\\Windows\\WER\\ReportQueue", true, true},
        
        // Windows Backup
        {L"Windows Backup Temp", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Backup", true, false},
        
        // Windows Sync
        {L"Sync Center Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\SyncCenter", true, false},
        
        // Windows Connect
        {L"Connect Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Connect", true, false},
        
        // Windows GameDVR (Additional)
        {L"GameDVR Temp", L"%LOCALAPPDATA%\\Microsoft\\Windows\\GameDVR", true, false},
        
        // Windows SettingSync
        {L"Setting Sync Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\SettingSync", true, false},
        
        // Windows Application Shortcuts
        {L"App Shortcuts Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Application Shortcuts", true, false},
        
        // Windows Burn
        {L"CD Burn Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Burn", true, false},
        
        // Windows History
        {L"File History Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\FileHistory", true, false},
        
        // Windows Ringtones
        {L"Ringtones Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Ringtones", true, false},
        
        // Windows Themes
        {L"Themes Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Themes\\CachedFiles", true, true},
        
        // Windows Sounds
        {L"Sounds Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\Sounds", true, false},
        
        // Python (Additional)
        {L"Python __pycache__", L"%USERPROFILE%\\__pycache__", true, true},
        {L"Jupyter Notebook Checkpoints", L"%USERPROFILE%\\.ipynb_checkpoints", true, true},
        {L"Python Eggs", L"%USERPROFILE%\\.eggs", true, true},
        {L"Python Pytest Cache", L"%USERPROFILE%\\.pytest_cache", true, true},
        {L"Python Tox", L"%USERPROFILE%\\.tox", true, true},
        {L"Python MyPy Cache", L"%USERPROFILE%\\.mypy_cache", true, true},
        
        // Node.js (Additional)
        {L"Node REPL History", L"%USERPROFILE%\\.node_repl_history", false, false},
        {L"NPM Debug Logs", L"%USERPROFILE%\\npm-debug.log*", false, false},
        {L"Yarn Error Logs", L"%USERPROFILE%\\yarn-error.log", false, false},
        {L"Yarn Debug Logs", L"%USERPROFILE%\\yarn-debug.log*", false, false},
        
        // Git
        {L"Git Credential Cache", L"%USERPROFILE%\\.git-credentials", false, false},
        
        // SSH
        {L"SSH Known Hosts Old", L"%USERPROFILE%\\.ssh\\known_hosts.old", false, false},
        
        // Windows Defender (Additional)
        {L"Windows Defender Quarantine", L"C:\\ProgramData\\Microsoft\\Windows Defender\\Quarantine", true, false},
        {L"Windows Defender Definition Updates", L"C:\\ProgramData\\Microsoft\\Windows Defender\\Definition Updates\\Backup", true, true},
        
        // Windows Update (Additional)
        {L"Windows Update Logs", L"C:\\Windows\\Logs\\WindowsUpdate", true, true},
        {L"Windows Update Medic", L"C:\\Windows\\Logs\\MoSetup", true, true},
        
        // Microsoft Edge (Additional)
        {L"Edge Snapshots", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\Snapshots", true, true},
        {L"Edge ShaderCache", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\ShaderCache", true, true},
        {L"Edge BrowserMetrics", L"%LOCALAPPDATA%\\Microsoft\\Edge\\User Data\\BrowserMetrics", true, true},
        
        // Chrome (Additional)
        {L"Chrome Snapshots", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Snapshots", true, true},
        {L"Chrome ShaderCache", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\ShaderCache", true, true},
        {L"Chrome BrowserMetrics", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\BrowserMetrics", true, true},
        {L"Chrome Optimization Guide", L"%LOCALAPPDATA%\\Google\\Chrome\\User Data\\OptimizationGuide", true, true},
        
        // Windows Spotlight (Additional)
        {L"Spotlight Assets", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets", true, true},
        {L"Spotlight Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\Settings", true, false},
        
        // Windows Apps (Additional)
        {L"Skype UWP Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.SkypeApp_kzf8qxf38zg5c\\LocalCache", true, true},
        {L"OneNote UWP Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.Office.OneNote_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Weather App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.BingWeather_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"News App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.BingNews_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Maps App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsMaps_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Alarms App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsAlarms_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Camera App Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsCamera_8wekyb3d8bbwe\\LocalCache", true, true},
        {L"Voice Recorder Cache", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsSoundRecorder_8wekyb3d8bbwe\\LocalCache", true, true},
        
        // Messaging Apps (Additional)
        {L"Telegram Cache", L"%APPDATA%\\Telegram Desktop\\tdata\\user_data\\cache", true, true},
        {L"Telegram Media Cache", L"%APPDATA%\\Telegram Desktop\\tdata\\user_data\\media_cache", true, true},
        {L"WhatsApp Cache", L"%LOCALAPPDATA%\\WhatsApp\\Cache", true, true},
        {L"Signal Cache", L"%APPDATA%\\Signal\\Cache", true, true},
        {L"Slack Cache", L"%APPDATA%\\Slack\\Cache", true, true},
        {L"Slack Code Cache", L"%APPDATA%\\Slack\\Code Cache", true, true},
        {L"Slack Service Worker", L"%APPDATA%\\Slack\\Service Worker", true, true},
        
        // Browsers (Additional)
        {L"Vivaldi Cache", L"%LOCALAPPDATA%\\Vivaldi\\User Data\\Default\\Cache", true, true},
        {L"Vivaldi Code Cache", L"%LOCALAPPDATA%\\Vivaldi\\User Data\\Default\\Code Cache", true, true},
        {L"Yandex Cache", L"%LOCALAPPDATA%\\Yandex\\YandexBrowser\\User Data\\Default\\Cache", true, true},
        {L"Tor Browser Cache", L"%APPDATA%\\Tor Browser\\Browser\\TorBrowser\\Data\\Browser\\Caches", true, true},
        
        // Download Managers (Additional)
        {L"uGet Temp", L"%APPDATA%\\uGet\\temp", true, true},
        {L"EagleGet Temp", L"%APPDATA%\\EagleGet\\temp", true, true},
        
        // Media Players (Additional)
        {L"MPC-HC Temp", L"%APPDATA%\\MPC-HC\\temp", true, true},
        {L"PotPlayer Temp", L"%APPDATA%\\PotPlayer\\Capture", true, true},
        {L"Kodi Cache", L"%APPDATA%\\Kodi\\cache", true, true},
        {L"Plex Cache", L"%LOCALAPPDATA%\\Plex Media Server\\Cache", true, true},
        {L"iTunes Cache", L"%APPDATA%\\Apple Computer\\iTunes\\iPod Software Updates", true, true},
        
        // Cloud Storage (Additional)
        {L"iCloud Temp", L"%LOCALAPPDATA%\\Apple\\iCloud\\Logs", true, true},
        {L"Box Cache", L"%LOCALAPPDATA%\\Box\\Box\\cache", true, true},
        {L"Mega Sync Cache", L"%LOCALAPPDATA%\\Mega Limited\\MEGAsync\\cache", true, true},
        {L"pCloud Cache", L"%LOCALAPPDATA%\\pCloud\\Cache", true, true},
        
        // Password Managers Cache
        {L"1Password Cache", L"%LOCALAPPDATA%\\1Password\\cache", true, true},
        {L"LastPass Cache", L"%LOCALAPPDATA%\\LastPass\\Cache", true, true},
        {L"Bitwarden Cache", L"%APPDATA%\\Bitwarden\\Cache", true, true},
        
        // VPN Clients
        {L"NordVPN Logs", L"%LOCALAPPDATA%\\NordVPN\\logs", true, true},
        {L"ExpressVPN Logs", L"%LOCALAPPDATA%\\ExpressVPN\\logs", true, true},
        {L"ProtonVPN Logs", L"%LOCALAPPDATA%\\ProtonVPN\\Logs", true, true},
        
        // Remote Desktop
        {L"TeamViewer Logs", L"%APPDATA%\\TeamViewer\\Logs", true, true},
        {L"AnyDesk Logs", L"%APPDATA%\\AnyDesk\\logs", true, true},
        {L"Chrome Remote Desktop Logs", L"%LOCALAPPDATA%\\Google\\Chrome Remote Desktop\\Logs", true, true},
        
        // System Utilities
        {L"CCleaner Logs", L"%APPDATA%\\Piriform\\CCleaner\\Logs", true, true},
        {L"Revo Uninstaller Logs", L"%APPDATA%\\VS Revo Group\\Revo Uninstaller Pro\\Logs", true, true},
        {L"WinDirStat Cache", L"%APPDATA%\\WinDirStat\\cache", true, true},
        
        // Compression Tools (Additional)
        {L"Bandizip Temp", L"%APPDATA%\\Bandizip\\Temp", true, true},
        {L"PowerISO Temp", L"%APPDATA%\\PowerISO\\Temp", true, true},
        {L"UltraISO Temp", L"%APPDATA%\\EZB Systems\\UltraISO\\Temp", true, true},
        
        // Windows Subsystem for Linux
        {L"WSL Temp", L"%LOCALAPPDATA%\\Temp\\wsl", true, true},
        {L"WSL Logs", L"%LOCALAPPDATA%\\Microsoft\\Windows\\WSL\\Logs", true, true},
        
        // Windows Terminal
        {L"Windows Terminal Logs", L"%LOCALAPPDATA%\\Packages\\Microsoft.WindowsTerminal_8wekyb3d8bbwe\\LocalState\\DiagOutputDir", true, true},
        
        // PowerShell
        {L"PowerShell Telemetry", L"%LOCALAPPDATA%\\Microsoft\\PowerShell\\telemetry", true, true},
        {L"PowerShell Module Analysis Cache", L"%LOCALAPPDATA%\\Microsoft\\Windows\\PowerShell\\ModuleAnalysisCache", true, true},
        
        // .NET
        {L".NET Temp Files", L"%LOCALAPPDATA%\\Temp\\.net", true, true},
        {L".NET NuGet HTTP Cache", L"%LOCALAPPDATA%\\NuGet\\v3-cache", true, true},
        
        // Windows Error Reporting (Additional)
        {L"WER Archive", L"C:\\ProgramData\\Microsoft\\Windows\\WER\\ReportArchive", true, true},
        {L"WER Temp", L"C:\\ProgramData\\Microsoft\\Windows\\WER\\Temp", true, true},
        
        // Windows Reliability
        {L"Reliability Monitor", L"C:\\ProgramData\\Microsoft\\RAC\\StateData", true, false},
        {L"Reliability Temp", L"C:\\ProgramData\\Microsoft\\RAC\\Temp", true, true},
        
        // Windows Unnecessary System Folders
        {L"Windows Installer Temp Files", L"C:\\Windows\\Installer\\$PatchCache$", true, true},
        {L"Windows SoftwareDistribution Backup", L"C:\\Windows\\SoftwareDistribution\\PostRebootEventCache.V2", true, true},
        {L"Windows Downloaded Installations", L"C:\\Windows\\Downloaded Installations", true, true},
        {L"Windows Assembly Temp", L"C:\\Windows\\assembly\\tmp", true, true},
        {L"Windows Assembly NativeImages Temp", L"C:\\Windows\\assembly\\NativeImages_*\\Temp", true, true},
        {L"Windows Microsoft.NET Temp", L"C:\\Windows\\Microsoft.NET\\Framework\\*\\Temporary ASP.NET Files", true, true},
        {L"Windows Microsoft.NET64 Temp", L"C:\\Windows\\Microsoft.NET\\Framework64\\*\\Temporary ASP.NET Files", true, true},
        {L"Windows Logs DPX", L"C:\\Windows\\Logs\\DPX", true, true},
        {L"Windows Logs DISM", L"C:\\Windows\\Logs\\DISM\\dism.log", false, false},
        {L"Windows Logs MeasuredBoot", L"C:\\Windows\\Logs\\MeasuredBoot", true, true},
        {L"Windows Logs SIH", L"C:\\Windows\\Logs\\SIH", true, true},
        {L"Windows Logs waasmedic", L"C:\\Windows\\Logs\\waasmedic", true, true},
        {L"Windows ServiceProfiles Temp", L"C:\\Windows\\ServiceProfiles\\LocalService\\AppData\\Local\\Temp", true, true},
        {L"Windows System32 LogFiles", L"C:\\Windows\\System32\\LogFiles\\WMI", true, true},
        {L"Windows System32 WDI LogFiles", L"C:\\Windows\\System32\\WDI\\LogFiles", true, true},
        {L"Windows System32 SRU", L"C:\\Windows\\System32\\SRU\\SRUDB.dat.LOG", false, false},
        {L"Windows Temp Installer Files", L"C:\\Windows\\Temp\\*.msi", false, false},
        {L"Windows LiveKernelReports", L"C:\\Windows\\LiveKernelReports", true, true},
        {L"Windows Panther Logs", L"C:\\Windows\\Panther\\UnattendGC", true, true},
        {L"Windows RemotePackages", L"C:\\Windows\\RemotePackages", true, true},
        {L"Windows rescache", L"C:\\Windows\\rescache", true, true},
        {L"Windows SystemApps Cache", L"C:\\Windows\\SystemApps\\*\\Assets", true, false},
        {L"Windows WinSxS Backup", L"C:\\Windows\\WinSxS\\Backup", true, true},
        {L"Windows WinSxS ManifestCache", L"C:\\Windows\\WinSxS\\ManifestCache", true, true},
        {L"Windows WinSxS Temp", L"C:\\Windows\\WinSxS\\Temp", true, true},
        
        // Windows Diagnostics
        {L"Windows Diagnostics Temp", L"C:\\Windows\\System32\\config\\systemprofile\\AppData\\Local\\Microsoft\\Windows\\Temporary Internet Files", true, true},
        {L"Windows Performance Diagnostics", L"C:\\Windows\\System32\\LogFiles\\WMI\\RtBackup", true, true},
        {L"Windows System Diagnostics", L"C:\\Windows\\System32\\LogFiles\\Scm", true, true},
        
        // Windows AppReadiness
        {L"AppReadiness Cache", L"C:\\Windows\\appcompat\\appraiser", true, false},
        {L"AppReadiness Logs", L"C:\\Windows\\AppReadiness", true, false},
        
        // Windows CbsTemp
        {L"CBS Temp Files", L"C:\\Windows\\Temp\\CBS", true, true},
        
        // Windows IME
        {L"IME Cache", L"%LOCALAPPDATA%\\Microsoft\\InputMethod\\Chs", true, false},
        {L"IME Logs", L"%LOCALAPPDATA%\\Microsoft\\InputMethod\\Logs", true, true},
        
        // Windows LanguageOverlayCache
        {L"Language Overlay Cache", L"C:\\Windows\\LanguageOverlayCache", true, true},
        
        // Windows Containers
        {L"Windows Containers Temp", L"C:\\ProgramData\\Microsoft\\Windows\\Containers\\BaseImages", true, false},
        
        // Windows AppRepository
        {L"App Repository Cache", L"C:\\Windows\\AppRepository", true, false},
        
        // Windows Provisioning
        {L"Provisioning Temp", L"C:\\Windows\\Provisioning\\Autopilot", true, false},
        
        // Windows SystemResources
        {L"System Resources Cache", L"C:\\Windows\\SystemResources", true, false},
        
        // Windows Registration
        {L"Registration Database Logs", L"C:\\Windows\\System32\\config\\RegBack", true, false},
        
        // Windows Servicing
        {L"Servicing Sessions", L"C:\\Windows\\servicing\\Sessions", true, true},
        {L"Servicing Temp", L"C:\\Windows\\servicing\\Temp", true, true},
        
        // Windows CatRoot
        {L"CatRoot Temp", L"C:\\Windows\\System32\\CatRoot2\\temp", true, true},
        
        // Windows DriverStore Temp
        {L"DriverStore Temp", L"C:\\Windows\\System32\\DriverStore\\Temp", true, true},
        {L"DriverStore FileRepository Temp", L"C:\\Windows\\System32\\DriverStore\\FileRepository\\*.tmp", false, false},
        
        // Windows Migration
        {L"Migration Logs", L"C:\\Windows\\Panther\\NewOS", true, false},
        
        // Windows Setup
        {L"Setup Temp", L"C:\\Windows\\Setup\\State", true, false},
        {L"Setup Scripts Temp", L"C:\\Windows\\Setup\\Scripts", true, false},
        
        // Windows SysPrep
        {L"SysPrep Panther", L"C:\\Windows\\System32\\Sysprep\\Panther", true, true},
        
        // Windows Tasks Cache
        {L"Tasks Cache", L"C:\\Windows\\System32\\Tasks\\Microsoft\\Windows\\UpdateOrchestrator", true, false},
        
        // Windows Feedback
        {L"Feedback Temp", L"C:\\ProgramData\\Microsoft\\Windows\\AppRepository\\Packages\\*\\S-*\\AppData", true, false},
        
        // Windows Remediation
        {L"Remediation Logs", L"C:\\Windows\\System32\\config\\systemprofile\\AppData\\Local\\Microsoft\\Windows\\PowerShell\\StartupProfileData-NonInteractive", true, true},
        
        // Windows NTFS
        {L"NTFS Log Files", L"C:\\$LogFile", false, false},
        {L"NTFS Bitmap", L"C:\\$Bitmap", false, false},
        
        // Windows Hibernation
        {L"Hibernation File", L"C:\\hiberfil.sys", false, false},
        {L"Swap File", L"C:\\swapfile.sys", false, false},
        
        // Windows Memory Dumps (Additional)
        {L"Kernel Memory Dumps", L"C:\\Windows\\MEMORY*.DMP", false, false},
        {L"System Memory Dumps", L"C:\\Windows\\Minidump\\*.dmp", false, false},
        
        // Windows Cryptography
        {L"Cryptography Logs", L"C:\\Windows\\System32\\config\\systemprofile\\AppData\\LocalLow\\Microsoft\\CryptnetUrlCache", true, true},
        
        // Windows NetworkList
        {L"Network List Cache", L"C:\\Windows\\System32\\config\\systemprofile\\AppData\\Local\\Microsoft\\Windows\\NetworkList", true, false},
        
        // Windows BITS
        {L"BITS Database", L"C:\\ProgramData\\Microsoft\\Network\\Downloader\\qmgr*.dat", false, false},
        
        // Windows AppCompat
        {L"AppCompat Cache", L"C:\\Windows\\appcompat\\Programs", true, false},
        {L"AppCompat Logs", L"C:\\Windows\\appcompat\\pca", true, false},
        
        // Windows Inventory
        {L"Inventory Logs", L"C:\\ProgramData\\Microsoft\\Diagnosis\\events*.db", false, false},
        
        // Windows Telemetry
        {L"Telemetry Logs", L"C:\\ProgramData\\Microsoft\\Diagnosis\\ETLLogs", true, true},
        {L"DiagTrack Logs", L"C:\\ProgramData\\Microsoft\\Diagnosis\\DiagTrack", true, true},
        
        // Windows UEV
        {L"UEV Temp", L"%LOCALAPPDATA%\\Microsoft\\UEV\\Temp", true, true},
        
        // Windows OneDrive Setup
        {L"OneDrive Setup Logs", L"%LOCALAPPDATA%\\Microsoft\\OneDrive\\setup\\logs", true, true},
        
        // Windows Retail Demo
        {L"Retail Demo Content", L"C:\\Windows\\RetailDemo", true, true}
    };
}

std::vector<CleanTarget> CleanTargets::GetAllTargets() {
    std::vector<CleanTarget> all;
    
    auto system = GetSystemTargets();
    auto browsers = GetBrowserTargets();
    auto devtools = GetDevToolTargets();
    auto gaming = GetGamingTargets();
    auto media = GetMediaTargets();
    
    all.insert(all.end(), system.begin(), system.end());
    all.insert(all.end(), browsers.begin(), browsers.end());
    all.insert(all.end(), devtools.begin(), devtools.end());
    all.insert(all.end(), gaming.begin(), gaming.end());
    all.insert(all.end(), media.begin(), media.end());
    
    return all;
}

} // namespace WindowsCleaner
