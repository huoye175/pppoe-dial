#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�
#include <windows.h>

// C ����ʱͷ�ļ�
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

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
