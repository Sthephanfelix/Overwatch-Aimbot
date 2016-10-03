#include "stdafx.h"
#include "Draw.h"
#include "DirectX.h"
#include <d3dx10.h>
#include "Player.h"


#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}
extern ID2D1HwndRenderTarget*	pRender;
extern RECT										MyRect;	
extern IDWriteFactory*					pWriteFactory;
extern ID2D1SolidColorBrush*		pBrush;
 ID2D1SolidColorBrush*					DrawBrush=NULL;
 ID2D1LinearGradientBrush*			DrawGradientBrush=NULL;


extern IDWriteTextFormat*				pTextFormatMsyh;
IDWriteTextFormat*						pTextFormatKaiTi=NULL;
IDWriteTextFormat*						pTextFormatSimSun = NULL;
IDWriteTextFormat*						pTextFormatHealth = NULL;


void DarwRelease()
{
	SAFE_RELEASE(DrawBrush);
	SAFE_RELEASE(pTextFormatKaiTi);
	SAFE_RELEASE(DrawGradientBrush);
}


bool TextFormatInit()//创建字体
{
	if (!pTextFormatKaiTi)
	{
		pWriteFactory->CreateTextFormat(L"KaiTi",
			NULL,
			DWRITE_FONT_WEIGHT_MEDIUM,    // 粗细
			DWRITE_FONT_STYLE_NORMAL,      // 斜体
			DWRITE_FONT_STRETCH_NORMAL,    // Stretch
			19.0f,                         // Size    
			L"MyFont",                      // Local
			&pTextFormatKaiTi);
	}
	if (!pTextFormatSimSun)
	{
		pWriteFactory->CreateTextFormat(L"SimSun",
			NULL,
			DWRITE_FONT_WEIGHT_MEDIUM,    // 粗细
			DWRITE_FONT_STYLE_NORMAL,      // 斜体
			DWRITE_FONT_STRETCH_NORMAL,    // Stretch
			18.0f,                         // Size    
			L"MyFont",                      // Local
			&pTextFormatSimSun);
	}
	if (!pTextFormatHealth)
	{
		pWriteFactory->CreateTextFormat(L"Msyh",
			NULL,
			DWRITE_FONT_WEIGHT_MEDIUM,    // 粗细
			DWRITE_FONT_STYLE_NORMAL,      // 斜体
			DWRITE_FONT_STRETCH_NORMAL,    // Stretch
			12.0f,                         // Size    
			L"MyFont",                      // Local
			&pTextFormatHealth);
	}
	return false;
}
bool SolidColorBrushInit()//创建固色画刷
{
	if (!DrawBrush)
	{
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &DrawBrush);
		return true;
	}
	return false;
}

bool LinearGradientBrushInit(int X, int Y, int W, int H, D3DCOLOR Color, D3DCOLOR Color2, float Alpha)//创建变色画刷
{
	SAFE_RELEASE(DrawGradientBrush);
	if (!DrawGradientBrush)
	{
		D2D1_GRADIENT_STOP GRADIENT_STOP[2];
		ID2D1GradientStopCollection* GradientStopCollection = NULL;
		GRADIENT_STOP[0].color = D2D1::ColorF(D2D1::ColorF(Color, Alpha));
		GRADIENT_STOP[0].position = 0.f;
		GRADIENT_STOP[1].color = D2D1::ColorF(D2D1::ColorF(Color2, Alpha));
		GRADIENT_STOP[1].position = 1.f;
		pRender->CreateGradientStopCollection(GRADIENT_STOP, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &GradientStopCollection);
		pRender->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F((float)X, (float)Y),
				D2D1::Point2F(float(W), float(H))),
			GradientStopCollection,
			&DrawGradientBrush
		);
		return true;
	}
	return false;
}

