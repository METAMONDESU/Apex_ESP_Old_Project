#include "Menu.h"
#include "Settings.h"
#include "SDK.h"
#define checkSetting(teamval, enemyval) (((gm.LocalPlayer.Team == player.Team) && teamval) || ((gm.LocalPlayer.Team != player.Team) && enemyval))
LPCSTR FilePath = "./Settings.ini";
c_menu_framework* menu_framework = new c_menu_framework;
static class Text
{
public:
	std::string text;
	int side;
	int size;
	D3DXCOLOR color;
	Text(std::string text, int side, int size, D3DXCOLOR color) : text(text), side(side), size(size), color(color) {}
};
void CoutOffset(LPCSTR name, DWORD offset)
{
	std::cout << "[" << name << "]" << " = 0x" << std::uppercase << std::hex << offset << std::endl;
}
void Coutfloat(LPCSTR name, float option)
{
	std::cout << "[" << name << "]" << " = " << std::dec << option << std::endl;
}

void Coutbool(LPCSTR name, int boolean)
{
	if (boolean == 1)
		std::cout << "[" << name << "]" << " = " << "ON" << std::endl;
	else
		std::cout << "[" << name << "]" << " = " << "OFF" << std::endl;
}
float ReadConfig(LPCSTR Option, bool Float)
{
	if (Float)
	{
		char cfg[10];
		GetPrivateProfileString(TEXT("Config"), Option, TEXT("0"), cfg, 10, FilePath);
		float opt = atof(cfg);
		return opt;
	}
	else
		return GetPrivateProfileInt(TEXT("Config"), Option, NULL, FilePath);
}
DWORD ReadInt(LPCTSTR Offsetname)
{
	DWORD Offset = GetPrivateProfileInt(TEXT("Offsets"), Offsetname, NULL, FilePath);
	CoutOffset(Offsetname, Offset);
	return Offset;
}
void ReadSettings()
{
	settings::aimbot = ReadConfig(TEXT("Active"), false);
	settings::autolock = ReadConfig(TEXT("AutoLock"), false);
	settings::aimknocked = ReadConfig(TEXT("AimKnocket"), false);
	settings::aimbothitbox = ReadConfig(TEXT("HitBox"), true);
	settings::aimspeed = ReadConfig(TEXT("AimSpeed"), true);
	settings::aimbotfov = ReadConfig(TEXT("Fov"), true);
	settings::aimlock = ReadConfig(TEXT("aimlock"), true);
	settings::aimspot = ReadConfig(TEXT("AimSpot"), false);
	settings::aimspotdis = ReadConfig(TEXT("AimSpotDis"), true);
	settings::aimspotfov = ReadConfig(TEXT("AimSpotFov"), true);
	settings::aimspotspeed = ReadConfig(TEXT("AimSpotSpeed"), true);

	settings::aimbotdrawfov = ReadConfig(TEXT("DrawFov"), false);
	settings::aimbotdrawhitbox = ReadConfig(TEXT("DrawHitBox"), false);

	settings::teamBoxes = ReadConfig(TEXT("TeamBox"), false);
	settings::teamDistance = ReadConfig(TEXT("TeamDistance"), false);
	settings::teamSnaplines = ReadConfig(TEXT("TeamSnaplines"), false);
	settings::teamHealth = ReadConfig(TEXT("TeamHealth"), false);
	settings::teamShieldHealth = ReadConfig(TEXT("ShieldHealth"), false);
	settings::teamFilledbox = ReadConfig(TEXT("TeamFilledbox"), false);
	settings::teamKnocked = ReadConfig(TEXT("TeamKnocked"), false);

	settings::enemyBoxes = ReadConfig(TEXT("EnemyBoxes"), false);
	settings::enemyDistance = ReadConfig(TEXT("EnemyDistance"), false);
	settings::enemySnaplines = ReadConfig(TEXT("EnemySnaplines"), false);
	settings::enemyHealth = ReadConfig(TEXT("EnemyHealth"), false);
	settings::enemyShieldHealth = ReadConfig(TEXT("EnemyShieldHealth"), false);
	settings::enemyFilledbox = ReadConfig(TEXT("EnemyFilledbox"), false);
	settings::enemyKnocked = ReadConfig(TEXT("EnemyKnocked"), false);

	settings::c_enable = ReadConfig(TEXT("CrosshairEnable"), false);
	settings::crosshair = ReadConfig(TEXT("CrosshairStyle"), true);
	settings::size = ReadConfig(TEXT("Size"), true);
	settings::stroke = ReadConfig(TEXT("Stroke"), true);

	settings::boxtype = ReadConfig(TEXT("BoxType"), true);
	settings::healthbartype = ReadConfig(TEXT("HealthbarType"), true);
	settings::playersdis = ReadConfig(TEXT("PlayersDis"), true);

	settings::menux = ReadConfig(TEXT("MenuX"), true);
	settings::menuy = ReadConfig(TEXT("MenuY"), true);
	std::cout <<  std::endl << std::endl;
	std::cout << "AIMBOT" << std::endl << std::endl;
	Coutbool(TEXT("Active"), settings::aimbot);
	Coutbool(TEXT("AutoLock"), settings::autolock);
	Coutbool(TEXT("AimKnocket"), settings::aimknocked);
	Coutfloat(TEXT("HitBox"), settings::aimbothitbox);
	Coutfloat(TEXT("AimSpeed"), settings::aimspeed);
	Coutfloat(TEXT("Fov"), settings::aimbotfov);
	Coutfloat(TEXT("Lock"), settings::aimlock);
	Coutbool(TEXT("AimSpot"), settings::aimspot);
	Coutfloat(TEXT("AimSpotDis"), settings::aimspotdis);
	Coutfloat(TEXT("AimSpotFov"), settings::aimspotfov);
	Coutfloat(TEXT("AimSpotSpeed"), settings::aimspotspeed);
	Coutbool(TEXT("DrawFov"), settings::aimbotdrawfov);
	Coutbool(TEXT("DrawHitBox"), settings::aimbotdrawhitbox);
	std::cout << std::endl << std::endl;
	std::cout << "TEAM ESP" << std::endl << std::endl;
	Coutbool(TEXT("Box"), settings::teamBoxes);
	Coutbool(TEXT("Distance"), settings::teamDistance);
	Coutbool(TEXT("Snaplines"), settings::teamSnaplines);
	Coutbool(TEXT("Health"), settings::teamHealth);
	Coutbool(TEXT("ShieldHealth"), settings::teamShieldHealth);
	Coutbool(TEXT("Filledbox"), settings::teamFilledbox);
	Coutbool(TEXT("Knocked"), settings::teamKnocked);

	std::cout << std::endl << std::endl;
	std::cout << "ENEMY ESP" << std::endl << std::endl;
	Coutbool(TEXT("Box"), settings::enemyBoxes);
	Coutbool(TEXT("Distance"), settings::enemyDistance);
	Coutbool(TEXT("Snaplines"), settings::enemySnaplines);
	Coutbool(TEXT("Health"), settings::enemyHealth);
	Coutbool(TEXT("ShieldHealth"), settings::enemyShieldHealth);
	Coutbool(TEXT("Filledbox"), settings::enemyFilledbox);
	Coutbool(TEXT("Filledbox"), settings::enemyKnocked);
	std::cout << std::endl << std::endl;
	std::cout << "CROSSHAIR" << std::endl << std::endl;
	Coutbool(TEXT("CrosshairEnable"), settings::c_enable);
	Coutfloat(TEXT("CrosshairStyle"), settings::crosshair);
	Coutfloat(TEXT("Size"), settings::size);
	Coutfloat(TEXT("stroke"), settings::stroke);
	std::cout << std::endl << std::endl;
	std::cout << "SETTINGS ESP" << std::endl << std::endl;
	Coutfloat(TEXT("BoxType"), settings::boxtype);
	Coutfloat(TEXT("HealthbarType"), settings::healthbartype);
	Coutfloat(TEXT("PlayersDis"), settings::healthbartype);
	std::cout << std::endl << std::endl;
	std::cout << "MENU" << std::endl << std::endl;
	Coutfloat(TEXT("MenuX"), settings::menux);
	Coutfloat(TEXT("MenuY"), settings::menuy);
	std::cout << std::endl << std::endl;
}	


