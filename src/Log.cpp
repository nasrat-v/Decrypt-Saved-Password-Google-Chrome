
#include "Log.hh"

Log::Log()
{
	_tryStatus = false;
}

Log::~Log() = default;

/**
 * Static function, can be called everywhere.
 * Log something in a log file
 * @param complementary_msg Additional message to display
 * @param file_path Special filepath, if empty the default file path is the macro LOGFILE_PATH
 */
 void Log::logSomething(const std::string &msg, const std::string &filePath)
{
	std::fstream file(filePath, std::fstream::out | std::fstream::app);

	file << msg.c_str() << std::endl;
	file.close();
}

 /**
 * Static function, can be called everywhere.
 * Log password in password log file
 * @param password Password infos to display
 */
 void Log::logPassword(const std::string &password)
 {
	 logSomething(password, PASSWORD_PATH);
 }

 void Log::logSuccessMsg(const std::string &msg)
 {
	 if (_tryStatus)
	 {
		 logSomething("\t\t[OK] " + msg);
		 _tryStatus = false;
	 }
	 else
		 logSomething("[OK] " + msg);
 }

 void Log::logFailureMsg(const std::string &msg)
 {
	 if (_tryStatus)
	 {
		 logSomething("\t\t[KO] " + msg);
		 _tryStatus = false;
	 }
	 else
		 logSomething("[KO] " + msg);
 }

 void Log::logInfoMsg(const std::string &msg)
 {
	 logSomething("\n[info] " + msg);
 }

 void Log::logTryMsg(const std::string &msg)
 {
	 logSomething("\t[try] " + msg);
	 _tryStatus = true;
 }