void DrawRenderText(int X, int Y, const char* text, D3DCOLOR Color, IDWriteTextFormat*TextFormat,float Alpha)//文字
{
	WCHAR WStr[256] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, text, strlen(text), WStr, strlen(text) * 2);
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawText(WStr, wcslen(WStr), TextFormat,
		D2D1::RectF((float)X, (float)Y, (float)MyRect.right, (float)MyRect.bottom), DrawBrush);
}
void DrawVectorLine(D3DXVECTOR3 X, D3DXVECTOR3 Y, D3DCOLOR Color, float Width  , float Alpha )//直线
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawLine(D2D1::Point2F(X.x, X.y), D2D1::Point2F(Y.x, Y.y), DrawBrush, Width);
}
void DrawLine(int X, int Y, int XX, int YY, D3DCOLOR Color, float Width , float Alpha)//直线
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawLine(D2D1::Point2F(X, Y), D2D1::Point2F(XX, YY), DrawBrush, Width);
}
void DrawFillRectangle(int X, int Y, int W, int H ,D3DCOLOR Color, float Alpha)//填充矩形
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->FillRectangle(D2D1::RectF((float)X, (float)Y, float(X + W), float(Y + H)), DrawBrush);
}
void DrawFillRectangleGradient(int X, int Y, int W, int H, D3DCOLOR Color, D3DCOLOR Color2, float Alpha = 1.0f)//填充变色矩形
{
	LinearGradientBrushInit( X,  Y,  W,  H, Color, Color2,  Alpha);
	pRender->FillRectangle(D2D1::RectF((float)X, (float)Y, float(X + W), float(Y + H)), DrawGradientBrush);
}
void DrawRectangle(float X, float Y, float W, float H, D3DCOLOR Color, float Width, float Alpha )//矩形
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawRectangle(D2D1::RectF(X, Y, X + W, Y + H), DrawBrush, Width);
}
void DrawRectangleGradient(float X, float Y, int W, int H, D3DCOLOR Color, D3DCOLOR Color2, float Width = 1.0f, float Alpha = 1.0f)//变色矩形
{
	LinearGradientBrushInit(X, Y, W, H, Color, Color2, Alpha);
	pRender->DrawRectangle(D2D1::RectF(X, Y, W, H), DrawGradientBrush, Width);
}
void DrawFillRoundedRectangle(int X, int Y, int W, int H, float RadianX, float RadianY, D3DCOLOR Color, float Alpha = 1.0f)//填充圆角
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((float)X, (float)Y, float(X + W), float(Y + H)),
		RadianX, RadianY), DrawBrush);
}
void DrawFillRoundedRectangleGradient(int X, int Y, int W, int H, float RadianX, float RadianY, D3DCOLOR Color, D3DCOLOR Color2,float Alpha = 1.0f)//填充变色圆角
{
	LinearGradientBrushInit(X, Y, W, H, Color, Color2, Alpha);
	pRender->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((float)X, (float)Y, float(X + W), float(Y + H)),
		RadianX, RadianY), DrawGradientBrush);
}
void DrawRoundedRectangle(int X, int Y, int W, int H, float RadianX, float RadianY, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f)//圆角
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((float)X, (float)Y, float(W), float(H)), RadianX, RadianY), DrawBrush, Width);
}
void DrawRoundedRectangleGradient(int X, int Y, int W, int H, float RadianX, float RadianY, D3DCOLOR Color, D3DCOLOR Color2, float Width = 1.0f, float Alpha = 1.0f)//变色圆角
{
	LinearGradientBrushInit(X, Y, W, H, Color, Color2, Alpha);
	pRender->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF((float)X, (float)Y, float(W), float(H)), RadianX, RadianY), DrawGradientBrush, Width);
}
void DrawFillRound(float X, float Y, float Radius, D3DCOLOR Color, float Alpha = 1.0f)//填充圆
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->FillEllipse(D2D1::Ellipse(D2D1::Point2F(X, Y), Radius, Radius), DrawBrush);
}
void DrawRound(float X, float Y, float Radius, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f)//圆
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(X, Y), Radius, Radius), DrawBrush, Width);
}
void DrawEllipse(float X, float Y, float RadiusX, float RadiusY, D3DCOLOR Color, float Width, float Alpha )//椭圆
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(X, Y), RadiusX, RadiusY), DrawBrush, Width);
}
void DrawFillEllipse(float X, float Y, float RadiusX, float RadiusY, D3DCOLOR Color, float Alpha = 1.0f)//填充椭圆
{
	DrawBrush->SetColor(D2D1::ColorF(Color, Alpha));
	pRender->FillEllipse(D2D1::Ellipse(D2D1::Point2F(X, Y), RadiusX, RadiusY), DrawBrush);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////							以下为自定义                   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawProgressBar(int X, int Y, int W, int H, int Value, int MaxValue, D3DCOLOR Color)//进度条
{
	int progress = (int)((float)Value / ((float)MaxValue / 100));
	int w = (int)((float)W / 100 * progress);
	D3DCOLOR color = D3DCOLOR_RGBA(255, 0, 0, 255);
	DrawFillRoundedRectangle(X, Y - 1, W + 1, H + 2, 15, 15, Color, 0);
	if (w >= 2)DrawFillRoundedRectangle(X + 1, Y, w - 1, H, 15, 15, color);
}

void DrawHalfRectangle(float X, float Y, float W, float H, D3DCOLOR Color, float Width, float Alpha)//断式矩形
{
	float DivW =W/4.5;
	float DivH = H/3.5;

	DrawLine(X, Y,	X + DivW, Y, Color, Width, Alpha);
 	DrawLine(X, Y, X,	Y + DivH, Color, Width, Alpha);

	DrawLine(X + W - DivW, Y, X + W , Y, Color, Width, Alpha);
	DrawLine(X + W, Y, X + W, Y + DivH, Color, Width, Alpha);

	DrawLine(X , Y + H, X + DivW, Y + H, Color, Width, Alpha);
	DrawLine(X , Y + H, X  , Y +H- DivH, Color, Width, Alpha);

	DrawLine(X + W - DivW, Y + H, X + W, Y + H, Color, Width, Alpha);
 	DrawLine(X + W, Y + H, X + W, Y + H - DivH, Color, Width, Alpha);
}
void DrawVerticalHealth(float Health, float MaxHealth,int X, int Y, int W, int H)//高要负数
{
	if (H >= -20)
		H = -21;

	int progress = (int)((float)Health / ((float)MaxHealth / 100));
	int h = (int)((float)H / 100 * progress);


	D3DCOLOR color = D3DCOLOR_RGBA(255, 0, 0, 255);
	if (progress >= 20) color = D3DCOLOR_RGBA(255, 165, 0, 255);
	if (progress >= 40) color = D3DCOLOR_RGBA(255, 255, 0, 255);
	if (progress >= 60) color = D3DCOLOR_RGBA(173, 255, 47, 255);
	if (progress >= 80) color = D3DCOLOR_RGBA(0, 255, 0, 255);

	DrawFillRoundedRectangle(X, Y - 1, W, H, 3, 1, D3DCOLOR_RGBA(0, 0, 0, 255));//最大血量黑色
	DrawFillRoundedRectangle(X + 1, Y, W, h, 3, 1, color, 0.8);//当前血量

// 	char text[20];
// 	sprintf_s(text, "HP: %d", (INT)Health);
// 	DrawRenderText(X-10, Y, text, color, pTextFormatHealth);
}
void DrawTransverseHealth(float Health, float MaxHealth, float _X, int X, int Y, int W, int H)
{

	if (W <= 30)
		W = 31;
	int progress = (int)((float)Health / ((float)MaxHealth / 100));
	int w = (int)((float)W / 100 * progress);

	D3DCOLOR color = D3DCOLOR_RGBA(255, 0, 0, 255);
	if (progress >= 20) color = D3DCOLOR_RGBA(255, 165, 0, 255);
	if (progress >= 40) color = D3DCOLOR_RGBA(255, 255, 0, 255);
	if (progress >= 60) color = D3DCOLOR_RGBA(173, 255, 47, 255);
	if (progress >= 80) color = D3DCOLOR_RGBA(0, 255, 0, 255);

	DrawFillRoundedRectangle(X, Y - 1, W,H, 3,1,D3DCOLOR_RGBA(0, 0, 0, 255));//最大血量黑色
	DrawFillRoundedRectangle(X + 1, Y, w, H, 3,1,color);//当前血量

// 	char text[20];
// 	sprintf_s(text, "HP: %d", (INT)Health);
// 	DrawRenderText(_X - 10, Y, text, color, pTextFormatHealth);
}
void DrawDistance(float Distance, int X, int Y)
{
	char text[20];
	sprintf_s(text, "[%d m]", (INT)Distance);
	DrawRenderText(X, Y+15, text, D2D1::ColorF::White, pTextFormatHealth);

}
void MyChack::MyCreateChack(int X, int Y, int W, int H, D3DCOLOR Color,const char* Text)
{
	MyChackX = X;
	MyChackY = Y;
	MyChackW = W;
	MyChackH = H;
	DrawRectangle(X - 1, Y - 1, W + 2, H + 2, D2D1::ColorF::Tomato,1.5);
	DrawRectangle(X, Y, W, H, Color,1.5);
	if (MyChackSelect)
	{
		DrawFillRectangle(X+2, Y+2, W-4, H-4, Color,1.5);
	}
	DrawRenderText(X + 18, Y - 3, Text, D2D1::ColorF::Black, pTextFormatSimSun);
	DrawRenderText(X+20, Y-5, Text, D2D1::ColorF::White, pTextFormatSimSun);
}

void MyChack::MyChackClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
		if (ClickMouse&&	
		((MyChackX < MousePoint.x) && ((MyChackX+MyChackW) > MousePoint.x))	&&
		((MyChackY < MousePoint.y) && ((MyChackH+ MyChackY) > MousePoint.y))
		)
	{
			MyChackSelect = !MyChackSelect;
			Sleep(100);

	}
}
void MyMenu::MyCreateMenu(POINT MenuPoint ,int W, int H, D3DCOLOR Color, const char* Text)
{
	MyMenuX = MenuPoint.x;
	MyMenuY = MenuPoint.y;
	MyMenuW = W;
	MyMenuH = H;
	if (MyMenuHideShow)
	{
		DrawFillRoundedRectangle(MyMenuX, MyMenuY, W, 25, 7, 7, Color);
		DrawFillRectangle(MyMenuX, MyMenuY + 10, W, 15, Color);
//		DrawFillRectangle(MyMenuX, MyMenuY + 10, 111, H+3, Color);
		DrawLine(MyMenuX + 1, MyMenuY + 25, MyMenuX + 1, MyMenuY + H + 12, Color);//左竖
		DrawLine(MyMenuX + W - 1, MyMenuY + 25, MyMenuX + W - 1, MyMenuY + H + 12, Color);//右竖
		DrawLine(MyMenuX, MyMenuY + H + 13, MyMenuX + W, MyMenuY + H + 13, Color);
		DrawFillRectangle(MyMenuX + 1, MyMenuY + 13, W - 2, H, D2D1::ColorF::Black, 0.6f);
		DrawRenderText(MyMenuX + 13, MyMenuY, Text, D2D1::ColorF::Black, pTextFormatMsyh);
		DrawRenderText(MyMenuX + 15, MyMenuY - 2, Text, D2D1::ColorF::White, pTextFormatMsyh);

	}
}
extern	POINT MenuPoint;
			POINT DragPoint;
