#include <windows.h>
#include <Winnetwk.h>

#pragma comment(lib, "Mpr.lib")

#define ERROR_BUFF_SIZE	1024

DWORD unmapdisk(const char* disk)
{
	return WNetCancelConnection(disk, TRUE);
}

DWORD mapdisk(const char* remote, const char* user, const char* password, const char* disk)
{
	NETRESOURCE nr;
	nr.dwType = RESOURCETYPE_ANY;
	nr.lpLocalName = (char*)disk;
	nr.lpRemoteName = (char*)remote;
	nr.lpProvider = NULL;

	unmapdisk(disk);

	printf("%s\n", nr.lpLocalName);
	return WNetAddConnection2(&nr, password, user, 0);
}

void formatmapdiskerror(DWORD err)
{
	DWORD dwWnetError;
	char* szErrorBuf[ERROR_BUFF_SIZE] = {0};
	char* nameBuf[ERROR_BUFF_SIZE] = {0};
	
	switch(err)
	{
	case ERROR_ACCESS_DENIED:
		printf("The caller does not have access to the network resource.\n");
		break;
	case ERROR_ALREADY_ASSIGNED:
		printf("The local device specified by the lpLocalName member is already connected to a network resource.\n");
		break; 
	case ERROR_BAD_DEV_TYPE:
		printf("The type of local device and the type of network resource do not match.\n");
		break;
	case ERROR_BAD_DEVICE:
		printf("The value specified by lpLocalName is invalid.\n");
		break;
	case ERROR_BAD_NET_NAME:
		printf("The value specified by the lpRemoteName member is not acceptable to any network resource provider, either because the resource name is invalid, or because the named resource cannot be located.\n");
		break;
	case ERROR_BAD_PROFILE:
		printf("The user profile is in an incorrect format.\n");
		break;
	case ERROR_BAD_PROVIDER:
		printf("The value specified by the lpProvider member does not match any provider.\n");
		break;
	case ERROR_BUSY:
		printf("The router or provider is busy, possibly initializing. The caller should retry.\n");
		break;
	case ERROR_CANCELLED:
		printf("The attempt to make the connection was canceled by the user through a dialog box from one of the network resource providers, or by a called resource.\n");
		break;
	case ERROR_CANNOT_OPEN_PROFILE:
		printf("The system is unable to open the user profile to process persistent connections.\n");
		break;
	case ERROR_DEVICE_ALREADY_REMEMBERED:
		printf("An entry for the device specified by lpLocalName is already in the user profile.\n");
		break;
	case ERROR_EXTENDED_ERROR:
		printf("A network-specific error occurred.\n");
		WNetGetLastError(&dwWnetError, (LPSTR)szErrorBuf, ERROR_BUFF_SIZE, (LPSTR)nameBuf, ERROR_BUFF_SIZE);
		printf("%s %s\n", szErrorBuf, nameBuf);
		break;
	case ERROR_INVALID_PASSWORD:
		printf("The specified password is invalid and the CONNECT_INTERACTIVE flag is not set.\n");
		break;
	case ERROR_NO_NET_OR_BAD_PATH:
		printf("The operation cannot be performed because a network component is not started or because a specified name cannot be used.\n");
		break;
	case ERROR_NO_NETWORK:
		printf("The network is unavailable.\n");
		break;
	default:
		printf("unknow error.\n");
	}
}

int main(int argc, char* argv[])
{
	DWORD ret;
	if (argc < 5)
	{
		printf("usage mapdisk address user password disk\n");
		return -1;
	}
	ret = mapdisk(argv[1], argv[2], argv[3], argv[4]);

	if (NO_ERROR != ret)
	{
		formatmapdiskerror(ret);
	}

	return 0;
}
