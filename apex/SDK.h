#pragma once
#include "kRpm.h"
#include "offsets.h"
#pragma warning(disable:4996)
MoaRpm rpm("Apex Legends", MoaRpm::MOA_MODE::KERNEL);
static int S_width = 1920, S_height = 1080;
static DWORD_PTR base = 0;
float Distance(D3DXVECTOR3 EntityPos, D3DXVECTOR3 PlayerPos);
double GetCrossDistance(double x1, double y1, double x2, double y2);
class Inputs {
private:
	static DWORD SendScanCode(WORD scan, BOOL up)
	{
		INPUT inp = { 0 };
		inp.type = INPUT_KEYBOARD;
		inp.ki.wScan = scan;
		inp.ki.dwFlags = KEYEVENTF_SCANCODE | (up ? KEYEVENTF_KEYUP : 0);
		return SendInput(1, &inp, sizeof(inp)) ? NO_ERROR : GetLastError();
	}
public:
	static bool mouse_move(int x, int y)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(input));
		return true;
	}
	static DWORD SendVirtualKey(UINT vk, BOOL up)
	{
		UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
		return scan ? SendScanCode(scan, up) : ERROR_NO_UNICODE_TRANSLATION;
	}
	bool static keydown(int key)
	{
		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}
};

class Player {
private:

public:
	Player() {}
	Player(DWORD_PTR _ptr) {
		this->ptr = _ptr;
		if (!rpm.IsValid(this->ptr)) return;
		unsigned long long pBoneArray = rpm.read<DWORD_PTR>(ptr + m_entity_bones);
		if (!rpm.IsValid(pBoneArray)) return;
		DWORD_PTR latestPrimaryWeapons = rpm.read<DWORD_PTR>(ptr + 0x1944) & 0xFFFF;
		DWORD_PTR listPrimaryWeapons = rpm.read<DWORD_PTR>(base + m_cl_entitylist + (latestPrimaryWeapons * 0x20));

		this->Flags = rpm.read<INT>(this->ptr + 0x0098);
		this->helmetType = rpm.read<INT>(this->ptr + 0x410c);
		this->armorType = rpm.read<INT>(this->ptr + 0x4110);
		this->weaponNameIndex = rpm.read<int>(listPrimaryWeapons + 0x16EC);
		this->Team = rpm.read<INT>(ptr + m_entity_team_num);
		this->Health = rpm.read<INT>(ptr + m_player_health);
		this->ShieldHealth = rpm.read<INT>(ptr + m_shieldHealth);
		this->ShieldHealthMax = rpm.read<INT>(ptr + m_shieldHealthMax);
		this->HealthMax = rpm.read<INT>(ptr + m_player_max_health);
		this->origin = rpm.read<D3DXVECTOR3>(ptr + m_entity_origin); 
		this->IsDowned = rpm.read<INT>(ptr + m_player_bleedout_state);

		Head.x = rpm.read<float>(pBoneArray + 0xCC + (BoneId::head * 0x30)) + origin.x;
		Head.y = rpm.read<float>(pBoneArray + 0xDC + (BoneId::head * 0x30)) + origin.y;
		Head.z = rpm.read<float>(pBoneArray + 0xEC + (BoneId::head * 0x30)) + origin.z;

	}
	DWORD_PTR ptr = 0x0;
	INT Flags = 0;
	INT helmetType = 0;
	INT armorType = 0;
	INT Health = 0;
	INT HealthMax = 0;
	INT ShieldHealth = 0;
	INT ShieldHealthMax = 0;
	INT Team = 0;
	D3DXVECTOR3 origin = D3DXVECTOR3();
	D3DXVECTOR3 Head = D3DXVECTOR3();
	FLOAT Distance = 0;
	bool IsDowned = 0;
	int weaponNameIndex = 0;
	bool GetEntityBonePosition(uint32_t BoneId, D3DXVECTOR3& Out)
	{
		unsigned long long pBoneArray = rpm.read<DWORD_PTR>(ptr + m_entity_bones);
		if (!rpm.IsValid(pBoneArray)) return false;

		Out.x = rpm.read<float>(pBoneArray + 0xCC + (BoneId * 0x30)) + origin.x;
		Out.y = rpm.read<float>(pBoneArray + 0xDC + (BoneId * 0x30)) + origin.y;
		Out.z = rpm.read<float>(pBoneArray + 0xEC + (BoneId * 0x30)) + origin.z;
		return true;
	}

