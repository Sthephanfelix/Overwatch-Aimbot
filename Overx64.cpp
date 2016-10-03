
// Overx64.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Overx64.h"
#include "HackProcess.h"
#include "Player.h"
#include "DirectX.h"
#include "Menu.h"
#include "VMProtectSDK.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

// COverx64App

BEGIN_MESSAGE_MAP(COverx64App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COverx64App 构造
 CHackProcess HackProcess;

COverx64App::COverx64App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
COverx64App theApp;

PCSTR   MyWindowName = " nenene";
HWND   MyHwnd;

PCSTR    GameWindowName = "守望先锋";
PCSTR    GamePorcessName = "Overwatch.exe";
HWND   GameHwnd;
RECT      GameRectSize;
PROCESSENTRY32 GamePorcess;
bool GameWindowMinimize=false;
bool GameWindowResize = false;
extern GameOffsets									_GameOffsets;
extern CHackProcess									HackProcess;
MyRectangle rectangle;

char zhuchengxu[64];
char tou[64];
char wei[64];
DWORD ID;
INT Read_INT(INT64 address	, DWORD 进程ID)
{
	HANDLE 内存操作句柄;
	BOOL A;
	INT ReadValue;
	if (进程ID == -1)
	{
		内存操作句柄 = GetCurrentProcess();
	}
	else
	{
		内存操作句柄 = OpenProcess(2035711, NULL, 进程ID);
	}
	A = ReadProcessMemory(内存操作句柄, (LPVOID)address, &ReadValue, 4, NULL);
	CloseHandle(内存操作句柄);
	if (A == FALSE) { return INT(-1); }
	else { return ReadValue; }
}
BOOL COverx64App::InitInstance()
{

	CWinApp::InitInstance();
	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

 			MyCreateWindow();

	return TRUE;
	
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
 		DirectXInit();
		DirectXRender();
		break;
	case WM_DESTROY:
		CleanupDX();
		PostQuitMessage(1);
		return 0;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
void COverx64App::MyCreateWindow()
{
	::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetGameWindowRect, 0, 0, 0);
		WNDCLASSEX wndClass;//EX窗口类
		wndClass.cbClsExtra = NULL;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.cbWndExtra = NULL;
		wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		wndClass.hCursor = ::LoadCursor(0, IDC_ARROW);
		wndClass.hIcon = ::LoadIcon(0, IDI_APPLICATION);
		wndClass.hIconSm = ::LoadIcon(0, IDI_APPLICATION);
		wndClass.hInstance = theApp.m_hInstance;
		wndClass.lpfnWndProc = WinProc;
		wndClass.lpszClassName = MyWindowName;
		wndClass.lpszMenuName = MyWindowName;
		wndClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wndClass))	exit(1);//判断创建成功 否->关闭

	GameHwnd = FindWindow(0, GameWindowName);
	if (GameHwnd)
	{
			GetWindowRect(GameHwnd, &GameRectSize);
			MyHwnd = CreateWindowEx(
			WS_EX_TOPMOST | 
			WS_EX_TRANSPARENT | 
			WS_EX_LAYERED|WS_EX_TOOLWINDOW,//任务栏不显示
			MyWindowName,
			MyWindowName,
			WS_POPUP,
			1, 1, 1920, 1080, 0, 0, 0, 0);
			SetLayeredWindowAttributes(MyHwnd, 0, 1, LWA_ALPHA);
			SetLayeredWindowAttributes(MyHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
			ShowWindow(MyHwnd, SW_SHOW);
	}
	MSG Message;
	ZeroMemory(&Message, sizeof(Message));
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		Sleep(1);
	}

}
extern GameDataWork				_GameData;

extern MyMenu							MyMenuA;
extern MyChack							Buttn_ESP;
extern MyChack							Buttn_Health;
extern MyChack							Buttn_Distance;
extern MyChack							Buttn_AimBot;
extern MyChack							Buttn_AimBotPre;
extern MyChack							Buttn_AimBotSpeed;
extern MyChack							Buttn_Team;
extern MyHealthButton				HealthStyle;
extern MyAimBotHeight				AimBotHeight;
extern MyAimBotKey					AimBotKey;
extern MyAimBotSpeed				Buttn_AimBotSpeedInt;
extern MyChangeButton				AimBotPre;
extern MyChangeButton				AimBotPreHealth;
extern MyChack							Buttn_AimBotPre;
extern MyChack							Buttn_AimBotPreHealth;

