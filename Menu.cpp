#include "stdafx.h"
#include "Menu.h"
#include "Draw.h"
#include "player.h"



extern MyRectangle		rectangle;
extern IDWriteTextFormat*						pTextFormatSimSun;
MyChack Buttn_ESP;
MyChack Buttn_Health;
MyChack Buttn_Distance;
MyChack Buttn_AimBot;
MyChack Buttn_AimBotPre;
MyChack Buttn_AimBotPreHealth;
MyChack Buttn_AimBotSpeed;
MyChack Buttn_Team;




MyMenu MyMenuA;
MyRadio EspRadio;
MyRadio AimRadio;
MyRadio MiscRadio;
MyRadio SetRadio;

MyHealthButton		HealthStyle;
MyAimBotHeight		AimBotHeight;
MyAimBotKey			AimBotKey;
MyAimBotSpeed		Buttn_AimBotSpeedInt;

MyChangeButton    AimBotPre;
MyChangeButton    AimBotPreHealth;




POINT GetMousePoint()
{

		POINT MousePoint = POINT{};
		GetCursorPos(&MousePoint);
		MousePoint.x = MousePoint.x - rectangle.Left;
		MousePoint.y = MousePoint.y - rectangle.Top;
		return MousePoint;

}



void MenuCreate(POINT MenuPoint)
{

	if (MyMenuA.MyMenuHideShow)
	{
		EspRadio.MyRadioSelect = true;
		AimRadio.MyRadioSelect = false;
		MyMenuA.MyCreateMenu(MenuPoint, 260, 500, D2D1::ColorF::SkyBlue, "LV 키 숨기기/표시");



		Buttn_ESP.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 50, 15, 15, D2D1::ColorF::SkyBlue, "ESP");
		Buttn_Health.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 80, 15, 15, D2D1::ColorF::SkyBlue, "체력");
		Buttn_Distance.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 110, 15, 15, D2D1::ColorF::SkyBlue, "거리");
		Buttn_AimBot.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 140, 15, 15, D2D1::ColorF::SkyBlue, "에임 봇");
		Buttn_AimBotPre.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 170, 15, 15, D2D1::ColorF::SkyBlue, "프리뷰 이동 좌/우");
		Buttn_AimBotPreHealth.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 200, 15, 15, D2D1::ColorF::SkyBlue, "프리뷰 이동 상/하");
		
		Buttn_AimBotSpeed.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 230, 15, 15, D2D1::ColorF::SkyBlue, "휴머나이징");
		Buttn_Team.MyCreateChack(MenuPoint.x + 25, MenuPoint.y + 260, 15, 15, D2D1::ColorF::SkyBlue, "팀 조준");
		HealthStyle.MyCreateButton(MenuPoint.x + 170, MenuPoint.y + 72, 15, 15, D2D1::ColorF::SkyBlue);
		AimBotHeight.MyCreateButton(MenuPoint.x + 90, MenuPoint.y + 355, 33, 15, D2D1::ColorF::SkyBlue);
		AimBotKey.MyCreateButton(MenuPoint.x + 210, MenuPoint.y + 280, 33, 15, D2D1::ColorF::SkyBlue);
		Buttn_AimBotSpeedInt.MyCreateButton(MenuPoint.x + 90, MenuPoint.y + 280, 33, 15, D2D1::ColorF::SkyBlue);
		AimBotPre.MyCreateButton(MenuPoint.x + 210, MenuPoint.y + 355, 33, 15, D2D1::ColorF::SkyBlue,"X:");
		AimBotPreHealth.MyCreateButton(MenuPoint.x + 90, MenuPoint.y + 430, 33, 15, D2D1::ColorF::SkyBlue,"X:");










	}

}
