#pragma once
#include <d3dx10.h>
#include <d3d10.h>



bool SolidColorBrushInit();
bool TextFormatInit();
void DrawRectangle(float X, float Y, float W, float H, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f);
void DrawEllipse(float X, float Y, float RadiusX, float RadiusY, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f);
void DrawFillRectangle(int X, int Y, int W, int H, D3DCOLOR Color, float Alpha = 1.0f);
void DrawLine(int X, int Y, int XX, int YY, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f);
void DrawRenderText(int X, int Y, const char* text, D3DCOLOR Color, IDWriteTextFormat*TextFormat, float Alpha = 1.0f);

void DrawHalfRectangle(float X, float Y, float W, float H, D3DCOLOR Color, float Width =2.0f, float Alpha = 1.0f);
void DrawVectorLine(D3DXVECTOR3 X, D3DXVECTOR3 Y, D3DCOLOR Color, float Width = 1.0f, float Alpha = 1.0f);
void DrawVerticalHealth(float Health, float MaxHealth,  int X, int Y, int W, int H);
void DrawTransverseHealth(float Health, float MaxHealth, float _X, int X, int Y, int W, int H);
void DrawDistance(float Distance, int X, int Y);
class MyChack
{
public:
	int MyChackX, MyChackY, MyChackW, MyChackH;
	void MyCreateChack(int X, int Y, int W, int H, D3DCOLOR Color, const char* Text);
		void MyChackClick(POINT MousePoint);	
		bool MyChackSelect;
};
class MyRadio
{
public:
	int MyRadioX, MyRadioY, MyRadioW, MyRadioH;
	void MyCreateRadio(int X, int Y, int W, int H, D3DCOLOR Color, const char* Text);
	void MyRadioClick(POINT MousePoint);
	bool MyRadioSelect;

	

};
class MyMenu
{
public:
	bool DragMenu = false;
	bool MyMenuHideShow = true;


	int MyMenuX, MyMenuY, MyMenuW, MyMenuH;
	void MyCreateMenu(POINT MenuPoint, int W, int H, D3DCOLOR Color, const char* Text);
	void MyClickPoint(POINT MousePoint);



	

};
class MyHealthButton
{
public:
	int MyButtonX, MyButtonY, MyButtonW, MyButtonH;
	void MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color);
	void MyButtonClick(POINT MousePoint);
	bool MyButtonSelect;
};
class MyAimBotHeight
{
public:
	int MyButtonX, MyButtonY, MyButtonW, MyButtonH;
	void MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color);
	void MyButtonClick(POINT MousePoint);
	int MyHeightInt=0;
};
class MyAimBotKey
{
public:
	int MyButtonX, MyButtonY, MyButtonW, MyButtonH;
	void MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color);
	void MyButtonClick(POINT MousePoint);
	DWORD MyHeightInt = 2;
};
class MyAimBotSpeed
{
public:
	int MyButtonX, MyButtonY, MyButtonW, MyButtonH;
	void MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color);
	void MyButtonClick(POINT MousePoint);
	int MyHeightInt = 0;
};
class MyChangeButton
{
public:
	int MyButtonX, MyButtonY, MyButtonW, MyButtonH;
	void MyCreateButton(int X, int Y, int W, int H, D3DCOLOR Color, char*Text);
	void MyButtonClick(POINT MousePoint);
	int MyHeightInt = 0;
};