void  GetGameWindowRect()
{
	
	while (true)
	{
		
		GameHwnd = NULL;
		GameHwnd = FindWindow(0, GameWindowName);
		
		if (GameHwnd == 0)
			exit(0);


		if (GetKeyState(VK_INSERT) & 0x8000)
		{
			MyMenuA.MyMenuHideShow = !MyMenuA.MyMenuHideShow;
			Sleep(100);
		}
		if (MyMenuA.MyMenuHideShow)
		{
			MyMenuA.MyClickPoint(GetMousePoint());
			HealthStyle.MyButtonClick(GetMousePoint());
			AimBotHeight.MyButtonClick(GetMousePoint());
			AimBotKey.MyButtonClick(GetMousePoint());
			Buttn_AimBotSpeed.MyChackClick(GetMousePoint());
			Buttn_AimBotPre.MyChackClick(GetMousePoint());
			Buttn_AimBotPreHealth.MyChackClick(GetMousePoint());

			Buttn_ESP.MyChackClick(GetMousePoint());
			Buttn_Health.MyChackClick(GetMousePoint());
			Buttn_Distance.MyChackClick(GetMousePoint());
			Buttn_AimBot.MyChackClick(GetMousePoint());
			Buttn_AimBotPre.MyChackClick(GetMousePoint());
			Buttn_Team.MyChackClick(GetMousePoint());
			Buttn_AimBotSpeedInt.MyButtonClick(GetMousePoint());
			AimBotPreHealth.MyButtonClick(GetMousePoint());
			AimBotPre.MyButtonClick(GetMousePoint());
		}
	
		
		if (GameHwnd)
		{
			if (!_GameData.GameProcessID)
			{
				_GameData.GameProcessID = HackProcess.GetProcess((char*)GamePorcessName, &GamePorcess);
				_GameData.GameModuleOffset = HackProcess.GetProcessModuleOffset();
			}
	
			RECT targetSize =RECT();
			GetWindowRect(GameHwnd, &targetSize);//获取窗口大小
			if (targetSize.left <= 0 && targetSize.top <= 0 && targetSize.right <= 0 && targetSize.bottom <= 0)//判断窗口是否最小化
			{
				GameWindowMinimize = true;
				continue;
			}
			GameWindowMinimize = false;

			RECT borderSize = RECT();
			GetClientRect(GameHwnd, &borderSize);//获取窗口客户区的大小
			DWORD dwStyle = GetWindowLong(GameHwnd, GWL_STYLE);//获取窗口风格
			int windowheight;
			int windowwidth;
			int borderheight;
			int borderwidth;

			if (rectangle.Width != (targetSize.bottom - targetSize.top)&& rectangle.Width != (borderSize.right - borderSize.left))
				GameWindowResize = true;

			rectangle.Width = targetSize.right - targetSize.left;
			rectangle.Height = targetSize.bottom - targetSize.top;
			rectangle.Left = targetSize.left;
			rectangle.Top = targetSize.top;
			_GameData.rect = rectangle;
			if ((dwStyle & WS_BORDER) != 0)
			{
				windowheight = targetSize.bottom - targetSize.top;
				windowwidth = targetSize.right - targetSize.left;

				rectangle.Height = borderSize.bottom - borderSize.top;
				rectangle.Width = borderSize.right - borderSize.left;

				borderheight = (windowheight - borderSize.bottom);
				borderwidth = (windowwidth - borderSize.right) / 2; //only want one side
				borderheight -= borderwidth; //remove bottom

				targetSize.left += borderwidth;
				targetSize.top += borderheight;

				rectangle.Left = targetSize.left;
				rectangle.Top = targetSize.top;
				_GameData.rect = rectangle;
			}
			MoveWindow(MyHwnd, targetSize.left, targetSize.top, rectangle.Width, rectangle.Height, true);

		}
		Sleep(1);
	}
	
}