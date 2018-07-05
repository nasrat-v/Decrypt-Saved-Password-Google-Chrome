#include "BrowserPassword.hh"

int			main(int ac, char **av)
{
	// ShowWindow(GetConsoleWindow(), SW_HIDE); /* HIDE CONSOLE */
	BrowserPassword	browser;
	
	browser.getBrowserPassword();
	browser.printPassword();
	return (0);
}