void Crosshair()
{
	switch (settings::crosshair)
	{
	case Renderer::CrosshairStyle::Dot:
		Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Dot, S_width / 2, S_height / 2, settings::size, settings::stroke, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	case Renderer::CrosshairStyle::Plus:
		Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Plus, S_width / 2, S_height / 2, settings::size, settings::stroke, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	case Renderer::CrosshairStyle::Cross:
		Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Cross, S_width / 2, S_height / 2, settings::size, settings::stroke, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	case Renderer::CrosshairStyle::Gap:
		Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Gap, S_width / 2, S_height / 2, settings::size, settings::stroke, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	case Renderer::CrosshairStyle::Diagonal:
		Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Diagonal, S_width / 2, S_height / 2, settings::size, settings::stroke, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		break;
	default:
		break;
	}
}

void drawLoop(int width, int height)
{
	float aimspeed = settings::aimspeed;
	float aimfov = settings::aimbotfov;
	D3DXCOLOR aimcolor = D3DXCOLOR(1,1,1,1);

	menu_framework->do_menu_controls();
	if(settings::c_enable)
	Crosshair();

	Manager gm(width, height);
	DrawString("Players " + std::to_string((int)gm.Total), 18, 10 , 10, 1.f, 0.f, 0.f, 1.f);

	for (Player& player : gm.Players) {
		D3DXVECTOR3 footLocation = player.origin;
		D3DXVECTOR3 headLocation = player.Head;
		D3DXVECTOR3 oheadLocation;
		D3DXVECTOR3 ofootLocation;
		D3DXCOLOR PlayerBox = D3DXCOLOR();
		D3DXCOLOR PlayerLine = D3DXCOLOR();
		D3DXCOLOR PlayerFillBox = D3DXCOLOR();
		D3DXCOLOR PlayerKnocked = D3DXCOLOR();

		if (player.Team == gm.LocalPlayer.Team) {
			PlayerBox = settings::FriendBox;
			PlayerLine = settings::FriendLine;
			PlayerFillBox = settings::FriendFillbox;
			PlayerKnocked = settings::FriendKnocked;
		}
		else {
			if (player.Distance < 80) {

				PlayerBox = settings::Closed;
				PlayerLine = settings::Closed;
				PlayerFillBox = settings::Closed;

			}
			else {
				PlayerBox = settings::EnemyBOX;
				PlayerLine = settings::EnemyLine;
				PlayerFillBox = settings::EnemyFillbox;
				PlayerKnocked = settings::EnemyKnocked;
			}
		}

		if (player.Team != gm.LocalPlayer.Team) {
			
			if (gm.ClosestVisibleEnemy.Distance < 80)
			{
				DrawBox((S_width / 2) - 30, 170, 220, 50, 1, 0, 0, 0, 0.5, true);
				DrawString("Enemy Closed " + std::to_string((int)gm.ClosestVisibleEnemy.Distance) + "m", 20, S_width / 2, 180, 1.f, 0.f, 0.f, 1.f);
			}
			if (settings::aimspot) {
				if (gm.ClosestVisibleEnemy.Distance < settings::aimspotdis)
				{
					aimspeed = settings::aimspotspeed;
					aimfov = settings::aimspotfov;
					aimcolor = D3DXCOLOR(1, 0, 0, 1);
				}
				else {
					aimspeed = settings::aimspeed;
					aimfov = settings::aimbotfov;
					aimcolor = D3DXCOLOR(1, 1, 1, 1);
				}
			}

		}
		if checkSetting(settings::teamKnocked, settings::enemyKnocked) {
			if (player.IsDowned)
			{
				PlayerBox = PlayerKnocked;
				PlayerLine = PlayerKnocked;
				PlayerFillBox = PlayerKnocked;
			}

		}
		if (player.Flags == 41) headLocation.y += 1.6;
		if (player.Flags == 43) headLocation.y += 1;

		if ((gm.WorldToScreen(footLocation, ofootLocation) && (gm.WorldToScreen(headLocation, oheadLocation)))) {
			float w2sHeight = Distance3D(ofootLocation, oheadLocation);
			float w2sWidth = w2sHeight;
			float heightoffset = Distance3D(ofootLocation, oheadLocation);
			float H = w2sHeight;
			float W = H / 2;
			float X = oheadLocation.x - W / 2;
			if (player.Flags == 41) w2sWidth /= 2;
			if (player.Flags == 43) w2sWidth /= 1.5;
			std::vector< Text > texts;
			if (player.Distance > settings::playersdis) continue;

			if checkSetting(settings::teamSnaplines, settings::enemySnaplines) {
				DrawLine(S_width / 2, 0, oheadLocation.x, oheadLocation.y, 1, pasteColorA(PlayerLine));
			}
			if checkSetting(settings::teamBoxes, settings::enemyBoxes) {
				if (settings::boxtype == 0)
				{
					if (!settings::esp_Outline)
						DrawBox(X, oheadLocation.y, W, H, 2.f, pasteColorA(PlayerBox), false);
					else if (settings::esp_Outline)
						Renderer::GetInstance()->DrawOutlineBox(X, oheadLocation.y, W, H, PlayerBox, 2.f);

				}
				else
				{
					if (!settings::esp_Outline)
						Renderer::GetInstance()->DrawCoalBox(X, oheadLocation.y, W, H, PlayerBox, 2.f);
					else if (settings::esp_Outline)
						Renderer::GetInstance()->DrawOutlineCoalBox(X, oheadLocation.y, W, H, PlayerBox, 2.f);

				}
			}
			if checkSetting(settings::teamFilledbox, settings::enemyFilledbox) {
				DrawBox(X, oheadLocation.y, W, H, 1, pasteColor(PlayerFillBox), 0.1, true);

			}
			if checkSetting(settings::teamHealth, settings::enemyHealth) {
				if(settings::healthbartype == 0)
					Renderer::GetInstance()->DrawHorBar(X, oheadLocation.y - 6, W, 4, (int)player.Health, (int)player.HealthMax, D3DXCOLOR(.48f, .98f, 0.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				else if(settings::healthbartype == 1)
					Renderer::GetInstance()->DrawVerBar(X - 6, oheadLocation.y, 4, H, (int)player.Health, (int)player.HealthMax, D3DXCOLOR(.48f, .98f, 0.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				else
					texts.push_back(Text("Health "+std::to_string((int)player.Health) + "%", 2, 11, D3DXCOLOR(.48f, .98f, 0.f, 1.f)));

			}
			if checkSetting(settings::teamShieldHealth, settings::enemyShieldHealth) {
				if (settings::shildbartype == 0) {
					if (player.ShieldHealth > 0)
						Renderer::GetInstance()->DrawHorBar(X, ofootLocation.y + 6, W, 4, player.ShieldHealth, player.ShieldHealthMax, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				}
				else if (settings::shildbartype == 1) {
					if (player.ShieldHealth > 0)
						Renderer::GetInstance()->DrawVerBar(X + W + 1, oheadLocation.y, 4, H, player.ShieldHealth, player.ShieldHealthMax,D3DXCOLOR(1.f, 1.f, 1.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				}
				else {
					if (player.ShieldHealth > 0)
						texts.push_back(Text("Shield " + std::to_string((int)player.ShieldHealth) + "%", 2, 11, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)));
				}
			}
			if checkSetting(settings::teamInfo, settings::enemyInfo) {
				
				std::string status = "";
				D3DXCOLOR CHelmet = D3DXCOLOR(1, 1, 1, 1);
				D3DXCOLOR CArmor = D3DXCOLOR(1, 1, 1, 1);
				if (player.Flags == 49 || player.Flags == 0x4b)
					status = "Aiming";

				if (player.helmetType == 0)
					CHelmet = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				else if(player.helmetType == 1)
					CHelmet = D3DXCOLOR(0.87f, 0.87f, 0.87f, 1.f);
				else if (player.helmetType == 2)
					CHelmet = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
				else if (player.helmetType == 3)
					CHelmet = D3DXCOLOR(0.65f, 0.03f, 0.83f, 1.f);
				else
					CHelmet = D3DXCOLOR(1.0f, 1.0f, 0.f, 1.f);

				if (player.armorType == 0)
					CArmor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				else if (player.armorType == 1)
					CArmor = D3DXCOLOR(0.87f, 0.87f, 0.87f, 1.f);
				else if (player.armorType == 2)
					CArmor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
				else if (player.armorType == 3)
					CArmor = D3DXCOLOR(0.65f, 0.03f, 0.83f, 1.f);
				else
					CArmor = D3DXCOLOR(1.0f, 1.0f, 0.f, 1.f);


				texts.push_back(Text(GetHelmetType(player), 2, 11, CHelmet));
				texts.push_back(Text(GetArmorType(player), 2, 11, CArmor));
				texts.push_back(Text(status, 2, 11, D3DXCOLOR(1.f, 0.f, 0.f, 1.f)));

			}
			if checkSetting(settings::teamDistance, settings::enemyDistance) {

				texts.push_back(Text(std::to_string((int)player.Distance) + "m", 1, 9, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)));
			}
			if checkSetting(settings::teamWeaponName, settings::enemyWeaponName) {
				if(settings::PlayerWeapontype ==0)
				texts.push_back(Text(GetGunName(player), 1, 9, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)));
				else
				texts.push_back(Text(GetGunType(player), 1, 9, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)));
			}
			int middle[3] = { ofootLocation.y,ofootLocation.y, ofootLocation.x - ofootLocation.y };
			int Top[3] = { oheadLocation.y,oheadLocation.y, oheadLocation.x - oheadLocation.y };
			for (auto text : texts)
			{
				int fontsize = text.size;
				float offset = (text.text.length() * fontsize) / 5;
				switch (text.side)
				{
				case 0:
					DrawString(text.text.c_str(), text.size, oheadLocation.x - offset, Top[1] - (heightoffset / 4) - fontsize, text.color.r, text.color.g, text.color.b);
					Top[1] -= fontsize;
					break;
				case 1:
					DrawString(text.text.c_str(), text.size, ofootLocation.x - offset, middle[1] + fontsize, text.color.r, text.color.g, text.color.b);
					middle[1] += fontsize;
					break;
				case 2:
					DrawString(text.text.c_str(), text.size, oheadLocation.x + (heightoffset / 4) + fontsize, Top[1] - fontsize, text.color.r, text.color.g, text.color.b);
					Top[1] += fontsize;
					break;
				}
			}

		}
	}
	if ((Inputs::keydown(VK_LBUTTON) || settings::autolock) && settings::aimbot)
	{

		D3DXVECTOR3 BONE = D3DXVECTOR3();
		std::vector<int> HitBoxesToScan{};
		switch (settings::aimbothitbox)
		{
		case 0:
			HitBoxesToScan.push_back(BoneId::head);
			break;
		case 1:
			HitBoxesToScan.push_back(BoneId::neck);
			break;
		case 2:
			HitBoxesToScan.push_back(BoneId::chest);
			break;
		default:
			break;
		}
		for (auto hitbox : HitBoxesToScan)
		{
			if (hitbox != -1)
			{
				if (gm.ClosestCrosshairEnemy.GetEntityBonePosition(hitbox, BONE)) {

					D3DXVECTOR3 Hitbox = CalcSoldierFuturePos(D3DXVECTOR3(BONE.x, BONE.y, BONE.z));
					if (GetCrossDistance(Hitbox.x, Hitbox.y, S_width / 2, S_height / 2) <= aimfov * 8)
					{
						auto roundPos = D3DXVECTOR2((float)round(Hitbox.x), (float)round(Hitbox.y));
						AimAtPosV2(roundPos.x, roundPos.y, aimspeed, false);
						if (settings::aimbotdrawhitbox)
							DrawCircle(roundPos.x, roundPos.y, 3, 3, 1.f, 0.f, 0.f, 1.f, true);

					}
				}
			}
		}
	}
	if (settings::aimbotdrawfov)
		DrawCircle(S_width / 2, S_height / 2, aimfov * 8, 1, aimcolor.r, aimcolor.g, aimcolor.b, aimcolor.a, false);

}
int main()
{
	const char* filename = "./APEX_BaseAddress.ini";
	std::ifstream infile(filename, std::fstream::in);
	DWORD_PTR a;
	infile >> std::hex >> a;
	base = a;
	std::cout << "Apex Legends ESP by Zakaria" << std::endl;
	std::cout << "Load Config" << std::endl;
	ReadSettings();
	std::cout << "Initializing Overlay" << std::endl;
	DirectOverlaySetup(drawLoop, FindWindow(NULL, "Apex Legends"));
	std::cout << std::endl << std::endl;
	std::cout << "Base Address = " << std::hex << base << std::endl;

	getchar();
}