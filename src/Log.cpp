
#include "Log.hh"

Log::Log() = default;

Log::~Log() = default;

/**
 * Static function, can be called everywhere.
 * Log something in a log file
 * @param complementary_msg Additional message to display
 * @param file_path Special filepath, if empty the default file path is the macro LOGFILE_PATH
 */
void                Log::logSomething(const std::string &msg, const std::string &filePath)
{
	std::fstream    file(filePath, std::fstream::out | std::fstream::app);

	file << msg.c_str() << std::endl;
	file.close();
}