
#include "ManageProcess.hh"

ManageProcess::ManageProcess() = default;

ManageProcess::~ManageProcess() = default;

#ifdef _UNICODE
void		ManageProcess::checkBrowserProcess(const wchar_t *processName)
#elif UNICODE
void		ManageProcess::checkBrowserProcess(const wchar_t *processName)
#else // ANSI
void		ManageProcess::checkBrowserProcess(const char *processName)
#endif /* _UNICODE */
{
	_processesToStop.clear();
	findProcesses(processName); // find all browser processus
	if (_processesToStop.size() > 0)
	{
		Log::logInfoMsg(std::to_string(_processesToStop.size()) + " process found");
		findProcessMainWindow(); // find window process handle to close process properly
		killProcesses();
	}
	else
		Log::logInfoMsg("No process found");
}

#ifdef _UNCODE
void			 ManageProcess::findProcesses(const wchar_t *processName)
#elif UNICODE
void			 ManageProcess::findProcesses(const wchar_t *processName)
#else // ANSI
void			 ManageProcess::findProcesses(const char *processName)
#endif /* _UNICODE */
{
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 ProcessEntry = { 0 };
	t_processData processData;

	ProcessEntry.dwSize = sizeof(ProcessEntry);
	Process32First(hProcessSnapShot, &ProcessEntry);
	while (Process32Next(hProcessSnapShot, &ProcessEntry))
	{
		if (_tcsicmp(ProcessEntry.szExeFile, processName) == 0)
		{
			processData.id = ProcessEntry.th32ProcessID;
			processData.handle = OpenProcess((SYNCHRONIZE | PROCESS_TERMINATE), FALSE, ProcessEntry.th32ProcessID);
			processData.mainWindowHandle = 0;
			_processesToStop.push_back(processData);
		}
	}
	CloseHandle(hProcessSnapShot);
}

void ManageProcess::findProcessMainWindow()
{
	EnumWindows(findWindowsProcessCallback, reinterpret_cast<LPARAM>(this));
}

BOOL CALLBACK ManageProcess::findWindowsProcessCallback(HWND hwnd, LPARAM param)
{
	return (reinterpret_cast<ManageProcess*>(param)->findWindowsProcess(hwnd));
}

BOOL ManageProcess::findWindowsProcess(HWND hwnd)
{
	for (t_processData &processData : _processesToStop)
	{
		if (!compareWindowAndProcessId(hwnd, processData.id))
		{
			processData.mainWindowHandle = hwnd;
			return (FALSE);
		}
	}
	return (TRUE);
}

BOOL ManageProcess::compareWindowAndProcessId(HWND hwnd, DWORD processId)
{
	DWORD windowProcessId;

	GetWindowThreadProcessId(hwnd, &windowProcessId);
	if (processId != windowProcessId || !isMainWindow(hwnd))
		return (TRUE);
	return (FALSE);
}

BOOL ManageProcess::isMainWindow(HWND hwnd)
{
	return (GetWindow(hwnd, GW_OWNER) == ((HWND)0) && IsWindowVisible(hwnd));
}

void ManageProcess::killProcesses()
{
	for (t_processData &process : _processesToStop)
	{
		if (!killProcessByClosingMainWindow(process)) // try to close the window first
			killProcessByTerminate(process); // if it doesn't work kill it in the dirty way
		CloseHandle(process.handle);
	}
}

bool ManageProcess::killProcessByClosingMainWindow(t_processData &process)
{
	Log::logTryMsg("Kill process by closing window");
	if (!PostMessageA(process.mainWindowHandle, WM_CLOSE, NULL, NULL))
	{
		Log::logFailureMsg("Error cannot post message WM_CLOSE on window: " + getLastErrorMessage());
		return (false);
	}
	return (waitForProcessToBeKilled(process));
}

bool ManageProcess::killProcessByTerminate(t_processData &process)
{
	Log::logTryMsg("Kill process by terminate");

	TerminateProcess(process.handle, 0);
	return (waitForProcessToBeKilled(process));
}

bool ManageProcess::waitForProcessToBeKilled(t_processData &process)
{
	DWORD status;

	if ((status = WaitForSingleObject(process.handle, UTIME_TO_WAIT)) == WAIT_OBJECT_0)
	{
		Log::logSuccessMsg("Process successfully killed");
		return (true);
	}
	else if (status == WAIT_TIMEOUT)
		Log::logFailureMsg("Timeout during wait for process kill");
	else
		Log::logFailureMsg("Error during wait for process kill: " + getLastErrorMessage());
	return (false);
}

const std::string ManageProcess::getLastErrorMessage()
{
	char error[(SIZE_BUFFER_ERROR + 1)];

	memset(error, 0, (SIZE_BUFFER_ERROR + 1));
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error, SIZE_BUFFER_ERROR, NULL);
	return (std::string(error));
}
