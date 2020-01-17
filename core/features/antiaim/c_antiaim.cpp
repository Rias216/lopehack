#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"

bool lby_update()
{
	static float next_lby_update_time = 0;
	float curtime = c_globals->cur_time;

	auto animstate = csgo::local_player->get_anim_state();
	if (!animstate)
		return false;

	if (!(csgo::local_player->flags() & fl_onground))
		return false;

	if (animstate->speed > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}


void anti_aim::run(c_usercmd* cmd, bool& sendpacket)
{

	if (!vars::checkbox["#aa_enable"]->get_bool()) return;
	if (cmd->buttons & in_attack) return;




	cmd->viewangles.x = 89.f;

	if (lby_update())
	{
		cmd->viewangles.y -= 58.f;
		sendpacket = false;
		//return;
	}

	else if (sendpacket)
	{
		cmd->viewangles.y +=  180.f;
	}
	else
	{
		
		cmd->viewangles.y += 120.f;
		
	}

	/*
	if (csgo::local_player->flags() & fl_onground && cmd->sidemove < 3 && cmd->sidemove > -3) {
		static bool switch_ = false;
		if (switch_)
			cmd->sidemove = 2;
		else
			cmd->sidemove = -2;
		switch_ = !switch_;
	}*/
}