void MyMenu::MyClickPoint(POINT MousePoint)
{
	bool ClickMouse = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	if (ClickMouse)
	{
		if (!DragMenu&&((MyMenuX < MousePoint.x) && ((MyMenuX + MyMenuW) > MousePoint.x)) &&
			((MyMenuY < MousePoint.y) && ((25 + MyMenuY) > MousePoint.y))
			)
	{

		if (DragPoint.y == 0 | DragPoint.x == 0)
				{
					DragPoint.y = MousePoint.y - MenuPoint.y;
					DragPoint.x = MousePoint.x - MenuPoint.x;
				}
				DragMenu = true;		
		}

		if (DragMenu == true)
		{
			MenuPoint.y = MousePoint.y - DragPoint.y;
			MenuPoint.x = MousePoint.x - DragPoint.x;
		}
	}
	else
	{
		DragMenu = false;
		DragPoint = POINT{};
	}

	
}
void MyRadio::MyCreateRadio(int X, int Y, int W, int H, D3DCOLOR Color, const char* Text) 
{
	MyRadioX = X;
	MyRadioY = Y;
	MyRadioW = W;
	MyRadioH = H;
	//DrawRectangle(X - 1, Y - 1, W + 2, H + 2, Color,0.7);
	DrawFillRectangle(X, Y, W, H, Color);

	if (!MyRadioSelect)
	{
		DrawFillRectangle(X, Y, W, H, D2D1::ColorF::Black, 0.6f);
	}
	DrawRenderText(X+4, Y + (H/5)+2, Text, D2D1::ColorF::Black, pTextFormatSimSun);
	DrawRenderText(X+6, Y +(H/5), Text, D2D1::ColorF::White, pTextFormatSimSun);
}
void MyHealthButton::MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color)
{
	MyButtonX = X + 6;
	MyButtonY = Y + (H / 2);
	MyButtonW = W;
	MyButtonH = H;
	DrawFillRectangle(MyButtonX, MyButtonY, W, H, Color);
	DrawLine (MyButtonX+11 , MyButtonY+1, MyButtonX+3, MyButtonY+8, D2D1::ColorF::White,2.5);
	DrawLine(MyButtonX+3 , MyButtonY + 8, MyButtonX+11, MyButtonY + 14, D2D1::ColorF::White, 2.5);
	DrawFillRectangle(MyButtonX+50, MyButtonY, W, H, Color);
	DrawLine(MyButtonX + 53, MyButtonY + 1, MyButtonX + 62, MyButtonY + 8, D2D1::ColorF::White, 3);
	DrawLine(MyButtonX + 62, MyButtonY + 8, MyButtonX + 53, MyButtonY + 14, D2D1::ColorF::White, 3);
	if (MyButtonSelect)
	{
		DrawRenderText(MyButtonX + 22, Y + (H / 5) + 2, "横", D2D1::ColorF::Black, pTextFormatSimSun);
		DrawRenderText(MyButtonX + 24, Y + (H / 5), "横", D2D1::ColorF::White, pTextFormatSimSun);
	}
	if (!MyButtonSelect)
	{
		DrawRenderText(MyButtonX + 22, Y + (H / 5) + 2, "竖", D2D1::ColorF::Black, pTextFormatSimSun);
		DrawRenderText(MyButtonX + 24, Y + (H / 5), "竖", D2D1::ColorF::White, pTextFormatSimSun);
	}

}
void MyHealthButton::MyButtonClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	if (ClickMouse &&
			((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
			((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
			)
		{
			MyButtonSelect = !MyButtonSelect;
			Sleep(100);
		}
	if (ClickMouse &&
		((MyButtonX+50 < MousePoint.x) && ((MyButtonX+50 + MyButtonW) > MousePoint.x)) &&
		((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
		)
	{
		MyButtonSelect = !MyButtonSelect;
		Sleep(100);
	}
}
void MyAimBotHeight::MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color)
{
	MyButtonX = X + 6;
	MyButtonY = Y + (H / 2);
	MyButtonW = W;
	MyButtonH = H;
	DrawFillRectangle(MyButtonX, MyButtonY, W, H, Color);
	DrawLine(MyButtonX  + MyButtonW / 2, MyButtonY + 1, MyButtonX + 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX  + MyButtonW / 2, MyButtonY + 1,  MyButtonX + MyButtonW-2 , MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawFillRectangle(MyButtonX , MyButtonY+50, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY +	64, MyButtonX + 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + MyButtonW - 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	char A[100];
	sprintf_s(A, "自瞄高度: %d", MyHeightInt);
	DrawRenderText(MyButtonX -83, MyButtonY + 19, A, D2D1::ColorF::White, pTextFormatSimSun);
}
void MyAimBotHeight::MyButtonClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
		)
	{
		MyHeightInt += 1;
		Sleep(100);
	}
	if (ClickMouse &&
		((MyButtonX  < MousePoint.x) && ((MyButtonX  + MyButtonW) > MousePoint.x)) &&
		((MyButtonY + 50 < MousePoint.y) && ((MyButtonH + MyButtonY + 50) > MousePoint.y))
		)
	{
		MyHeightInt -= 1;
		Sleep(100);
	}
}
void MyAimBotKey::MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color)
{
	MyButtonX = X + 6;
	MyButtonY = Y + (H / 2);
	MyButtonW = W;
	MyButtonH = H;
	DrawFillRectangle(MyButtonX, MyButtonY, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + MyButtonW - 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawFillRectangle(MyButtonX, MyButtonY + 50, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + MyButtonW - 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	char A[100];
	if (MyHeightInt == 1)
	{
		DrawRenderText(MyButtonX - 73, MyButtonY + 19, "自瞄键: 左键", D2D1::ColorF::White, pTextFormatSimSun);
		return;
	}
	if (MyHeightInt == 2)
	{
		DrawRenderText(MyButtonX - 73, MyButtonY + 19, "自瞄键: 右键", D2D1::ColorF::White, pTextFormatSimSun);
		return;
	}
	if (MyHeightInt == 4)
	{
		DrawRenderText(MyButtonX - 73, MyButtonY + 19, "自瞄键: 中键", D2D1::ColorF::White, pTextFormatSimSun);
		return;
	}
	sprintf_s(A, "自瞄键:  %d", MyHeightInt);
	DrawRenderText(MyButtonX - 73, MyButtonY + 19, A, D2D1::ColorF::White, pTextFormatSimSun);
}
void MyAimBotKey::MyButtonClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
		)
	{
		if (MyHeightInt == 0xfe)
		{
			return;
		}
		MyHeightInt += 1;
		Sleep(100);
	}
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY + 50 < MousePoint.y) && ((MyButtonH + MyButtonY + 50) > MousePoint.y))
		)
	{
		if (MyHeightInt == 1)
		{
			return;
		}
		MyHeightInt -= 1;
		Sleep(100);
	}
}

