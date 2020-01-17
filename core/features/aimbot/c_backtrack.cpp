#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"



float fov(vec3_t angle)
{
	
	vec3_t view_angle;
	c_engine->get_view_angles(view_angle);

	float best_delta = FLT_MAX;

	float delta = (angle - view_angle).clamped().length();

	if (delta < best_delta)
		best_delta = delta;


		return best_delta;
}

void backtrack::backtrack(c_usercmd* cmd)
{
	if (!csgo::local_player || !csgo::local_player->is_alive() || !cmd || !vars::checkbox["#rage_backtrack"]->get_bool()) return;

	std::vector<vec3_t> data_this_tick = {};

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto temp_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!temp_player || temp_player->dormant() || !temp_player->is_alive() || temp_player->index() == csgo::local_player->index()) continue;

		// if setting

		data_this_tick.push_back(temp_player->get_hitbox_position(rage_bot::hitbox(temp_player, 0)));


	}

	old_player_records[cmd->tick_count] = data_this_tick;

	for (auto i = old_player_records.begin(); i != old_player_records.end();)
	{
		const int from_tick = i->first;
		if ((cmd->tick_count - from_tick) * c_globals->interval_per_tick > 0.2f/*make this a silder (max backtrack time)*/)
		{
			old_player_records.erase(i++->first);
		}
		else
		{
			++i;
		}
	}

	if (cmd->buttons & in_attack)
	{
		bool found_hittable_tick = false;
		for (auto i = old_player_records.begin(); i != old_player_records.end(); i++)
		{
			const int record_tick = i->first;

			for (int record = 0; record < i->second.size(); record++)
			{
				vec3_t goal = math::calculate_angle_2(csgo::local_player->get_eye_pos(), i->second[record] - csgo::local_player->aim_punch_angle() * 2.0f);

				if (fov(goal) < 3.5)
				{
					cmd->tick_count = record_tick;
					found_hittable_tick = true;
					break;
				}
			}
			if (found_hittable_tick)
			{
				break;
			}
		}
	}
}