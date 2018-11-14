
#include "BrowserPassword.hh"

int main(int ac, const char **av) 
{
	BrowserPassword core;

	Log::logSomething("\n[Start new analyse]\n");
	// Chrome
	core.setBrowserType(BrowserPassword::BrowserType::CHROME);
	Log::logSomething("Launch Chrome analyse...");
	core.databaseSpying();
	// Opera
	core.setBrowserType(BrowserPassword::BrowserType::OPERA);
	Log::logSomething("Launch Opera analyse...");
	core.databaseSpying();
	Log::logSomething("\n[Analyse terminated]\n");
	return (0);
}
