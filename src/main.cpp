
#include "BrowserPassword.hh"

int main(int ac, const char **av) 
{
	BrowserPassword core;

	Log::logSomething("\n[Start new analyse]");
	// Chrome
	core.setBrowserType(BrowserPassword::BrowserType::CHROME);
	Log::logSomething("\n\\ Launch Chrome analyse /\n");
	core.databaseSpying();
	// Opera
	core.setBrowserType(BrowserPassword::BrowserType::OPERA);
	Log::logSomething("\n\\ Launch Opera analyse /\n");
	core.databaseSpying();
	Log::logSomething("\n[Analyse terminated]\n");
	return (0);
}
