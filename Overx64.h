
// Overx64.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif




void GetGameWindowRect();
class COverx64App : public CWinApp
{
public:

	COverx64App();
public:
	virtual BOOL InitInstance();
	virtual void MyCreateWindow();









	DECLARE_MESSAGE_MAP()
};
extern COverx64App theApp;