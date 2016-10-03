#include "stdafx.h"
#include "DirectX.h"
#include "Overx64.h"
#include "Player.h"
#include "Menu.h"
#include "Draw.h"

#pragma  comment(lib,"d2d1.lib")
 #pragma  comment(lib,"Dwrite.lib")
 #pragma comment(lib, "dwmapi.lib")
#include "Comply.h"
extern HWND						MyHwnd;
RECT									MyRect;




ID2D1Factory*						pFactory = NULL; // Direct2D factory
ID2D1HwndRenderTarget* pRender = NULL; // Render target
ID2D1SolidColorBrush*		pBrush = NULL; // A black brush, reflect the line color

IDWriteFactory*					pWriteFactory = NULL;
IDWriteTextFormat*			pTextFormatMsyh = NULL;
//IDWriteTextFormat*			pTextFormatMsyh = NULL;

POINT MyMenuPos = {20,200};



void CleanupDX()//清除DX
{
	// 	SAFE_RELEASE(p_Font);
	// 	SAFE_RELEASE(p_Device);
}
bool OnResize()
{
	if (MyHwnd)
	{
		MARGINS Margin = { 0, 0, MyRect.right - MyRect.left, MyRect.bottom - MyRect.top };
		DwmExtendFrameIntoClientArea(MyHwnd, &Margin);
		return true;
	}
	return false;
}
void DirectXInit()
{
	OnResize();
	
	if (!pRender)
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		GetWindowRect(MyHwnd, &MyRect);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, 
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			D2D1::HwndRenderTargetProperties(MyHwnd, 
			D2D1::SizeU(MyRect.right - MyRect.left, MyRect.bottom - MyRect.top)),	&pRender);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));
		pWriteFactory->CreateTextFormat(L"Msyh",
			NULL,
			DWRITE_FONT_WEIGHT_SEMI_BOLD,    // 粗细
			DWRITE_FONT_STYLE_NORMAL,      // 斜体
			DWRITE_FONT_STRETCH_NORMAL,    // Stretch
			18.0f,                         // Size    
			L"MyFont",                      // Local
			&pTextFormatMsyh);

	}
	SolidColorBrushInit();
	TextFormatInit();


}
extern bool						GameWindowResize;
extern bool						GameWindowMinimize;
extern GameOffsets		_GameOffsets;
extern GameDataWork	_GameData;
extern MyRectangle		rectangle;
extern Player					*LordPlayer;
bool									isAimbot;
Player								AimPlayer;
float									AimFov;




extern MyChack					Buttn_AimBot;
extern MyAimBotHeight		AimBotHeight;
extern MyAimBotSpeed		Buttn_AimBotSpeedInt;
extern MyChack					Buttn_AimBotSpeed;
extern MyAimBotKey			AimBotKey;
extern MyChack					Buttn_Team;



