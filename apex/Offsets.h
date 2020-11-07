#pragma once
#include "Includes.h"
#define NUM_ENT_ENTRIES			(1 << 12)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

#define m_view_render 0xcb011e0
#define m_view_matrix 0x1b3bd0

#define m_LocalPlayer 0x1dea658 
#define m_cl_entitylist 0x1883c38

#define m_entity_origin 0x014c 
#define m_entity_team_num 0x03f0
#define m_entity_bones 0x0ee0
#define m_player_shields 0x0170
#define m_player_health 0x03e0
#define m_player_max_health 0x0510
#define m_shieldHealth 0x0170 
#define m_shieldHealthMax 0x0174
#define m_player_bleedout_state 0x2590
enum BoneId {
	head = 8,
	neck = 7,
	Left_shoulder = 11,
	Right_shoulder = 35,
	chest = 5,
	Spine = 3,
	Left_hand = 15,
	Right_hand = 39,
	Left_arm = 41,
	Right_arm = 16,
	Left_knee = 58,
	Right_knee = 63,
	Left_foot = 65,
	Right_foot = 60,
	Left_ElbowRoll = 13,
	Right_ElbowRoll = 37,

};
enum  WeaponID {
	BARE_HANDS = 14,

	HAVOC = 64,
	LSTAR = 65,

	KRABER = 54,
	MASTIFF = 55,
	DEVOTION = 48,

	SENTINEL = 1,
	CHARGE_RIFLE = 66,
	LONGBOW = 49,
	TRIPLE_TAKE = 62,

	WINGMAN = 63,
	SPITFIRE = 61,
	PROWLER = 57,
	HEMLOK = 53,
	FLATLINE = 51,

	RE45 = 47,
	P2020 = 60,
	R301 = 0,
	R99 = 59,
	ALTERNATOR = 46,
	G7_SCOUT = 52,

	MOZAMBIQUE = 56,
	PEACEKEEPER = 58,
	EVA8_AUTO = 50,
};
enum  HelmetID {
	HELMET_LV0,
	HELMET_LV1,
	HELMET_LV2,
	HELMET_LV3,
	HELMET_LV4,
};
enum  ArmorID {
	BODY_ARMOR_LV0,
	BODY_ARMOR_LV1,
	BODY_ARMOR_LV2,
	BODY_ARMOR_LV3,
	BODY_ARMOR_LV4,
};
class AxisAlignedBox
{
public:
	D3DXVECTOR4 Min;
	D3DXVECTOR4 Max;
};