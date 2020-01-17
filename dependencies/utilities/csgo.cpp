#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	bool sendpacket = false;
	namespace fonts {
		unsigned long watermark_font;
		unsigned long name_font;
	}
	namespace tick_abuse
	{
		int m_nTickbaseShift;
		bool bInSendMove;
		int32_t nSinceUse;
		bool bFirstSendMovePack;
	}
	namespace desync_chams
	{
		bool m_should_update_fake = false;
		std::array< animation_layer, 13 > m_fake_layers;
		std::array< float, 20 > m_fake_poses;
		anim_state* m_fake_state = nullptr;
		float m_fake_rotation = 0.f;
		bool init_fake_anim = false;
		float m_fake_spawntime = 0.f;
		float m_fake_delta = 0.f;
		matrix_t m_fake_matrix[128];
		matrix_t m_fake_position_matrix[128];
		bool m_got_fake_matrix = false;
		float m_real_yaw_ang = 0.f;
	}
}