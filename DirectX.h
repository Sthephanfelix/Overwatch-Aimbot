#pragma once
#include "Player.h"
void CleanupDX();//Çå³ýDX
void DirectXInit();
void  DirectXRender();
void GetClosestPlayerToCrossHair(Player player, float aimfov, float &max);
