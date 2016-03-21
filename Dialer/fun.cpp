#include "stdafx.h"
#pragma comment(lib,"rasapi32.lib ")
HRASCONN ghRasConn = NULL;
extern HWND button1;
extern  HWND show;
extern  HWND button1;
extern  HWND button2;
int RsaCreateLink(LPCTSTR szEntry, HWND hDlgWnd)
{
	ghRasConn = NULL;
	LPRASENTRY lpRasEntry = NULL;
	DWORD dwBufferSize = 0;
	DWORD dwRet = 0;
	DWORD rgep_res;
	DWORD dwDevInfo = 0;
	rgep_res = RasGetEntryProperties(NULL, L"", NULL, &dwBufferSize, NULL, &dwDevInfo);
	if (dwBufferSize == 0)
		return -1;
	lpRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
	if (lpRasEntry == NULL)
		return -2;
	ZeroMemory(lpRasEntry, sizeof(RASENTRY));
	lpRasEntry->dwSize = dwBufferSize;
	lpRasEntry->dwfOptions = RASEO_RemoteDefaultGateway;
	lpRasEntry->dwType = RASET_Broadband;

	lstrcpy(lpRasEntry->szDeviceType, RASDT_PPPoE);
	lstrcpy(lpRasEntry->szDeviceName, NULL);
	lpRasEntry->dwfNetProtocols = RASNP_Ip;
	lpRasEntry->dwFramingProtocol = RASFP_Ppp;

	dwRet = RasSetEntryProperties(NULL, szEntry, lpRasEntry, dwBufferSize, NULL, dwDevInfo); // 创建连接
	HeapFree(GetProcessHeap(), 0, (LPVOID)lpRasEntry);
	if (dwRet == ERROR_CANNOT_OPEN_PROFILE)
	{
		MessageBox(hDlgWnd,
			L"电话簿已经被破坏或丢失成分",
			L"错误",
			MB_OK);
	}
	return 0;
}

int RsaDial(LPCTSTR szEntry, LPCTSTR szUserName, LPCTSTR szPassword, HWND hDlgWnd)
{
	UINT i = 0;
	DWORD dwRet = 0;
	RASDIALPARAMS rdParams;
	
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));

	rdParams.dwSize = sizeof(RASDIALPARAMS);
	lstrcpy(rdParams.szEntryName, szEntry);
	lstrcpy(rdParams.szUserName, szUserName);
	lstrcpy(rdParams.szPassword, szPassword);
	ghRasConn = NULL;
	dwRet = RasDial(NULL, NULL, &rdParams, 1, &RasDiafunc1, &ghRasConn);
	TCHAR str[20];
	wsprintf(str, L"%u", dwRet);
	lstrcat(str, L"错误!");
	/*if (dwRet != 0)
	{
		MessageBox(hDlgWnd,
			str,
			L"错误",
			MB_OK);
	}*/
	return 0;
}
int hungup(LPCTSTR szEntry)
{
	if (ghRasConn != NULL)
	{
		RasHangUp(ghRasConn);
		Sleep(3000);
		ghRasConn = NULL;
	}
	RasDeleteEntry(NULL, szEntry);
	return 0;
}
BOOL ConnectNotify(HWND hDlgWnd)
{
	HANDLE connEvent = NULL;
	connEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	RasConnectionNotification(ghRasConn, connEvent, RASCN_Disconnection);
		if (WaitForSingleObject(connEvent, INFINITE) == WAIT_OBJECT_0)
			MessageBox(hDlgWnd,
			L"连接失败",
			L"错误",
			MB_OK);
		
	
	return FALSE;
}
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	
	ConnectNotify(NULL);
	return 0;
}

void WINAPI RasDiafunc(UINT unMsg, RASCONNSTATE rascs, DWORD dwError)
{
	TCHAR szRasString[256];
	TCHAR str[300];
	if (dwError)
	{
		RasGetErrorString((UINT)dwError, szRasString, 256);
		wsprintf(str, L"%u", dwError);
		lstrcat(str, szRasString);
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)str);
		EnableWindow(button2, true);
		return;
	}
}
void WINAPI RasDiafunc1(HRASCONN hrasconn, UINT unMsg, RASCONNSTATE rascs, DWORD dwError, DWORD dwExtendError)
{
	TCHAR szRasString[256];
	TCHAR str[300];
	if (dwError)
	{
		RasGetErrorString((UINT)dwError, szRasString, 256);
		wsprintf(str, L"%u", dwError);
		lstrcat(str,szRasString);
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)str);
		EnableWindow(button2, true);
		return;
	}
	switch (rascs)
	{
	case RASCS_OpenPort:
		break;
	case RASCS_PortOpened:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"正在打开端口");
		break;
	case RASCS_ConnectDevice:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"ConnectDevice。。");
		break;
	case RASCS_DeviceConnected:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"DeviceConnected。。");
		break;
	case RASCS_AllDevicesConnected:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"AllDevicesConnected。。");
		break;
	case RASCS_Authenticate:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"Authenticate。。");
		break;
	case RASCS_AuthNotify:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"AuthNotify。。");
		break;
	case RASCS_AuthRetry:
		break;
	case RASCS_AuthCallback:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"AuthCallback。。");
		break;
	case RASCS_AuthChangePassword:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"AuthChangePassword。。");
		break;
	case RASCS_AuthProject:
		break;
	case RASCS_AuthLinkSpeed:
		break;
	case RASCS_AuthAck:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"AuthAck。。");
		break;
	case RASCS_ReAuthenticate:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"ReAuthenticate。。");
		break;
	case RASCS_Authenticated:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"Authenticated。。");
		break;
	case RASCS_PrepareForCallback:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"PrepareForCallback。。");
		break;
	case RASCS_WaitForModemReset:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"WaitForModemReset。。");
		break;
	case RASCS_WaitForCallback:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"WaitForCallback。。");
		break;
	case RASCS_Projected:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"Projected。。");
		break;
	case RASCS_StartAuthentication:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"StartAuthentication。。");
		break;
	case RASCS_CallbackComplete:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"正在验证用户名密码。。");
		break;
	case RASCS_LogonNetwork:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"正在登录。。");
		break;
	case RASCS_SubEntryConnected:
		break;
	case RASCS_SubEntryDisconnected:
		break;
	case RASCS_ApplySettings:
		break;
	case RASCS_Interactive:
		break;
	case RASCS_RetryAuthentication:
		break;
	case RASCS_CallbackSetByCaller:
		break;
	case RASCS_PasswordExpired:
		break;
	case RASCS_InvokeEapUI:
		break;
	case RASCS_Connected:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"连接成功。。");
		break;
	case RASCS_Disconnected:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"连接断开。。");
		break;
	default:
		SendMessage(show, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"正在拨号。。");
		break;
	}
}