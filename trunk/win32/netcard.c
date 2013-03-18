DeviceName <-> AdapterName GetAdaptersAddresses() 
LOCAL_MACHINE_SYSTEM//CurrentControlSet//Control//Network//{4D36E972-E325-11CE-BFC1-08002BE10318}//AdapterName//Connection//Name
#include <windows.h>
#include <locale.h>
#define INITGUID
#include <Devpkey.h>
#undef INITGUID
#include <devguid.h>
#include <Setupapi.h>
#include <Cfgmgr32.h>

#pragma comment(lib,"Setupapi.lib")

VOID PrintDeviceName(HDEVINFO hDevices, PSP_DEVINFO_DATA pDevice);
VOID DisableDevice(HDEVINFO hDevices, PSP_DEVINFO_DATA pDevice, BOOL bStatus);

int main(int argc, char* argv[])
{
	HDEVINFO hDevices = NULL;

	setlocale(LC_ALL, "chs_china.936");

	do 
	{
		LONG i = -1;

		hDevices = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT);
		if (INVALID_HANDLE_VALUE == hDevices)
		{
			break;
		}
		
		while (TRUE)
		{
			BOOL bStatus = FALSE;

			DWORD status = 0;
			DWORD problem = 0;

			SP_DEVINFO_DATA device = {0};

			device.cbSize = sizeof(SP_DEVINFO_DATA);

			bStatus = SetupDiEnumDeviceInfo(hDevices, ++i, &device);
			if (!bStatus)
			{
				break;
			}

			PrintDeviceName(hDevices, &device);
			if (CM_Get_DevNode_Status(&status, &problem, device.DevInst,0) != CR_SUCCESS)   
			{
				wprintf(L"CM_Get_DevNode_Status Failure, Error = %d\n", GetLastError());
				continue;   
			}
			if (problem == CM_PROB_DISABLED)
			{
				DisableDevice(hDevices, &device, TRUE);
			}
			else
			{
				DisableDevice(hDevices, &device, FALSE);
			}
		}

	}
	while (FALSE);

	if (NULL != hDevices)
	{
		SetupDiDestroyDeviceInfoList(hDevices);
		hDevices = NULL;
	}

	return 0;
}

VOID PrintDeviceName(HDEVINFO hDevices, PSP_DEVINFO_DATA pDevice)
{
	PWCHAR pszName = NULL;

	if (NULL == hDevices || NULL == pDevice)
	{
		return;
	}

	do 
	{
		DWORD dwBytes = 0;

		BOOL bStatus = FALSE;

		DEVPROPTYPE nType = 0;

		bStatus = SetupDiGetDevicePropertyW(
			hDevices, pDevice, 
			&DEVPKEY_NAME, &nType, 
			NULL, 0, 
			&dwBytes, 0);

		if (!bStatus && ERROR_INSUFFICIENT_BUFFER != GetLastError())
		{
			DWORD dwError = GetLastError();
			break;
		}

		pszName = (PWCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytes + sizeof(WCHAR));
		if (NULL == pszName)
		{
			break;
		}

		bStatus = SetupDiGetDevicePropertyW(
			hDevices, pDevice, 
			&DEVPKEY_NAME, &nType, 
			(PBYTE)pszName, dwBytes, 
			&dwBytes, 0);

		if (!bStatus)
		{
			break;
		}
#if 0
		printf("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X\n",
		((GUID*)pszName)->Data1,
		((GUID*)pszName)->Data2,
		((GUID*)pszName)->Data3,
		((GUID*)pszName)->Data4[0],
		((GUID*)pszName)->Data4[1], 
		((GUID*)pszName)->Data4[2],
		((GUID*)pszName)->Data4[3],
		((GUID*)pszName)->Data4[4],
		((GUID*)pszName)->Data4[5],
		((GUID*)pszName)->Data4[6],
		((GUID*)pszName)->Data4[7]);
#else
		wprintf(L"%s\n", pszName);
#endif
	}
	while (FALSE);

	if (NULL != pszName)
	{
		HeapFree(GetProcessHeap(), 0, pszName);
		pszName = NULL;
	}
}

VOID DisableDevice(HDEVINFO hDevices, PSP_DEVINFO_DATA pDevice, BOOL bStatus)
{
	if (NULL == hDevices || NULL == pDevice)
	{
		return;
	}

	do 
	{
		SP_PROPCHANGE_PARAMS parameters = {0};
		PSP_CLASSINSTALL_HEADER pHeader = &parameters.ClassInstallHeader;

		pHeader->cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		pHeader->InstallFunction = DIF_PROPERTYCHANGE;

		parameters.Scope = DICS_FLAG_GLOBAL;
		parameters.StateChange = (bStatus ? DICS_ENABLE : DICS_DISABLE);

		bStatus = SetupDiSetClassInstallParams(hDevices, pDevice, (PSP_CLASSINSTALL_HEADER)&parameters, sizeof(SP_PROPCHANGE_PARAMS));
		if (!bStatus)
		{
			wprintf(L"SetupDiSetClassInstallParams Failure, Error = %d\n", GetLastError());
		}

		bStatus = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevices, pDevice);
		if (!bStatus)
		{
			wprintf(L"SetupDiCallClassInstaller Failure, Error = %d\n", GetLastError());
		}
	}
	while (FALSE);
}
