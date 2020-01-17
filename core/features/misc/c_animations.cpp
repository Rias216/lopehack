#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"
#include <array>

void update_state(anim_state* state, vec3_t ang) {
	using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto ret = reinterpret_cast<fn>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));

	if (!ret)
		return;

	ret(state, NULL, NULL, ang.y, ang.x, NULL);
}

void update_anim_state(anim_state* state, vec3_t angle)
{
	static auto UpdateAnimState = utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

bool fresh_tick()
{
	static int old_tick_count;

	if (old_tick_count != c_globals->tick_count)
	{
		old_tick_count = c_globals->tick_count;
		return true;
	}

	return false;
}

void animations::local_animations(c_usercmd* cmd, bool& sendpacket)
{
	
}


void animations::fake_animation()
{
	if (!csgo::local_player || !csgo::local_player->is_alive() || !vars::checkbox["#cham_desync"]->get_bool())
		return;

	
	
	if (csgo::desync_chams::m_fake_spawntime != csgo::local_player->spawn_time() || csgo::desync_chams::m_should_update_fake)
	{
		csgo::desync_chams::init_fake_anim = false;
		csgo::desync_chams::m_fake_spawntime = csgo::local_player->spawn_time();
		csgo::desync_chams::m_should_update_fake = false;
	}

	if (!csgo::desync_chams::init_fake_anim)
	{
		csgo::desync_chams::m_fake_state = static_cast<anim_state*> (interfaces::mem_alloc->Alloc(sizeof(anim_state)));

		if (csgo::desync_chams::m_fake_state != nullptr)
			csgo::local_player->create_animation_state(csgo::desync_chams::m_fake_state);

		csgo::desync_chams::init_fake_anim = true;
	}

	animation_layer layer_backup[13];
	float pose_backup[20];

	std::memcpy(layer_backup, csgo::local_player->anim_overlays(), sizeof(layer_backup));
	std::memcpy(pose_backup, csgo::local_player->pose_parameter().data(), sizeof(pose_backup));

	const auto ba_curtime = c_globals->cur_time;

	update_anim_state(csgo::desync_chams::m_fake_state, csgo::local_player->eye_angles());

	csgo::desync_chams::m_fake_rotation = reinterpret_cast<anim_state*> (csgo::desync_chams::m_fake_state)->goal_feet_yaw;
	csgo::local_player->set_abs_angles_vec(vec3_t(0, reinterpret_cast<anim_state*> (csgo::desync_chams::m_fake_state)->goal_feet_yaw, 0));
	csgo::desync_chams::m_got_fake_matrix = csgo::local_player->setup_bones(csgo::desync_chams::m_fake_matrix, 128, bone_used_by_anything, c_globals->cur_time);

	memcpy(csgo::desync_chams::m_fake_position_matrix, csgo::desync_chams::m_fake_matrix, sizeof(csgo::desync_chams::m_fake_position_matrix));

	const auto org_tmp = csgo::local_player->vec_origin();

	if (csgo::desync_chams::m_got_fake_matrix)
	{
		for (auto& i : csgo::desync_chams::m_fake_matrix)
		{
			i[0][3] -= org_tmp.x;
			i[1][3] -= org_tmp.y;
			i[2][3] -= org_tmp.z;
		}
	}


	float m_server_abs_rotation = 0.f; // replace with actuall real angle!

	if (csgo::sendpacket)
		m_server_abs_rotation = reinterpret_cast<anim_state*> (csgo::local_player->get_anim_state())->goal_feet_yaw;

	csgo::local_player->set_abs_angles_vec(vec3_t(0, m_server_abs_rotation, 0)); // restore real abs rotation

	std::memcpy(csgo::desync_chams::m_fake_layers.data(), csgo::local_player->anim_overlays(), sizeof(csgo::desync_chams::m_fake_layers));
	std::memcpy(csgo::desync_chams::m_fake_poses.data(), csgo::local_player->pose_parameter().data(), sizeof(csgo::desync_chams::m_fake_poses));

	c_globals->cur_time = ba_curtime;

	std::memcpy(csgo::local_player->anim_overlays(), layer_backup, sizeof(layer_backup));
	std::memcpy(csgo::local_player->pose_parameter().data(), pose_backup, sizeof(pose_backup));


}

