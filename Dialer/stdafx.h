#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

#include <ras.h>
wchar_t* AnsiToUnicode(const char* szStr);
inline char* UnicodeToAnsi(const wchar_t* szStr);
int RsaCreateLink(LPCTSTR szEntry, HWND hDlgWnd);
int RsaDial(LPCTSTR szEntry, LPCTSTR szUserName, LPCTSTR szPassword, HWND hDlgWnd);
int hungup(LPCTSTR szEntry);
void WINAPI RasDiafunc1(HRASCONN hrasconn, UINT unMsg, RASCONNSTATE rascs, DWORD dwError, DWORD dwExtendError);
BOOL ConnectNotify(HWND hDlgWnd);
DWORD WINAPI ThreadProc(LPVOID lpParam);
void WINAPI RasDiafunc(UINT unMsg, RASCONNSTATE rascs, DWORD dwError);

// TODO:  在此处引用程序需要的其他头文件
