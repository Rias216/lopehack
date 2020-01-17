#pragma once
#include "../../dependencies/common_includes.hpp"

namespace misc {

	namespace extra {
		void thirdperson();
		
	};

	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void auto_strafe(c_usercmd* cmd);
	};
}

namespace visuals {

	namespace visual_main {
		void esp_render();
	}


	namespace basic {
		void esp_name(player_t* player);
		void esp_weapon(player_t* player);
		void esp_health_bar(player_t* player);
		void esp_box_setup(player_t* player);
		void esp_box(player_t* player);
		void esp_ammobar(player_t* player);
		void esp_glow();
		void esp_arrows(player_t* player);
	}
	
	namespace local {
		void local_scope();
		void nightmode();
		void indicators();
		void watermark();

	}
	struct {
		int left, top, right, bottom;
	}box;
}

namespace weird_stuff {
	int get_key(std::string key_value);
}

namespace rage_bot {
	void run(c_usercmd* cmd);
	void no_recoil(c_usercmd* cmd);
	bool aim(c_usercmd* cmd);
	int hitbox(player_t* player, int mode);
	player_t* closest_enemy();
	bool bone_visible(player_t*, int bone);
	void auto_pistol(c_usercmd* cmd);
	bool hitchance(player_t* player, weapon_t* weapon, vec3_t angle, int chance);
	void multipoint(player_t* player, bool sec);
	void auto_revolver(c_usercmd* cmd);
	//bool should_stop = false;
}

namespace backtrack
{
	float lerp_time();
	bool valid_tick(float sim_time);
	void backtrack(c_usercmd* cmd);
	static std::map<int, std::vector<vec3_t>> old_player_records = {};
}

namespace anti_aim {

	namespace fakelag {
		void run(c_usercmd* cmd, bool& sendpacket);
	}

	void run(c_usercmd* cmd, bool& sendpacket);
	

}

namespace animations
{
	void local_animations(c_usercmd* cmd, bool& sendpacket);
	void fake_animation();

	
	struct fake_var {
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
	};
	//fake_var var;
	
}

struct aimpoint_t
{
	aimpoint_t(const vec3_t& point, const vec3_t& center, const int hitbox)
	{
		this->point = point;
		this->center = center;
		this->hitbox = hitbox;
	}
	vec3_t point;
	vec3_t center;
	int hitbox;
};

namespace skinchanger
{
	void model_changer();
}


namespace autowall
{
	float damage(const vec3_t& point);
	bool CanHitFloatingPoint(const vec3_t& point, const vec3_t& source, player_t* pEnt);
}

struct fire_bullet_data
{
	fire_bullet_data(const vec3_t& eyePos, player_t* entity) : src(eyePos), filter(filter)
	{
	}

	vec3_t          src;
	trace_t         enter_trace;
	vec3_t          direction;
	trace_filter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