extern MyChangeButton		AimBotPre;
extern MyChangeButton		AimBotPreHealth;
extern MyChack					Buttn_AimBotPre;
extern MyChack					Buttn_AimBotPreHealth;
bool GetAimKey()
{
	return (GetAsyncKeyState(AimBotKey.MyHeightInt));
}
void GetClosestPlayerToCrossHair(Player player, float aimfov, float &max)
{
	if (!GetAimKey() || !isAimbot)
	{
		D3DXVECTOR3 w2sHead;
		if (player.Health<= 0)
			return;

// 		if (player.Team == 0)
// 		{
// 
// 			if (Buttn_Team.MyChackSelect == false)
// 					{
// 						return;
// 					}
// 		
// 				}
// 				else
// 				{
// 					
// 					if (Buttn_Team.MyChackSelect == true)
// 					{
// 						return;
// 					}
// 				}
		_GameData.WorldToScreen(player.Bones.Bone_Head, w2sHead);
		D3DXVECTOR3 ScreenCenter = { float(rectangle.Width/2),float(rectangle.Height/2),0};
		float Dist = _GameData.CalcVector2Distance(w2sHead, ScreenCenter);
		if (Dist < max)
		{
			AimPlayer = player;
			max = Dist;
			AimFov = aimfov;
		}

	}
	else
	{
		AimPlayer.PositionAddress = HackProcess.Read<INT64>(AimPlayer.PlayerAddress + _GameOffsets.EnemyPointer[8]);
		AimPlayer.PreAddress = HackProcess.Read<INT64>(AimPlayer.PositionAddress + _GameOffsets.EnemyPointer[10]);
		AimPlayer.PositionAddress = HackProcess.Read<INT64>(AimPlayer.PositionAddress);
		AimPlayer.Bones.Bone_Head = HackProcess.Read<D3DXVECTOR3>(AimPlayer.PositionAddress + _GameOffsets.Game_HeadPositionPointer);
		AimPlayer.Distance = _GameData.CalcVector3Distance(_GameData.GameViewOrigin, AimPlayer.Bones.Bone_Head);
		
		if (Buttn_AimBotPre.MyChackSelect)
		{
			AimPlayer.Pre = HackProcess.Read<D3DXVECTOR3>(AimPlayer.PreAddress + _GameOffsets.EnemyPointer[11])*(AimPlayer.Distance*(0.001+ AimBotPre.MyHeightInt*0.001));
			AimPlayer.Bones.Bone_Head = AimPlayer.Bones.Bone_Head + AimPlayer.Pre;
		}
		else
		{
			AimPlayer.Bones.Bone_Head = AimPlayer.Bones.Bone_Head;
		}
		if (Buttn_AimBotPreHealth.MyChackSelect)
		{
			AimPlayer.Bones.Bone_Head.y += (AimBotHeight.MyHeightInt*0.02)+(AimPlayer.Distance*(0.001 + AimBotPreHealth.MyHeightInt*0.001));
		}
		else
		{
			AimPlayer.Bones.Bone_Head.y += (AimBotHeight.MyHeightInt*0.02);
		}
		




		D3DXVECTOR3 Bone_Head = AimPlayer.Bones.Bone_Head;
		if (_GameData.WorldToScreen(Bone_Head, Bone_Head, AimPlayer.ScreenBones.Bone_Head));
		DrawLine(AimPlayer.ScreenBones.Bone_Head.x - 6,  AimPlayer.ScreenBones.Bone_Head.y , AimPlayer.ScreenBones.Bone_Head.x + 6,  AimPlayer.ScreenBones.Bone_Head.y , D2D1::ColorF::Magenta);
		DrawLine(AimPlayer.ScreenBones.Bone_Head.x , AimPlayer.ScreenBones.Bone_Head.y-6 , AimPlayer.ScreenBones.Bone_Head.x , AimPlayer.ScreenBones.Bone_Head.y + 6, D2D1::ColorF::Magenta);
		//DrawFillRectangle(AimPlayer.ScreenBones.Bone_Head.x - 1.5, AimPlayer.ScreenBones.Bone_Head.y - 1.5, 3, 3, D2D1::ColorF::Magenta);
	}
}
void MAimbot(float x, float y)
{
	float AimSpeed = (float)(Buttn_AimBotSpeedInt.MyHeightInt*0.5) + 2;
	float TargetX = 0;
	float TargetY = 0;
	if (x != 0)
	{
		if (x > (rectangle.Width / 2))
		{
			TargetX = -((rectangle.Width / 2) - x);
			TargetX /= AimSpeed;
			if (TargetX + (rectangle.Width / 2) + 15 > (rectangle.Width / 2) * 2) TargetX = 0;
		}
		if (x < (rectangle.Width / 2))
		{
			TargetX = x - (rectangle.Width / 2);
			TargetX /= AimSpeed;
			if (TargetX + (rectangle.Width / 2) - 15 < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > (rectangle.Height / 2))
		{
			TargetY = -((rectangle.Height / 2) - y);
			TargetY /= AimSpeed;
			if (TargetY + (rectangle.Height / 2) + 15 > (rectangle.Height / 2) * 2) TargetY = 0;
		}

		if (y < (rectangle.Height / 2))
		{
			TargetY = y - (rectangle.Height / 2);
			TargetY /= AimSpeed;
			if (TargetY + (rectangle.Height / 2) - 15 < 0) TargetY = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
	return;

}

void GameAimInit(Player player)
{
	D3DXVECTOR3 AimBotBody;
	player.PositionAddress = HackProcess.Read<INT64>(player.PlayerAddress + _GameOffsets.EnemyPointer[8]);
	player.PreAddress = HackProcess.Read<INT64>(player.PositionAddress + _GameOffsets.EnemyPointer[10]);
	player.PositionAddress = HackProcess.Read<INT64>(player.PositionAddress );
	player.Bones.Bone_Head=HackProcess.Read<D3DXVECTOR3>(player.PositionAddress + _GameOffsets.Game_HeadPositionPointer) ;
	player.Distance = _GameData.CalcVector3Distance(_GameData.GameViewOrigin, player.Bones.Bone_Head);
	player.Health = HackProcess.Read<float>(player.HealthAddress + _GameOffsets.GameHealth);
	player.MaxHealth = HackProcess.Read<float>(player.HealthAddress + _GameOffsets.GameMaxHealth);
	if (player.Health <= 0 || player.Health > player.MaxHealth)
	{
		isAimbot = false;
		return;
	}

	if (Buttn_AimBotPre.MyChackSelect)
	{
		player.Pre = HackProcess.Read<D3DXVECTOR3>(player.PreAddress + _GameOffsets.EnemyPointer[11])*(player.Distance*(0.001 + AimBotPre.MyHeightInt*0.001));
		player.Bones.Bone_Head = player.Bones.Bone_Head + player.Pre;
	}
 	if (!Buttn_AimBotPre.MyChackSelect)
	{
		player.Bones.Bone_Head = player.Bones.Bone_Head;
	}
	if (Buttn_AimBotPreHealth.MyChackSelect)
	{
		player.Bones.Bone_Head.y += (AimBotHeight.MyHeightInt*0.02 )+(player.Distance*(0.001 + AimBotPreHealth.MyHeightInt*0.001));
	}
	if (!Buttn_AimBotPreHealth.MyChackSelect)
	{
		player.Bones.Bone_Head.y += AimBotHeight.MyHeightInt*0.02;
	}

	_GameData.WorldToScreen(player.Bones.Bone_Head, AimBotBody);

	if (AimBotBody.y != 0 || AimBotBody.x != 0)
	{
		D3DXVECTOR3 ScreenCenter = { float(rectangle.Width / 2),float(rectangle.Height / 2),0 };
		if (( _GameData.CalcVector2Distance(AimBotBody, ScreenCenter) <= AimFov *6 || isAimbot))
		{
			isAimbot = true;
			
			_GameData.CalcAngles(player.Angle, _GameData.GameViewOrigin, player.Bones.Bone_Head);
			if (Buttn_AimBotSpeed.MyChackSelect)
			{
				MAimbot(AimBotBody.x, AimBotBody.y);
				if ((_GameData.CalcVector2Distance(AimBotBody, ScreenCenter) <= AimFov * 2))
				{
					HackProcess.Write<D3DXVECTOR3>(_GameData.GameAimBotAddress, player.Angle);

				}
			}
			else
			{
				HackProcess.Write<D3DXVECTOR3>(_GameData.GameAimBotAddress, player.Angle);

			}
			
		}
	}
}


void GameAimBot()
{if (Buttn_AimBot.MyChackSelect)
{
	if (GetAimKey())
	{
		GameAimInit(AimPlayer);
	}
	else
	{
		isAimbot = false;
	}
}
	
}
POINT MenuPoint= POINT{  20,  20 };
void  DirectXRender()
{
	GameAimBot();//自瞄
	if (GameWindowResize = true)
	{
		pRender->Resize(D2D1::SizeU(rectangle.Width, rectangle.Height));
		GameWindowResize = false;
	}
	pRender->BeginDraw();
	pRender->Clear(D2D1_COLOR_F{ 1,0,0,0 });
	
	ReadGameData();

	

	if (!GameWindowMinimize)
	{

// 		开始画功能(player_data);

			MenuCreate(MenuPoint);
	
		
	}
	pRender->EndDraw();
}
