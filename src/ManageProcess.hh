#pragma once

#include <iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <sstream>

#include "Log.hh"

# define UTIME_TO_WAIT		  (DWORD)(_processesToStop.size() * 200)
# define SIZE_BUFFER_ERROR  sizeof(char)

class ManageProcess
{
public:
	ManageProcess();
	~ManageProcess();

	void				    checkBrowserProcess(const wchar_t *processName);

private:
	/* Attributes */
	typedef struct  s_processData
	{
		DWORD			    id;
		HANDLE			  handle;
		HWND			    mainWindowHandle;
	}					      t_processData;

	std::vector<t_processData>	_processesToStop;

	/* Methods */
	void					        findProcesses(const wchar_t *processName);
	void					        findProcessMainWindow();
	void					        killProcesses();
	bool					        killProcessByTerminate(t_processData &process);
	bool					        killProcessByClosingMainWindow(t_processData &process);
	bool					        waitForProcessToBeKilled(t_processData &process);
	static BOOL CALLBACK  findWindowsProcessCallback(HWND hwnd, LPARAM param);
	BOOL					        findWindowsProcess(HWND hwnd);
	BOOL					        compareWindowAndProcessId(HWND hwnd, DWORD processId);
	BOOL					        isMainWindow(HWND hwnd);
	const std::string     getLastErrorMessage();
};
