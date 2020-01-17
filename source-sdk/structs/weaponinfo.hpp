#pragma once

class weapon_info_t {
public:
	std::uint8_t pad_0x0000[0x4]; //0x0000
	char* m_weapon_name; //0x0004 
	std::uint8_t pad_0x0008[0xC]; //0x0008
	std::uint32_t m_max_clip; //0x0014 
	std::uint8_t pad_0x0018[0x68]; //0x0018
	char* m_ammo_name; //0x0080 
	char* m_ammo_name_2; //0x0084 
	char* m_hud_name; //0x0088 
	char* m_weapon_id; //0x008C 
	std::uint8_t pad_0x0090[0x3C]; //0x0090
	std::uint32_t m_type; //0x00CC 
	std::uint32_t m_price; //0x00D0 
	std::uint32_t m_reward; //0x00D4 
	std::uint8_t pad_0x00D8[0x14]; //0x00D8
	std::uint8_t m_full_auto; //0x00EC 
	std::uint8_t pad_0x00ED[0x3]; //0x00ED
	std::uint32_t m_dmg; //0x00F0 
	float m_armor_ratio; //0x00F4 
	std::uint32_t m_bullets; //0x00F8 
	float m_penetration; //0x00FC 
	std::uint8_t pad_0x0100[0x8]; //0x0100
	float m_range; //0x0108 
	float m_range_modifier; //0x010C 
	std::uint8_t pad_0x0110[0x20]; //0x0110
	float m_max_speed; //0x0130 
	float m_max_speed_alt; //0x0134 
	std::uint8_t pad_0x0138[0x108]; //0x0138
};