	BOOL IsValid() {
		if ((this->origin.x != NULL) &&
			(this->Team != 0) &&
			(this->Health > NULL) &&
			(this->ptr != NULL)) return true;
		return false;
	}
};

class Manager {
private:
	D3DMATRIX  vMatrix;
public:
	Player LocalPlayer = NULL;
	std::vector<Player> Players = std::vector<Player>();
	Player ClosestVisibleEnemy = NULL;
	Player ClosestCrosshairEnemy = NULL;
	INT Total = 0;

	Manager(int width, int height) {
		S_width = width;
		S_height = height;
		DWORD_PTR  ViewRender = rpm.read<DWORD_PTR>(base + m_view_render);
		if (!rpm.IsValid(ViewRender)) return;
		DWORD_PTR View_matrix = rpm.read<DWORD_PTR>(ViewRender + m_view_matrix);
		if (!rpm.IsValid(View_matrix)) return;
		vMatrix = rpm.read<D3DMATRIX>(View_matrix);

		this->LocalPlayer = Player(rpm.read<DWORD_PTR>(base + m_LocalPlayer));
		for (size_t i = 0; i < 64; i++) {
			Player player(rpm.read<DWORD_PTR>(base + m_cl_entitylist + (i * 0x20)));

			if (player.ptr != 0);// break;
			if (player.IsValid() && player.ptr != this->LocalPlayer.ptr) {
				player.Distance = Distance(this->LocalPlayer.origin,player.origin);
				this->Players.push_back(player);
			}
		}

		float closestcrosshair = FLT_MAX;
		for (size_t i = 0; i < this->Players.size(); i++) {
			if (!this->LocalPlayer.IsDowned) {
				D3DXVECTOR3 out;
				if (WorldToScreen(this->Players[i].origin, out)) {
					float crossdis = GetCrossDistance(out.x, out.y, S_width / 2, S_height / 2);
					if ((crossdis < closestcrosshair)) {
						if (this->Players[i].Team != this->LocalPlayer.Team && this->Players[i].Distance < 125) {
							if (this->Players[i].IsDowned) {
								if (settings::aimknocked)
									ClosestCrosshairEnemy = this->Players[i];
							}
							else {
								ClosestCrosshairEnemy = this->Players[i];
							}
							closestcrosshair = crossdis;
						}
					}
				}
			}
		}
		float closestdis = FLT_MAX;
		for (int i = 0; i < this->Players.size(); i++) {
				if ((this->Players[i].Distance < closestdis)) {
					if (this->Players[i].Team != this->LocalPlayer.Team) {
						ClosestVisibleEnemy = this->Players[i];
						closestdis = this->Players[i].Distance;
					}
				}
		}

		for (int i = 0; i < this->Players.size(); i++) {
			Total = this->Players.size();

		}
	}

