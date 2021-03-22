#include <Windows.h>
#include <iostream>

#include "memory.hpp"
#include "ifexist.hpp"

using namespace std;

#define JUNKS \
__asm _emit 0x00 \
__asm _emit 0x01 \
__asm _emit 0x03 \
__asm _emit 0x04 \
__asm _emit 0x05 \
__asm _emit 0x06 \
__asm _emit 0x07 \
__asm _emit 0x08 \
__asm _emit 0x09 \
__asm _emit 0x95 \
__asm _emit 0x55 \
__asm _emit 0x65 \
__asm _emit 0x85 \
__asm _emit 0x95 \
__asm _emit 0x06 \
__asm _emit 0x02 \
__asm _emit 0x25 \
__asm _emit 0x09 \
__asm _emit 0x21 \
__asm _emit 0x35 \
__asm _emit 0x26 \
__asm _emit 0x32 \
__asm _emit 0x69 \
__asm _emit 0x97 \
__asm _emit 0x99 \
__asm _emit 0x14 \
__asm _emit 0x28 \


// Don't change this!
#define _JUNK_BLOCK(s) __asm jmp s JUNKS __asm s:


/////////////////////////////////////////////////////////////////////////////////////
//                                 INJECTOR CODE                                   //
/////////////////////////////////////////////////////////////////////////////////////

DWORD Process(char* ProcessName)
{
	_JUNK_BLOCK(jmp_label1)
		HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	_JUNK_BLOCK(jmp_label2)
		PROCESSENTRY32 ProcEntry;

	_JUNK_BLOCK(jmp_label3)
		ProcEntry.dwSize = sizeof(ProcEntry);

	_JUNK_BLOCK(jmp_label4)
		do
		{
			_JUNK_BLOCK(jmp_label5)

			{
				_JUNK_BLOCK(jmp_label6)
					DWORD dwPID = ProcEntry.th32ProcessID;

				_JUNK_BLOCK(jmp_label7)
					CloseHandle(hPID);

				_JUNK_BLOCK(jmp_label8)
					return dwPID;
			}

			_JUNK_BLOCK(jmp_label9)
		} while (Process32Next(hPID, &ProcEntry));

			_JUNK_BLOCK(jmp_label10)
}
Injector inj;

DWORD pid;

//now it's global
LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");

void bypass()
{
	// Restore original NtOpenFile from external process
	//credits: PadPlayerPL(pozdro dla ciebie byczku <3)
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL);
	}
	else
	{
		cout << "Unable to bypass :(\n";
		Sleep(2000);
		exit(-1);
	}
}

void Backup()
{
	if (ntOpenFile) {
		//So, when I patching first 5 bytes I need to backup them to 0? (I think)
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 0, NULL);
	}
	else
	{
		cout << "Unable to backup :(\n";
		Sleep(2000);
		exit(-1);
	}
}

int main()
{
	SetConsoleTitle("TFJector");
	
	cout << "Kreditsy:\n PadplayerPL\n\n" << endl;

	inj.hwndproc = FindWindowA(0, "hl2");


	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	inj.clientDLL = inj.GetModule(pid, "client.dll");

	if (DoesFileExist("czittf2.dll")) {
		bypass();

		if (inj.inject(pid, "czittf2.dll")) {
			cout << "Zainjestowano pomyślnie!\n\n" << endl;
			Backup();
			Sleep(2000);
			exit(0);
		}
		else
		{
			cout << "Injekcja nieudana!\n\n" << endl;
			Backup();
			Sleep(2000);
			exit(-1);
		}
	
	}
	else
	{
		cout << "nie mozna znalezc czittf2.dll\n\n";
		Sleep(2000);
		exit(-1);
	}

	return 0;
}