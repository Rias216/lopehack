#pragma once
#include "../common_includes.hpp"

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"

namespace csgo {
	extern player_t* local_player;
	extern bool sendpacket;
	namespace tick_abuse
	{
		extern int m_nTickbaseShift;
		extern bool bInSendMove;
		extern int32_t nSinceUse;
		extern bool bFirstSendMovePack;
	}
	namespace desync_chams
	{
		extern bool m_should_update_fake;
		extern std::array< animation_layer, 13 > m_fake_layers;
		extern std::array< float, 20 > m_fake_poses;
		extern anim_state* m_fake_state;
		extern float m_fake_rotation;
		extern bool init_fake_anim;
		extern float m_fake_spawntime;
		extern float m_fake_delta;
		extern	matrix_t m_fake_matrix[128];
		extern	matrix_t m_fake_position_matrix[128];
		extern	bool m_got_fake_matrix;
		extern	float m_real_yaw_ang;
	}
}