	bool WorldToScreen(D3DXVECTOR3& vIn, D3DXVECTOR3& vOut)
	{

		vOut.x = vMatrix.m[0][0] * vIn.x + vMatrix.m[0][1] * vIn.y + vMatrix.m[0][2] * vIn.z + vMatrix.m[0][3];
		vOut.y = vMatrix.m[1][0] * vIn.x + vMatrix.m[1][1] * vIn.y + vMatrix.m[1][2] * vIn.z + vMatrix.m[1][3];

		float w = vMatrix.m[3][0] * vIn.x + vMatrix.m[3][1] * vIn.y + vMatrix.m[3][2] * vIn.z + vMatrix.m[3][3];

		if (w < 0.65f)
			return false;

		float invw = 1.0f / w;

		vOut.x *= invw;
		vOut.y *= invw;

		float x = S_width / 2;
		float y = S_height / 2;

		x += 0.5 * vOut.x * S_width + 0.5;
		y -= 0.5 * vOut.y * S_height + 0.5;

		vOut.x = x;
		vOut.y = y;

		return true;
	}


};
float Distance3D(D3DXVECTOR3  v1, D3DXVECTOR3 v2)
{
	float x_d = (v2.x - v1.x);
	float y_d = (v2.y - v1.y);
	float z_d = (v2.z - v1.z);
	return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
}
double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
void AimAtPosV2(float x, float y, float speed,bool smooth)
{
	int ScreenCenterX = S_width / 2, ScreenCenterY = S_height / 2;

	float AimSpeed = (float)speed;
	float TargetX = 0;
	float TargetY = 0;

	//X Axis
	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			TargetX += settings::aimlock;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			TargetX -= settings::aimlock;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	//Y Axis

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			TargetY += settings::aimlock;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			TargetY -= settings::aimlock;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	if (!smooth)
	{
		Inputs::mouse_move((int)TargetX, (int)(TargetY));
		return;
	}

	TargetX /= 10;
	TargetY /= 10;

	if (abs(TargetX) < 1)
	{
		if (TargetX > 0)
		{
			TargetX = 1;
		}
		if (TargetX < 0)
		{
			TargetX = -1;
		}
	}
	if (abs(TargetY) < 1)
	{
		if (TargetY > 0)
		{
			TargetY = 1;
		}
		if (TargetY < 0)
		{
			TargetY = -1;
		}
	}
	Inputs::mouse_move((int)TargetX, (int)(TargetY));

}
D3DXVECTOR3 CalcSoldierFuturePos(D3DXVECTOR3 InVec)
{
	Manager gm(S_width, S_height);
	D3DXVECTOR3 NewPos, OutPos;
	if (gm.WorldToScreen(InVec, NewPos)) {
		OutPos.x = NewPos.x;
		OutPos.y = NewPos.y;
		OutPos.z = NewPos.z;
	}
	else
	{
		OutPos.x = 0;
		OutPos.y = 0;
		OutPos.z = 0;
	}
	return OutPos;
}
float Distance(D3DXVECTOR3 EntityPos, D3DXVECTOR3 PlayerPos)
{
	float Dist = sqrt((PlayerPos.x - EntityPos.x) * (PlayerPos.x - EntityPos.x)
		+ (PlayerPos.y - EntityPos.y) * (PlayerPos.y - EntityPos.y)
		+ (PlayerPos.z - EntityPos.z) * (PlayerPos.z - EntityPos.z));
	return Dist * 0.01905f;
}
char* GetHelmetType(Player pPlayer)
{
	int HelmetId = pPlayer.helmetType;
	switch (HelmetId)
	{
		case (int)(HelmetID::HELMET_LV0) :
			return "Helmet LV0";
			break;
		case (int)(HelmetID::HELMET_LV1) :
			return "Helmet LV1";
			break;
		case (int)(HelmetID::HELMET_LV2) :
			return "Helmet LV2";
			break;
		case (int)(HelmetID::HELMET_LV3) :
			return "Helmet LV3";
			break;
		case (int)(HelmetID::HELMET_LV4) :
			return "Helmet LV4";
			break;
	}
	return "NULL";
}
char* GetArmorType(Player pPlayer)
{
	int ArmorId = pPlayer.armorType;
	switch (ArmorId)
	{
		case (int)(ArmorID::BODY_ARMOR_LV0) :
			return "Armor LV0";
			break;
		case (int)(ArmorID::BODY_ARMOR_LV1) :
			return "Armor LV1";
			break;
		case (int)(ArmorID::BODY_ARMOR_LV2) :
			return "Armor LV2";
			break;
		case (int)(ArmorID::BODY_ARMOR_LV3) :
			return "Armor LV3";
			break;
		case (int)(ArmorID::BODY_ARMOR_LV4) :
			return "Armor LV4";
			break;
	}
	return "NULL";
}

