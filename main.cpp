#include <iostream>
#include <Windows.h>
#include <vector>
#include "proc.h"


int main() {

	//Get ProcID of target process
	DWORD procId = getProcId(L"ac_client.exe");


	//Get Module Base Address
	uintptr_t moduleBase = getModuleBaseAddress(procId, L"ac_client.exe");


	//Get Handle to Process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);


	//Resolve base address of Pointer Chain
	uintptr_t dynamicPointerBaseAddress = moduleBase + 0x10f4f4;

	std::cout << "Dynamic Pointer Base Address: 0x" << std::hex << dynamicPointerBaseAddress << std::endl;



	//Resolve ammo ptr chain
	std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0};
	uintptr_t ammoAddress = findDMAaddy(hProcess, dynamicPointerBaseAddress, ammoOffsets);

	std::cout << "Ammo Address: 0x" << std::hex << ammoAddress << std::endl;



	//Read Ammo value
	int ammoValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "Current ammo equals: " << std::dec << ammoValue << std::endl;



	//Write to it
	int newAmmo = 1337;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddress, &newAmmo, sizeof(newAmmo), nullptr);



	//Read it again to confirm it works
	ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "New ammo is: " << std::dec << ammoValue << std::endl;




	std::cin.get();
	return 0;
}