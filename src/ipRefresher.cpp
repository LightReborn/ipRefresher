/*
ipRefresher.cpp
Author: Michael R. Roark
ipRefresher.exe sits and checks the connection to the internet using a reliable address(google). 
If connection is not found to be present, it executes ipconfig/release then /renew
*/

//Includes kept in stdafx.h
#include "stdafx.h"

//#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"Wininet.lib")

int system_no_output(
	std::string command
)
{
	command.insert(0, "/C");

	SHELLEXECUTEINFOA ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "cmd.exe";
	ShExecInfo.lpParameters = command.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	if( ShellExecuteExA( &ShExecInfo) == false)
	{
		return -1;
	}

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	DWORD rv;
	GetExitCodeProcess(ShExecInfo.hProcess, &rv);
	CloseHandle(ShExecInfo.hProcess);

	return rv;
}

void update_time(
	time_t start, 
	time_t current, 
	int& hours, 
	int& minutes, 
	int& seconds
)
{
	current = time(0);
	hours = (current - start) / 3600;
	minutes = ((current - start) / 60) % 60;
	seconds = (current - start) % 60;
}

void refresh()
{
	system_no_output("ipconfig/release");
	system_no_output("ipconfig/renew");
	return;
}

int _tmain(
	int argc, 
	char** argv[]
)
{
	time_t start = time(0);
	time_t current = time(0);
	int seconds=0, minutes=0, hours=0;
	LPCWSTR url = TEXT("http://www.google.com/");
	BOOL bConnect;

	while(true){
		//test connection
		bConnect = InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, 0);
		if (bConnect)
		{
			//internet connection DOES exists
			update_time(start, current, hours, minutes, seconds);
			system("cls");
			printf("Connection found!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t%02d:%02d:%02d",hours, minutes, seconds);
		}
		else
		{
			//internet DOES NOT connection exists
			update_time(start, current, hours, minutes, seconds);
			system("cls");
			printf("Connection not found!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t%02d:%02d:%02d",hours, minutes, seconds);
			refresh();
		}
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
	system("pause");
	return 0;
}
