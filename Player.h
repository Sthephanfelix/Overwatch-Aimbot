#pragma once
#include "HackProcess.h"
#include <d3dx10.h>
#include <d3d10.h>
#include "Draw.h"
#include "VMProtectSDK.h"
#include <vector>
#pragma  comment(lib,"d3dx10.lib")
#pragma  comment(lib,"d3d10.lib")
using namespace std;
#define M_PI 3.14159265358979323846
extern	CHackProcess					HackProcess;




struct AxisAlignedBox
{
	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
};


 struct MyRectangle
 {
	 int Left;
	 int Top;
	 int Width;
	 int Height;
 };
 
struct  RadDollVector3
{
	D3DXVECTOR3
		Bone_Head,
		Bone_Neck,
		Bone_Chest,
		Bone_Belly,
		Bone_LeftShoulder,
		Bone_LeftElbow,
		Bone_LeftHand,
		Bone_LeftButt,
		Bone_LeftKnee,
		Bone_LeftFoot,
		Bone_LeftTiptoe,
		Bone_RightShoulder,
		Bone_RightElbow,
		Bone_RightHand,
		Bone_RightButt,
		Bone_RightKnee,
		Bone_RightFoot,
		Bone_RightTiptoe;

};

enum BoneNumber
{
	BoneHead						=	6,
	BoneNeck						=	4,
	BoneChest						=	3,
	BoneBelly							=	0,
	BoneLeftShoulder			=	11,
	BoneLeftElbow				=	12,
	BoneLeftHand					=	13,
	BoneLeftButt					=	24,
	BoneLeftKnee					=	25,
	BoneLeftFoot					= 26,
	BoneLeftTiptoe				=	27,
	BoneRightShoulder			=	17,
	BoneRightElbow				=	18,
	BoneRightHand				=	19,
	BoneRightButt					=	28,
	BoneRightKnee				=	29,
	BoneRightFoot				=	30,
	BoneRightTiptoe				=	31,
};

class GameDataWork
{
public:
	DWORD					GameProcessID;
	INT64						GameModuleOffset;

	INT64						pGameAddress;

	INT64						GameViewOffset;
	INT64						GameViewAddress;
	D3DXVECTOR3		GameViewOrigin;
	D3DXMATRIX			GameViewMatrix;
	D3DXMATRIX			GameViewFov;

	INT64						GameAimBotOffset;
	INT64						GameAimBotAddress;





	MyRectangle			rect;

	INT64						GameEnemyOffset;
	INT64						pGameEnemyAddress;


