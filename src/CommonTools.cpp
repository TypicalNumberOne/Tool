#include "pch.h"

#include "CommonTools.h"
#include "Message.h"


void Typical_Tool::WindowsSystem::SetDisplaySize(int displayWidth, int displayHeight)
{
	//初始化
	DEVMODE NewDevMode;
	EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &NewDevMode);

	//记录修改信息
	NewDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	NewDevMode.dmPelsWidth = displayWidth;
	NewDevMode.dmPelsHeight = displayHeight;

	//根据修改信息 修改屏幕分辨率
	ChangeDisplaySettings(&NewDevMode, 0);
}

 bool Typical_Tool::WindowsSystem::IsUserAdmin()
{
	BOOL retVal = FALSE;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	BOOL result = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup);

	if (result)
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &retVal))
		{
			retVal = FALSE;
		}
		FreeSid(AdministratorsGroup);
	}
	return retVal;
}

 void Typical_Tool::WindowsSystem::MoveCursorLocation(int x, int y) {
	 COORD pos = { x,y };
	 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	 SetConsoleCursorPosition(hOut, pos);
 }

float Typical_Tool::GameTools::GetFps()
{
	//临时统计
	static int tempCount = 0;
	static Timers timer; //时间
	static float fps; //帧率

	if (tempCount > FPS_COUNT)
	{
		//将时间节点拓展到两个
		if (timer.GetTimerSize() < 2)
			timer.AddTimer();

		tempCount = 0;
		timer.SetTimer(timer.GetTime(), 2);
		float tempTime = timer.计算首尾间隔();
		fps = FPS_COUNT / (tempTime / 1000.0f); //获取的系统时间为毫秒数
		timer.SetTimer(timer.GetTime(), 1);
	}

	tempCount++;
	return fps;
}