char* GetGunName(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	switch (WeaponId)
	{
	case (int)(WeaponID::BARE_HANDS):
		return "HANDS";
		break;
	case (int)WeaponID::HAVOC:
		return "HAVOC";
		break;
	case (int)WeaponID::LSTAR:
		return "LSTAR";
		break;
	case (int)WeaponID::KRABER:
		return "KRABER";
		break;
	case (int)WeaponID::MASTIFF:
		return "MASTIFF";
		break;
	case (int)WeaponID::DEVOTION:
		return "DEVOTION";
		break;
	case (int)WeaponID::SENTINEL:
		return "SENTINEL";
		break;
	case (int)WeaponID::CHARGE_RIFLE:
		return "CHARGE_RIFLE";
		break;
	case (int)WeaponID::LONGBOW:
		return "LONGBOW";
		break;
	case (int)WeaponID::TRIPLE_TAKE:
		return "TRIPLE_TAKE";
		break;
	case (int)WeaponID::WINGMAN:
		return "WINGMAN";
		break;
	case (int)WeaponID::SPITFIRE:
		return "SPITFIRE";
		break;
	case (int)WeaponID::PROWLER:
		return "PROWLER";
		break;
	case (int)WeaponID::HEMLOK:
		return "HEMLOK";
		break;
	case (int)WeaponID::FLATLINE:
		return "FLATLINE";
		break;
	case (int)WeaponID::RE45:
		return "RE45";
		break;
	case (int)WeaponID::P2020:
		return "P2020";
		break;
	case (int)WeaponID::R301:
		return "R301";
		break;
	case (int)WeaponID::R99:
		return "R99";
		break;
	case (int)WeaponID::ALTERNATOR:
		return "ALTERNATOR";
		break;
	case (int)WeaponID::G7_SCOUT:
		return "G7_SCOUT";
		break;
	case (int)WeaponID::MOZAMBIQUE:
		return "MOZAMBIQUE";
		break;
	case (int)WeaponID::PEACEKEEPER:
		return "PEACEKEEPER";
		break;
	case (int)WeaponID::EVA8_AUTO:
		return "EVA8_AUTO";
		break;
	
	}
	return "NULL";
}
char* GetGunType(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	switch (WeaponId)
	{
		case (int)WeaponID::BARE_HANDS:
			return "HANDS";
			break;
		case (int)WeaponID::KRABER:
		case (int)WeaponID::SENTINEL:
		case (int)WeaponID::LONGBOW:
		case (int)WeaponID::TRIPLE_TAKE:
		case (int)WeaponID::CHARGE_RIFLE:
			return "SNIPER";
			break;
		case (int)WeaponID::HEMLOK:
		case (int)WeaponID::FLATLINE:
		case (int)WeaponID::G7_SCOUT:
		case (int)WeaponID::R301:
		case (int)WeaponID::HAVOC:
			return "AR";
			break;
		case (int)WeaponID::DEVOTION:
		case (int)WeaponID::SPITFIRE:
		case (int)WeaponID::LSTAR:
			return "LMG";
			break;
		case (int)WeaponID::PROWLER:
		case (int)WeaponID::R99:
		case (int)WeaponID::ALTERNATOR:
			return "SMG";
			break;
		case (int)WeaponID::MASTIFF:
		case (int)WeaponID::EVA8_AUTO:
		case (int)WeaponID::PEACEKEEPER:
		case (int)WeaponID::MOZAMBIQUE:
			return "SHOTGUN";
			break;
		case (int)WeaponID::WINGMAN:
		case (int)WeaponID::RE45:
		case (int)WeaponID::P2020:
			return "PISTOL";
			break;

	}
	return "NULL";
}
bool IsPistol(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if (WeaponId == (int)WeaponID::RE45
		|| WeaponId == (int)WeaponID::P2020
		|| WeaponId == (int)WeaponID::WINGMAN)
		return true;
	else
		return false;
}
bool IsSniper(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if (WeaponId == (int)WeaponID::KRABER
		||WeaponId == (int)WeaponID::SENTINEL 
		||WeaponId == (int)WeaponID::LONGBOW 
		||WeaponId == (int)WeaponID::TRIPLE_TAKE
		||WeaponId == (int)WeaponID::CHARGE_RIFLE)
		return true;
	else
		return false;
}

bool IsLMG(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if (WeaponId == (int)WeaponID::DEVOTION || WeaponId == (int)WeaponID::SPITFIRE || WeaponId == (int)WeaponID::LSTAR)
		return true;
	else
		return false;
}
bool IsSmg(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if ( WeaponId == (int)WeaponID::PROWLER
		|| WeaponId == (int)WeaponID::R99
		|| WeaponId == (int)WeaponID::ALTERNATOR)
		return true;
	else
		return false;
}
bool IsShotgun(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if (WeaponId == (int)WeaponID::MASTIFF
		|| WeaponId == (int)WeaponID::PEACEKEEPER 
		|| WeaponId == (int)WeaponID::EVA8_AUTO
		|| WeaponId == (int)WeaponID::MOZAMBIQUE)
		return true;
	else
		return false;
}
bool IsAR(Player pPlayer)
{
	int WeaponId = pPlayer.weaponNameIndex;
	if (WeaponId == NULL) return false;

	if (WeaponId == (int)WeaponID::FLATLINE
		|| WeaponId == (int)WeaponID::G7_SCOUT
		|| WeaponId == (int)WeaponID::R301
		|| WeaponId == (int)WeaponID::HAVOC
		|| WeaponId == (int)WeaponID::HEMLOK)	
		return true;
	else
		return false;
}