	float CalcVector2Distance(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
	{
		float d = 0;
		d = (float)sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
		return d;
	}
	float CalcVector3Distance(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
	{
		float d = 0;
		d = (float)sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)+ (a.z - b.z)*(a.z - b.z));
		return d;
	}
	D3DXVECTOR3 CalcVector3DivSub(D3DXVECTOR3 &a, D3DXVECTOR3 &b,float IntDiv)
	{
		return (a-b)/ IntDiv;
	}
	float VectorLength2D(D3DXVECTOR3 Vector)
	{
		return sqrt(((Vector.x)*(Vector.x)) + ((Vector.z)*(Vector.z)));
	}
	void CalcAngles(D3DXVECTOR3& angles, D3DXVECTOR3 vOrigin, D3DXVECTOR3 aimPoint)
	{
		angles = aimPoint-vOrigin;
		D3DXVec3Normalize(&angles, &angles);

	}
	bool WorldToScreen(D3DXVECTOR3 _Enemy, D3DXVECTOR3 &_Screen)
	{
		_Screen = D3DXVECTOR3(0, 0,0);
		float ScreenW = 0;

		ScreenW = (GameViewMatrix._14 * _Enemy.x) + (GameViewMatrix._24* _Enemy.y) + (GameViewMatrix._34 * _Enemy.z + GameViewMatrix._44);

		if (ScreenW < 0.0001f)
			return false;

		float ScreenY = (GameViewMatrix._12 * _Enemy.x) + (GameViewMatrix._22 * _Enemy.y) + (GameViewMatrix._32 * _Enemy.z + GameViewMatrix._42);
		float ScreenX = (GameViewMatrix._11 * _Enemy.x) + (GameViewMatrix._21 * _Enemy.y) + (GameViewMatrix._31 * _Enemy.z + GameViewMatrix._41);

		_Screen.y = (rect.Height / 2) - (rect.Height / 2) * ScreenY / ScreenW;
		_Screen.x = (rect.Width / 2) + (rect.Width / 2) * ScreenX / ScreenW;
		return true;
	}
	bool WorldToScreen(D3DXVECTOR3 _Enemy, D3DXVECTOR3  _Tiptop, D3DXVECTOR3 &_Screen)
	{
		_Screen = D3DXVECTOR3(0, 0,0);
		float HeadHeight = _Tiptop.y;// +0.7f;
		float ScreenW = (GameViewMatrix._14 * _Enemy.x) + (GameViewMatrix._24 * HeadHeight) + (GameViewMatrix._34 * _Enemy.z + GameViewMatrix._44);

		if (ScreenW < 0.0001f)
			return false;

		float ScreenX = (GameViewMatrix._11 * _Enemy.x) + (GameViewMatrix._21 * HeadHeight) + (GameViewMatrix._31 * _Enemy.z + GameViewMatrix._41);
		float ScreenY = (GameViewMatrix._12 * _Enemy.x) + (GameViewMatrix._22 * HeadHeight) + (GameViewMatrix._32 * _Enemy.z + GameViewMatrix._42);

		_Screen.x = (rect.Width / 2) + (rect.Width / 2) * ScreenX / ScreenW;
		_Screen.y = (rect.Height / 2) - (rect.Height / 2) * ScreenY / ScreenW;
		return true;
	}

};
extern	GameDataWork				_GameData;
volatile class Player
{
public:
	INT64						PlayerAddress;
	INT64						HealthAddress;;//血量
	float							Health;//血量
	float							MaxHealth;//最大血量
	int							Team;//判断团队 类型
	INT64						PositionAddress;
	D3DXVECTOR3		Position;//坐标
	D3DXVECTOR3		ScreenPosition;//坐标
	INT64						AngleAddress;
	D3DXVECTOR3		Angle;//角度
	D3DXVECTOR3		AimPosition;
	D3DXVECTOR3		Pre;
	INT64		PreAddress;
	RadDollVector3		Bones;//骨骼
	RadDollVector3		ScreenBones;//骨骼
	 int							Ammo, AmmoClip;//子弹 子弹夹
	 float						Yaw;//视角
	 float						Pitch;//视角
	 float						Distance; // 距离
	 int							Visible;//看见
	 D3DXCOLOR          Color;




	 INT64						GameBoneOffset = 0x484;
	 bool CalcPlayerBone(int Id,D3DXVECTOR3 &BonePosition)
	 {
		 INT	 pBoneAddress = HackProcess.Read<INT>(PlayerAddress + GameBoneOffset);
		 if (!HackProcess.IsValid64(pBoneAddress)) return false;
		 BonePosition = HackProcess.Read<D3DXVECTOR3>(pBoneAddress + Id * 0xa0 + 0x40);
		 return true;
	 }
	 bool FCalcPlayerBone()
	 {
		 INT	 pBoneAddress = HackProcess.Read<INT>(PlayerAddress + GameBoneOffset);
		 if (!HackProcess.IsValid64(pBoneAddress)) return false;
		 
		 for (size_t i = 0; i < 100; i++)
		 {
			 D3DXVECTOR3 BonePosition = HackProcess.Read<D3DXVECTOR3>(pBoneAddress + i * 0xa0 + 0x40);
			 D3DXVECTOR3 A;
			 _GameData.WorldToScreen(BonePosition, A);
			 DrawFillRectangle(A.x, A.y, 5, 5, D2D1::ColorF::Magenta);
			 
		 }
		 return true;
	 }


	 bool GetPlayerBone()
	 {
		 if (!CalcPlayerBone(BoneHead,					Bones.Bone_Head) || 
			 !CalcPlayerBone(BoneNeck,					Bones.Bone_Neck) ||
			 !CalcPlayerBone(BoneChest,					Bones.Bone_Chest) ||
			 !CalcPlayerBone(BoneBelly,					Bones.Bone_Belly) ||
			 !CalcPlayerBone(BoneLeftShoulder,		Bones.Bone_LeftShoulder) ||
			 !CalcPlayerBone(BoneLeftElbow,			Bones.Bone_LeftElbow) ||
			 !CalcPlayerBone(BoneLeftHand,				Bones.Bone_LeftHand) ||
			 !CalcPlayerBone(BoneLeftButt,				Bones.Bone_LeftButt) ||
			 !CalcPlayerBone(BoneLeftKnee,				Bones.Bone_LeftKnee) ||
			 !CalcPlayerBone(BoneLeftFoot,				Bones.Bone_LeftFoot) ||
			 !CalcPlayerBone(BoneLeftTiptoe,			Bones.Bone_LeftTiptoe) ||
			 !CalcPlayerBone(BoneRightShoulder,	Bones.Bone_RightShoulder) ||
			 !CalcPlayerBone(BoneRightElbow,			Bones.Bone_RightElbow) ||
			 !CalcPlayerBone(BoneRightHand,			Bones.Bone_RightHand) ||
			 !CalcPlayerBone(BoneRightButt,			Bones.Bone_RightButt) ||
			 !CalcPlayerBone(BoneRightKnee,			Bones.Bone_RightKnee) ||
			 !CalcPlayerBone(BoneRightFoot,			Bones.Bone_RightFoot) ||
			 !CalcPlayerBone(BoneRightTiptoe,		Bones.Bone_RightTiptoe)
			 )
			 return false;
		 return true;

	 }

	 void DrawBone()
	 {


		 if (_GameData.WorldToScreen(Bones.Bone_Head,							ScreenBones.Bone_Head) &&
			 _GameData.WorldToScreen(Bones.Bone_Neck,								ScreenBones.Bone_Neck) &&
			 _GameData.WorldToScreen(Bones.Bone_Chest,							ScreenBones.Bone_Chest) &&
			 _GameData.WorldToScreen(Bones.Bone_Belly,								ScreenBones.Bone_Belly) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftShoulder,				ScreenBones.Bone_LeftShoulder) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftElbow,						ScreenBones.Bone_LeftElbow) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftHand,						ScreenBones.Bone_LeftHand) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftButt,						ScreenBones.Bone_LeftButt) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftKnee,						ScreenBones.Bone_LeftKnee) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftFoot,						ScreenBones.Bone_LeftFoot) &&
			 _GameData.WorldToScreen(Bones.Bone_LeftTiptoe,					ScreenBones.Bone_LeftTiptoe) &&
			 _GameData.WorldToScreen(Bones.Bone_RightShoulder,				ScreenBones.Bone_RightShoulder) &&
			 _GameData.WorldToScreen(Bones.Bone_RightElbow,					ScreenBones.Bone_RightElbow) &&
			 _GameData.WorldToScreen(Bones.Bone_RightHand,					ScreenBones.Bone_RightHand) &&
			 _GameData.WorldToScreen(Bones.Bone_RightButt,						ScreenBones.Bone_RightButt) &&
			 _GameData.WorldToScreen(Bones.Bone_RightKnee,					ScreenBones.Bone_RightKnee) &&
			 _GameData.WorldToScreen(Bones.Bone_RightFoot,						ScreenBones.Bone_RightFoot) &&
			 _GameData.WorldToScreen(Bones.Bone_RightTiptoe,					ScreenBones.Bone_RightTiptoe) )
		 {
			 int stroke = 3;
			 int strokeW = stroke % 2 == 0 ? stroke / 2 : (stroke - 1) / 2;
			
			 DrawEllipse(ScreenBones.Bone_Head.x, ScreenBones.Bone_Head.y,
				 _GameData.CalcVector2Distance(ScreenBones.Bone_Head, ScreenBones.Bone_Neck)+
				 _GameData.CalcVector2Distance(ScreenBones.Bone_Head, ScreenBones.Bone_Neck)/3,
				 _GameData.CalcVector2Distance(ScreenBones.Bone_Head, ScreenBones.Bone_Neck)+
				 _GameData.CalcVector2Distance(ScreenBones.Bone_Head, ScreenBones.Bone_Neck)/2,
				 Color, 2);
			 DrawFillRectangle(ScreenBones.Bone_Head.x - strokeW,	ScreenBones.Bone_Head.y -	strokeW, stroke, stroke, Color);
			 DrawFillRectangle(ScreenBones.Bone_Neck.x - strokeW,	ScreenBones.Bone_Neck.y -	strokeW, stroke, stroke, Color);
			 DrawFillRectangle(ScreenBones.Bone_Chest.x - strokeW,ScreenBones.Bone_Chest.y -	strokeW, stroke, stroke, Color);
			 DrawFillRectangle(ScreenBones.Bone_Belly.x - strokeW, ScreenBones.Bone_Belly.y -		strokeW, stroke, stroke, Color);
			 DrawLine(ScreenBones.Bone_Neck.x, 
				 ScreenBones.Bone_Neck.y, 
				 ScreenBones.Bone_Chest.x, 
				 ScreenBones.Bone_Chest.y, Color, 2);
			 DrawLine(ScreenBones.Bone_Chest.x,
				 ScreenBones.Bone_Chest.y,
				 ScreenBones.Bone_Belly.x,
				 ScreenBones.Bone_Belly.y, Color, 2);

			 DrawLine(ScreenBones.Bone_Neck.x,
				 ScreenBones.Bone_Neck.y,
				 ScreenBones.Bone_LeftShoulder.x,
				 ScreenBones.Bone_LeftShoulder.y, Color, 2);
			 DrawLine(ScreenBones.Bone_LeftShoulder.x,
				 ScreenBones.Bone_LeftShoulder.y,
				 ScreenBones.Bone_LeftElbow.x,
				 ScreenBones.Bone_LeftElbow.y, Color, 2);
			 DrawLine(ScreenBones.Bone_LeftElbow.x,
				 ScreenBones.Bone_LeftElbow.y,
				 ScreenBones.Bone_LeftHand.x,
				 ScreenBones.Bone_LeftHand.y, Color, 2);

			 DrawLine(ScreenBones.Bone_Neck.x,
				 ScreenBones.Bone_Neck.y,
				 ScreenBones.Bone_RightShoulder.x,
				 ScreenBones.Bone_RightShoulder.y, Color, 2);
			 DrawLine(ScreenBones.Bone_RightShoulder.x,
				 ScreenBones.Bone_RightShoulder.y,
				 ScreenBones.Bone_RightElbow.x,
				 ScreenBones.Bone_RightElbow.y, Color, 2);
			 DrawLine(ScreenBones.Bone_RightElbow.x,
				 ScreenBones.Bone_RightElbow.y,
				 ScreenBones.Bone_RightHand.x,
				 ScreenBones.Bone_RightHand.y, Color, 2);

			 DrawLine(ScreenBones.Bone_Belly.x,
				 ScreenBones.Bone_Belly.y,
				 ScreenBones.Bone_LeftButt.x,
				 ScreenBones.Bone_LeftButt.y, Color, 2);
			 DrawLine(ScreenBones.Bone_LeftButt.x,
				 ScreenBones.Bone_LeftButt.y,
				 ScreenBones.Bone_LeftKnee.x,
				 ScreenBones.Bone_LeftKnee.y, Color, 2);
			 DrawLine(ScreenBones.Bone_LeftKnee.x,
				 ScreenBones.Bone_LeftKnee.y,
				 ScreenBones.Bone_LeftFoot.x,
				 ScreenBones.Bone_LeftFoot.y, Color, 2);
			 DrawLine(ScreenBones.Bone_LeftFoot.x,
				 ScreenBones.Bone_LeftFoot.y,
				 ScreenBones.Bone_LeftTiptoe.x,
				 ScreenBones.Bone_LeftTiptoe.y, Color, 2);

			 DrawLine(ScreenBones.Bone_Belly.x,
				 ScreenBones.Bone_Belly.y,
				 ScreenBones.Bone_RightButt.x,
				 ScreenBones.Bone_RightButt.y, Color, 2);
			 DrawLine(ScreenBones.Bone_RightButt.x,
				 ScreenBones.Bone_RightButt.y,
				 ScreenBones.Bone_RightKnee.x,
				 ScreenBones.Bone_RightKnee.y, Color, 2);
			 DrawLine(ScreenBones.Bone_RightKnee.x,
				 ScreenBones.Bone_RightKnee.y,
				 ScreenBones.Bone_RightFoot.x,
				 ScreenBones.Bone_RightFoot.y, Color, 2);
			 DrawLine(ScreenBones.Bone_RightFoot.x,
				 ScreenBones.Bone_RightFoot.y,
				 ScreenBones.Bone_RightTiptoe.x,
				 ScreenBones.Bone_RightTiptoe.y, Color, 2);
		 }
	 }



	 AxisAlignedBox GetCubeRectangle(float height)
	 {
		 AxisAlignedBox aabb;
		 aabb.Min = { -0.40000f, 0.000000f, -0.40000f };
		 aabb.Max = { 0.40000f, height, 0.40000f };
		 return aabb;
	 }

	 void DrawCubeRectangle(float height, D3DXVECTOR3 Position, float Yaw, D3DCOLOR Color, float Width=2.0f, float Alpha=1.0f)//方体
	 {
		 float cosY = (float)cos(Yaw);
		 float sinY = (float)sin(Yaw);
		 AxisAlignedBox Cube = GetCubeRectangle(height);
		 D3DXVECTOR3 fld = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Min.x * sinY, Cube.Min.y, Cube.Min.x * cosY + Cube.Min.z * sinY } +Position; // 0
		 D3DXVECTOR3 brt = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Max.x * sinY, Cube.Min.y, Cube.Max.x * cosY + Cube.Min.z * sinY } +Position; // 1
		 D3DXVECTOR3 bld = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Max.x * sinY, Cube.Min.y, Cube.Max.x * cosY + Cube.Max.z * sinY } +Position; // 2
		 D3DXVECTOR3 frt = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Min.x * sinY, Cube.Min.y, Cube.Min.x * cosY + Cube.Max.z * sinY } +Position; // 3
		 D3DXVECTOR3 frd = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Min.x * sinY, Cube.Max.y, Cube.Min.x * cosY + Cube.Max.z * sinY } +Position; // 4
		 D3DXVECTOR3 brb = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Min.x * sinY, Cube.Max.y, Cube.Min.x * cosY + Cube.Min.z * sinY } +Position; // 5
		 D3DXVECTOR3 blt = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Max.x * sinY, Cube.Max.y, Cube.Max.x * cosY + Cube.Min.z * sinY } +Position; // 6
		 D3DXVECTOR3 flt = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Max.x * sinY, Cube.Max.y, Cube.Max.x * cosY + Cube.Max.z * sinY } +Position; // 7


		 if (!_GameData.WorldToScreen(fld, fld) || !_GameData.WorldToScreen(brt, brt)
			 || !_GameData.WorldToScreen(bld, bld) || !_GameData.WorldToScreen(frt, frt)
			 || !_GameData.WorldToScreen(frd, frd) || !_GameData.WorldToScreen(brb, brb)
			 || !_GameData.WorldToScreen(blt, blt) || !_GameData.WorldToScreen(flt, flt))
			 return;
		 
		 DrawVectorLine(fld, brt, Color, Width, Alpha);
		 DrawVectorLine(brb, blt, Color, Width, Alpha);
		 DrawVectorLine(fld, brb, Color, Width, Alpha);
		 DrawVectorLine(brt, blt, Color, Width, Alpha);

		 DrawVectorLine(frt, bld, Color, Width, Alpha);
		 DrawVectorLine(frd, flt, Color, Width, Alpha);
		 DrawVectorLine(frt, frd, Color, Width, Alpha);
		 DrawVectorLine(bld, flt, Color, Width, Alpha);

		 DrawVectorLine(frt, fld, Color, Width, Alpha);
		 DrawVectorLine(frd, brb, Color, Width, Alpha);
		 DrawVectorLine(brt, bld, Color, Width, Alpha);
		 DrawVectorLine(blt, flt, Color, Width, Alpha);
	 }

	 void DrawHalfCubeRectangle(float height, D3DXVECTOR3 Position, float Yaw, D3DCOLOR Color, float Width = 2.0f, float Alpha = 1.0f)//断式方体
	 {
		 float cosY = (float)cos(Yaw);
		 float sinY = (float)sin(Yaw);
		 AxisAlignedBox Cube = GetCubeRectangle(height);
		 D3DXVECTOR3 fld = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Min.x * sinY, Cube.Min.y, Cube.Min.x * cosY + Cube.Min.z * sinY } +Position; // 0
		 D3DXVECTOR3 brt = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Max.x * sinY, Cube.Min.y, Cube.Max.x * cosY + Cube.Min.z * sinY } +Position; // 1
		 D3DXVECTOR3 bld = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Max.x * sinY, Cube.Min.y, Cube.Max.x * cosY + Cube.Max.z * sinY } +Position; // 2
		 D3DXVECTOR3 frt = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Min.x * sinY, Cube.Min.y, Cube.Min.x * cosY + Cube.Max.z * sinY } +Position; // 3
		 D3DXVECTOR3 frd = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Min.x * sinY, Cube.Max.y, Cube.Min.x * cosY + Cube.Max.z * sinY } +Position; // 4
		 D3DXVECTOR3 brb = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Min.x * sinY, Cube.Max.y, Cube.Min.x * cosY + Cube.Min.z * sinY } +Position; // 5
		 D3DXVECTOR3 blt = D3DXVECTOR3{ Cube.Min.z * cosY - Cube.Max.x * sinY, Cube.Max.y, Cube.Max.x * cosY + Cube.Min.z * sinY } +Position; // 6
		 D3DXVECTOR3 flt = D3DXVECTOR3{ Cube.Max.z * cosY - Cube.Max.x * sinY, Cube.Max.y, Cube.Max.x * cosY + Cube.Max.z * sinY } +Position; // 7


		 if (!_GameData.WorldToScreen(fld, fld) || !_GameData.WorldToScreen(brt, brt)
			 || !_GameData.WorldToScreen(bld, bld) || !_GameData.WorldToScreen(frt, frt)
			 || !_GameData.WorldToScreen(frd, frd) || !_GameData.WorldToScreen(brb, brb)
			 || !_GameData.WorldToScreen(blt, blt) || !_GameData.WorldToScreen(flt, flt))
			 return;

		
		 
		 DrawVectorLine(fld, _GameData.CalcVector3DivSub(brb, fld, 4) + fld, Color, Width, Alpha);
		 DrawVectorLine(fld, _GameData.CalcVector3DivSub(brt, fld,2.5f) + fld, Color, Width, Alpha);
		 DrawVectorLine(fld, _GameData.CalcVector3DivSub(frt, fld, 2.5f) + fld, Color, Width, Alpha);

		 DrawVectorLine(frt, _GameData.CalcVector3DivSub(frd, frt, 4) + frt, Color, Width, Alpha);
		 DrawVectorLine(frt, _GameData.CalcVector3DivSub(fld, frt, 2.5f) + frt, Color, Width, Alpha);
		 DrawVectorLine(frt, _GameData.CalcVector3DivSub(bld, frt, 2.5f) + frt, Color, Width, Alpha);

		 DrawVectorLine(bld, _GameData.CalcVector3DivSub(flt, bld, 4) + bld, Color, Width, Alpha);
		 DrawVectorLine(bld, _GameData.CalcVector3DivSub(frt, bld, 2.5f) + bld, Color, Width, Alpha);
		 DrawVectorLine(bld, _GameData.CalcVector3DivSub(brt, bld, 2.5f) + bld, Color, Width, Alpha);

		 DrawVectorLine(brt, _GameData.CalcVector3DivSub(blt, brt, 4) + brt, Color, Width, Alpha);
		 DrawVectorLine(brt, _GameData.CalcVector3DivSub(bld, brt, 2.5f) + brt, Color, Width, Alpha);
		 DrawVectorLine(brt, _GameData.CalcVector3DivSub(fld, brt, 2.5f) + brt, Color, Width, Alpha);
		 

		 DrawVectorLine(brb, _GameData.CalcVector3DivSub(fld, brb, 4) + brb, Color, Width, Alpha);
		 DrawVectorLine(brb, _GameData.CalcVector3DivSub(frd, brb, 2.5f) + brb, Color, Width, Alpha);
		 DrawVectorLine(brb, _GameData.CalcVector3DivSub(blt, brb, 2.5f) + brb, Color, Width, Alpha);

		 DrawVectorLine(blt, _GameData.CalcVector3DivSub(brt, blt, 4) + blt, Color, Width, Alpha);
		 DrawVectorLine(blt, _GameData.CalcVector3DivSub(brb, blt, 2.5f) + blt, Color, Width, Alpha);
		 DrawVectorLine(blt, _GameData.CalcVector3DivSub(flt, blt, 2.5f) + blt, Color, Width, Alpha);

		 DrawVectorLine(flt, _GameData.CalcVector3DivSub(bld, flt, 4) + flt, Color, Width, Alpha);
		 DrawVectorLine(flt, _GameData.CalcVector3DivSub(blt, flt, 2.5f) + flt, Color, Width, Alpha);
		 DrawVectorLine(flt, _GameData.CalcVector3DivSub(frd, flt, 2.5f) + flt, Color, Width, Alpha);

		 DrawVectorLine(frd, _GameData.CalcVector3DivSub(frt, frd, 4) + frd, Color, Width, Alpha);
		 DrawVectorLine(frd, _GameData.CalcVector3DivSub(flt, frd, 2.5f) + frd, Color, Width, Alpha);
		 DrawVectorLine(frd, _GameData.CalcVector3DivSub(brb, frd, 2.5f) + frd, Color, Width, Alpha);

	 }
};



class GameOffsets
{

public:
	
	INT64		GameOffset							= 0x01CBD628;
	INT64		GameViewOffset = 0x01853708;
	INT64		GameEnemyOffset = 0x01C27628;
	INT64		GameAimBotOffset = 0x01C34608;



	INT64		GameHealth = 0x188;
	INT64		GameMaxHealth = 0x184;
	INT64		GamePositionPointer = 0X170;
	INT64		Game_HeadPositionPointer = 0X180;

	vector <INT64> AimBotPointer = { 0x12c0, 0x980,0x1d08,0x28,0x8,0,0x1cc0 };
	vector <INT64> EnemyPointer = { 0xc60,0xe08,0x38,0x38,0x38,0x38,0x0,0x1a8,0x50,0x68,0x10,0xB0 };
	vector <INT64> ViewPointer = { 0x8, 0x30, 0xfe8,0x8e8,0x300,0x330,0x110 };
	
};

