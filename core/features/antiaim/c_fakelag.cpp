#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"
void anti_aim::fakelag::run(c_usercmd* cmd, bool& sendpacket)
{
	
	if (!csgo::local_player) return;
	if (!c_engine->is_in_game() || !c_engine->is_connected()) return;
	if (!vars::checkbox["#enable_fakelag"]->get_bool()) return;
	if (!csgo::local_player->is_alive()) return;
	//if (c_engine->IsVoiceRecording()) return;


	int choke_amount = vars::slider["#fakelag_value"]->get_value();
	if (!vars::checkbox["#choke_stand"]->get_bool() && csgo::local_player->velocity().Length2D() < 10) choke_amount = 3;
	if (!vars::checkbox["#choke_shot"]->get_bool() && cmd->buttons & in_attack) choke_amount = 2;

	auto net_channel = c_engine->get_net_channel_info();

	if (!net_channel) return;
	
	if (vars::combobox["#fakelag_type"]->get_value() == 0) // factor
	{
		if (net_channel->choked_packets < choke_amount)
		{
			sendpacket = false;
		}
		else
		{
			sendpacket = true;
		}
	}
	else if (vars::combobox["#fakelag_type"]->get_value() == 1)
	{
		return;
	}


}