void MyAimBotSpeed::MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color)
{
	MyButtonX = X + 6;
	MyButtonY = Y + (H / 2);
	MyButtonW = W;
	MyButtonH = H;
	DrawFillRectangle(MyButtonX, MyButtonY, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + MyButtonW - 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawFillRectangle(MyButtonX, MyButtonY + 50, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + MyButtonW - 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	char A[100];
	sprintf_s(A, "平滑速度:  %d", MyHeightInt);
	DrawRenderText(MyButtonX - 85, MyButtonY + 19, A, D2D1::ColorF::White, pTextFormatSimSun);
}
void MyAimBotSpeed::MyButtonClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
		)
	{
		MyHeightInt += 1;
		Sleep(100);
	}
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY + 50 < MousePoint.y) && ((MyButtonH + MyButtonY + 50) > MousePoint.y))
		)
	{
		MyHeightInt -= 1;
		Sleep(100);
	}
}
void MyChangeButton::MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color,char*Text)
{
	MyButtonX = X + 6;
	MyButtonY = Y + (H / 2);
	MyButtonW = W;
	MyButtonH = H;
	DrawFillRectangle(MyButtonX, MyButtonY, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 1, MyButtonX + MyButtonW - 2, MyButtonY + 12, D2D1::ColorF::White, 2.5);
	DrawFillRectangle(MyButtonX, MyButtonY + 50, W, H, Color);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	DrawLine(MyButtonX + MyButtonW / 2, MyButtonY + 64, MyButtonX + MyButtonW - 2, MyButtonY + 53, D2D1::ColorF::White, 2.5);
	char A[100];
	sprintf_s(A, "%d", MyHeightInt);
	DrawRenderText(MyButtonX - 83, MyButtonY + 19, Text, D2D1::ColorF::White, pTextFormatSimSun);
	DrawRenderText(MyButtonX + 13, MyButtonY + 20, A, D2D1::ColorF::White, pTextFormatSimSun);
}
void MyChangeButton::MyButtonClick(POINT MousePoint)
{
	bool ClickMouse = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY < MousePoint.y) && ((MyButtonH + MyButtonY) > MousePoint.y))
		)
	{
		MyHeightInt += 1;
		Sleep(100);
	}
	if (ClickMouse &&
		((MyButtonX < MousePoint.x) && ((MyButtonX + MyButtonW) > MousePoint.x)) &&
		((MyButtonY + 50 < MousePoint.y) && ((MyButtonH + MyButtonY + 50) > MousePoint.y))
		)
	{
		MyHeightInt -= 1;
		Sleep(100);
	}
}