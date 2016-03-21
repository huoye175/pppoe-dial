#include "stdafx.h"
#include "Dialer.h"
#define IDB_ONE     3301
#define IDB_TWO     3302
#define IDT_ONE     3402
#define IDT_TWO     3403
#define MAX_LOADSTRING 100

// ȫ�ֱ���
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HWND NameBox = NULL;							//�û��������
HWND PassBox = NULL;							//���������
HWND show;										//״̬
HWND button1;									//��ť1
HWND button2;									//��ť2
LPTSTR UserName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 16);
LPTSTR Password = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 16);
LPTSTR LinkName = L"�������";
// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIALER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIALER));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIALER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DIALER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 270, NULL, NULL, hInstance, NULL);
	//��ť
	button1 = CreateWindow(L"Button", L"��¼", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		50, 100, 40, 20, hWnd, (HMENU)IDB_ONE, hInst, NULL);
	button2 = CreateWindow(L"Button", L"�Ҷ�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		100, 100, 40, 20, hWnd, (HMENU)IDB_TWO, hInst, NULL);
	//�����
	NameBox = CreateWindow(L"EDIT", L"", WS_VISIBLE | ES_CENTER | WS_CHILD,
		100, 30, 120, 15, hWnd, (HMENU)IDT_ONE, hInst, NULL);
	PassBox = CreateWindow(L"EDIT", L"", WS_VISIBLE | ES_CENTER | WS_CHILD | ES_PASSWORD,
		100, 60, 120, 15, hWnd, (HMENU)IDT_TWO, hInst, NULL);
	//��ǩ
	  CreateWindow(L"Static", L"�˺ţ�", WS_VISIBLE | WS_CHILD,
		30, 30, 45, 20, hWnd, NULL, hInst, NULL);
	CreateWindow(L"Static", L"���룺", WS_VISIBLE | WS_CHILD,
		30, 60, 45, 20, hWnd, NULL, hInst, NULL);
	//��ʾ����
	show = CreateWindow(L"Static", L"", WS_VISIBLE | WS_CHILD,
		40, 130, 200, 80, hWnd, NULL, hInst, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
DWORD threadID;
HANDLE hThread;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	PAINTSTRUCT ps;
	HDC hdc;
	LPCTSTR msg = L"Hello,welcome to windows!";
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_ONE:
		{
			i = GetWindowText(NameBox, UserName, 16);
			GetWindowText(PassBox, Password, 16);
			if (i == 0)
			{
				MessageBox(hWnd, L"����Ϊ��", L"��ʾ", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				EnableWindow(button1, FALSE);
				if (RsaCreateLink(LinkName, hWnd) == 0)
					RsaDial(LinkName, UserName, Password, hWnd);
				//hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &threadID); // �����߳�
				//WaitForSingleObject(hThread, INFINITE);
				//CloseHandle(hThread); // �ر��ں˶���
				//ConnectNotify(hWnd);
			}
		}
		break;
		case IDB_TWO:
		{
			hungup(LinkName);
			EnableWindow(button1, true);
		}
			
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}