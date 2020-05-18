#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <initguid.h>
#include <virtdisk.h>
#include "abRctDemo.h"
using namespace std;

int main(void)
{
	abRctDemo* rct = new abRctDemo();

	rct->connectWmiNamespace();
	//step 1: create snapshot
	LPCWSTR vmName = L"linux-8g";
	rct->createSnapshot(vmName);
	cout << "Create snapshot compelete." << endl << endl;

	//step 2: convert checkpoint
	rct->convertCheckpoint();
	cout << "Checkpoint conversion complete." << endl;

	//step 3: retrieve change blocks
	PCWSTR virtualDiskPath = L"D:\\Hyper-V\\linux-8g����\\Virtual Hard Disks\\linux-8g����.vhdx";
	rct->retrieveChangeBlocks(virtualDiskPath);
	cout << "Retrieve change blocks complete." << endl;

	//step 4: delete reference checkpoint

	delete rct;
	rct = NULL;

	getchar();
	return 0;
}