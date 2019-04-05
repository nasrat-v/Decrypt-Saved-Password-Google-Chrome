#pragma once

#include <iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <sstream>

#include "Log.hh"

# define UTIME_TO_WAIT      (DWORD)(_processesToStop.size() * 200)
# define SIZE_BUFFER_ERROR  sizeof(char)

class ManageProcess
{
public:
	ManageProcess();
	~ManageProcess();

#ifdef _UNICODE | UNICODE
	void		checkBrowserProcess(const wchar_t *processName);
#else // ANSI
	void		checkBrowserProcess(const char *processName);
#endif /* _UNICODE */

private:
	/* Attributes */
	typedef struct  s_processData
	{
		DWORD	id;
		HANDLE	handle;
		HWND	mainWindowHandle;
	}			t_processData;

	std::vector<t_processData> _processesToStop;

	/* Methods */

#ifdef _UNICODE | UNICODE
	void			findProcesses(const wchar_t *processName);
#else // ANSI
	void			findProcesses(const char *processName);
#endif /* _UNICODE */

	void			findProcessMainWindow();
	void			killProcesses();
	bool			killProcessByTerminate(t_processData &process);
	bool			killProcessByClosingMainWindow(t_processData &process);
	bool			waitForProcessToBeKilled(t_processData &process);
	static BOOL CALLBACK	findWindowsProcessCallback(HWND hwnd, LPARAM param);
	BOOL			findWindowsProcess(HWND hwnd);
	BOOL			compareWindowAndProcessId(HWND hwnd, DWORD processId);
	BOOL			isMainWindow(HWND hwnd);
	const std::string     	getLastErrorMessage();
};
