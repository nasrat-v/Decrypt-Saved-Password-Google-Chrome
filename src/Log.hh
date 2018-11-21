#pragma once

#include <iostream>
#include <fstream>

# define LOGFILE_PATH   "log.txt"
# define PASSWORD_PATH	"password.txt"

static bool		_tryStatus;

class Log
{
public:

	static void 	logPassword(const std::string &password);
	static void 	logSuccessMsg(const std::string &msg);
	static void 	logFailureMsg(const std::string &msg);
	static void 	logInfoMsg(const std::string &msg);
	static void 	logTryMsg(const std::string &msg);
	static void	logSomething(const std::string &msg, const std::string &filePath = LOGFILE_PATH);

private:
	Log();
	~Log();
};
