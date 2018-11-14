#pragma once

#ifndef __LOG_HH__
# define __LOG_HH__

# define LOGFILE_PATH   "password.txt"

#include <iostream>
#include <fstream>

class Log
{
public:

	static void	logSomething(const std::string &msg, const std::string &filePath = LOGFILE_PATH);

private:
	Log();
	~Log();
};

#endif /* !__LOG_HH__ */