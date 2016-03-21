#include "stdafx.h"
#include "Dialer.h"
#define IDB_ONE     3301
#define IDB_TWO     3302
#define IDT_ONE     3402
#define IDT_TWO     3403
#define MAX_LOADSTRING 100

// 全局变量
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HWND NameBox = NULL;							//用户名输入框
HWND PassBox = NULL;							//密码输入框
HWND show;										//状态
HWND button1;									//按钮1
HWND button2;									//按钮2
LPTSTR UserName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 16);
LPTSTR Password = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 16);
LPTSTR LinkName = L"宽带连接";
// 此代码模块中包含的函数的前向声明: 
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

	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIALER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIALER));

	// 主消息循环: 
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
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 270, NULL, NULL, hInstance, NULL);
	//按钮
	button1 = CreateWindow(L"Button", L"登录", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		50, 100, 40, 20, hWnd, (HMENU)IDB_ONE, hInst, NULL);
	button2 = CreateWindow(L"Button", L"挂断", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		100, 100, 40, 20, hWnd, (HMENU)IDB_TWO, hInst, NULL);
	//输入框
	NameBox = CreateWindow(L"EDIT", L"", WS_VISIBLE | ES_CENTER | WS_CHILD,
		100, 30, 120, 15, hWnd, (HMENU)IDT_ONE, hInst, NULL);
	PassBox = CreateWindow(L"EDIT", L"", WS_VISIBLE | ES_CENTER | WS_CHILD | ES_PASSWORD,
		100, 60, 120, 15, hWnd, (HMENU)IDT_TWO, hInst, NULL);
	//标签
	  CreateWindow(L"Static", L"账号：", WS_VISIBLE | WS_CHILD,
		30, 30, 45, 20, hWnd, NULL, hInst, NULL);
	CreateWindow(L"Static", L"密码：", WS_VISIBLE | WS_CHILD,
		30, 60, 45, 20, hWnd, NULL, hInst, NULL);
	//显示进度
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
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择: 
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
				MessageBox(hWnd, L"密码为空", L"提示", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				EnableWindow(button1, FALSE);
				if (RsaCreateLink(LinkName, hWnd) == 0)
					RsaDial(LinkName, UserName, Password, hWnd);
				//hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &threadID); // 创建线程
				//WaitForSingleObject(hThread, INFINITE);
				//CloseHandle(hThread); // 关闭内核对象
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
		// TODO:  在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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