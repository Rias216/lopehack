#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"

#define prefer_head 0
#define prefer_body 1
player_t* rage_bot::closest_enemy()
{
	vec3_t view_angle;
	c_engine->get_view_angles(view_angle);
	
	player_t* closest_entity = nullptr;

	float best_delta = FLT_MAX;

	for (size_t i = 1; i <= 64; i++)
	{
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (player && !player->dormant() && player->is_alive() && player->team() != csgo::local_player->team())
		{ 
			vec3_t angle = math::calculate_angle_2(csgo::local_player->get_eye_pos(), player->get_eye_pos()).clamped();

			float delta = (angle - view_angle).clamped().length();

			if (delta < best_delta)
			{
				best_delta = delta;
				closest_entity = player;
			}

		}

	}


	return closest_entity;
}




void rage_bot::auto_pistol(c_usercmd* cmd)
{
	if (csgo::local_player->active_weapon()->get_weapon_data()->m_type == cs_weapon_type::WEAPONTYPE_PISTOL && cmd->tick_count % 2)
	{
		static int lastshot;
		if (cmd->buttons & in_attack)
			lastshot++;

		if (!(cmd->buttons & in_attack) || lastshot > 1)
		{
			cmd->buttons &= ~in_attack;
			lastshot = 0;
		}
		return;
	}
}

bool rage_bot::bone_visible(player_t* player, int bone)
{
	vec3_t end;
	end = player->get_bone_position(bone);

	trace_t trace;
	ray_t ray;

	trace_filter filter;
	filter.skip = csgo::local_player;

	ray.initialize(csgo::local_player->get_eye_pos(), end);

	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &trace);

	if (trace.entity && trace.entity->team() != csgo::local_player->team() && !(trace.entity->health() <= 0) && !(trace.entity->dormant()))
	{
		if (trace.physicsBone <= 128 && trace.physicsBone > 0)
		{
			return true;
		}

	}
	return false;
}
float RandomFloat(float min, float max)
{
	// this  function assumes max > min, you may want 
	// more robust error checking for a non-debug build
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}
bool rage_bot::hitchance(player_t* player, weapon_t* weapon, vec3_t angle, int chance)
{
	
	if (!weapon)
		return false;
	if (chance == 0) return true;

	vec3_t forward, right, up;
	vec3_t src = csgo::local_player->get_eye_pos();
	math::angle_vectors(angle, &forward, &right, &up);

	int cHits = 0;
	int cNeededHits = static_cast<int>(150.f * chance); // remove ( / 100.f)

	weapon->update_accuracy_penalty();
	float weap_spread = weapon->get_spread();
	float weap_inaccuracy = weapon->inaccuracy();
	
	for (int i = 0; i < 150; i++)
	{
		float a = RandomFloat(0.f, 1.f);
		float b = RandomFloat(0.f, 2.f * M_PI_F); // M_PI
		float c = RandomFloat(0.f, 1.f);
		float d = RandomFloat(0.f, 2.f * M_PI_F); // M_PI

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if (weapon->item_definition_index() == 64)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		vec3_t spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.normalize();

		vec3_t viewAnglesSpread;
		math::vector_angles_3(direction, up, viewAnglesSpread);
		
       viewAnglesSpread.normalize();

		vec3_t viewForward;
		math::angle_vectors_alternative(viewAnglesSpread, viewForward);
		viewForward.normalize();

		viewForward = src + (viewForward * weapon->get_weapon_data()->m_range);

		trace_t tr;
		ray_t ray;

		ray.initialize(src, viewForward);
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, player, &tr);
		
		
		if (tr.did_hit()) // test , hit_entity   did_hit_non_world_entity
			++cHits;

		if (static_cast<int>((static_cast<float>(cHits) / 150.f) * 100.f) >= chance)
			return true;

		if ((150 - i + cHits) < cNeededHits)
			return false;
	}
	return false;
}

void rage_bot::multipoint(player_t* player, bool sec)
{
	if (!player) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->active_weapon()) return;

	auto model = player->model();
	if (!model) return;

	auto weapon = csgo::local_player->active_weapon();

	bool moving = player->velocity().length() > 0.1f;

	for (auto i = 0; i < hitbox_max; i++)
	{
		if ((i == hitbox_right_foot || i == hitbox_left_foot) && moving || (player->flags() & fl_onground))
			continue;

		if (sec && i > hitbox_neck)
			continue;

		if (!sec && i <= hitbox_neck)
			continue;

		auto studio_model = interfaces::model_info->get_studio_model(model);

		if (!studio_model)
			continue;

		auto hitbox = studio_model->hitbox_set(0)->hitbox(i);

		if (!hitbox) continue;

		const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
		matrix_t bone_matrix[MAXSTUDIOBONES];
		if (!player->setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) continue;
		console::log("made it past setupbones!\n");


		vec3_t max;
		vec3_t min;

		math::transform_vector(hitbox->maxs /*+ mod*/, bone_matrix[hitbox->bone], max);
		math::transform_vector(hitbox->mins /*+ mod*/, bone_matrix[hitbox->bone], min);

		const auto center = (min + max) * 0.5f;

		// body scale

		if (i == hitbox_head)
		{
			// head scale
		}

		//radius = rs
	}
}


void rage_bot::no_recoil(c_usercmd* cmd)
{
	static auto weapon_recoil_scale = c_cvar->get_convar("weapon_recoil_scale");

	cmd->viewangles -= csgo::local_player->aim_punch_angle() * weapon_recoil_scale->get_float();
}
bool rage_bot::aim(c_usercmd* cmd)
{
	
	if (!csgo::local_player->active_weapon()) return false;

		player_t* player = closest_enemy();

		if (player)
		{
			vec3_t angle;

			if (hitbox(player, prefer_head | prefer_body) == 69) return false;
			if (vars::combobox["#hitscan_mode"]->get_value() == 0)
				angle = math::calculate_angle_2(csgo::local_player->get_eye_pos(), player->get_hitbox_position(hitbox(player, prefer_head)));
			else if (vars::combobox["#hitscan_mode"]->get_value() == 1)
				angle = math::calculate_angle_2(csgo::local_player->get_eye_pos(), player->get_hitbox_position(hitbox(player, prefer_body)));


			if (!vars::checkbox["#ragehehe"]->get_bool())
				return false;

			

			if (!hitchance(player, csgo::local_player->active_weapon(), angle, vars::slider["#weapon_auto_hitchance"]->get_value()/* * 100.f*/)) // *100.f
				return false;
			float distance = csgo::local_player->vec_origin().distance_to(player->vec_origin());
			if (csgo::local_player->active_weapon()->item_definition_index() == WEAPON_ZEUS && distance > 155) return false;


			if (vars::checkbox["#silent_aim"]->get_bool())
				cmd->viewangles = angle;
			else
				c_engine->set_view_angles(angle);
			
			return true;
		}
	

	return false;
}

bool acceptable_spread()
{
	if (!vars::checkbox["#limit_spread"]->get_bool()) return true;
	auto weapon = csgo::local_player->active_weapon();
	float inacc = csgo::local_player->active_weapon()->inaccuracy() * 1000.f;
	if (csgo::local_player->is_sniper(csgo::local_player)/* && weapon->item_definition_index() != WEAPON_AWP*/)
	{
		if (inacc < 8.f)
			return true;
	}
	else
	{
		if (inacc < 90.f)
			return true;
	}

	return false;
}

void rage_bot::auto_revolver(c_usercmd* cmd)
{
	if (!(c_engine->is_in_game() || c_engine->is_connected())) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;

	if (!vars::checkbox["#ragehehe"]->get_bool())
		return;

	static auto r8cock_flag = true;
	static auto r8cock_time = 0.0f;

	if (csgo::local_player->active_weapon()->item_definition_index() == WEAPON_REVOLVER && !(cmd->buttons & in_attack))
	{
		//globals.revolver_working = true;
		r8cock_flag = true;

		if (r8cock_flag/* && globals.weapon->can_fire()*/)
		{
			if (r8cock_time <= time_to_ticks(csgo::local_player->get_tick_base()))
			{
				if (csgo::local_player->active_weapon()->next_secondary_attack() <= time_to_ticks(csgo::local_player->get_tick_base()))
					r8cock_time = time_to_ticks(csgo::local_player->get_tick_base()) + 0.234375f;
				else
					cmd->buttons |= in_attack2;
			}
			else
				cmd->buttons |= in_attack;

			r8cock_flag = time_to_ticks(csgo::local_player->get_tick_base()) > r8cock_time;
		}
		else
		{
			r8cock_flag = false;
			r8cock_time = time_to_ticks(csgo::local_player->get_tick_base()) + 0.234375f;
			cmd->buttons &= ~in_attack;
		}
	}
}

void rage_bot::run(c_usercmd* cmd)
{
	if (!(c_engine->is_in_game() || c_engine->is_connected())) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;

	if (!vars::checkbox["#ragehehe"]->get_bool())
		return;

	auto local_weapon = csgo::local_player->active_weapon();
	if (!local_weapon) return;
	if (csgo::local_player->next_attack() > c_globals->cur_time) return;
	if (csgo::local_player->client_class()->class_id == 105) return;
	
	//if (!closest_enemy) return;

	//auto interval_per_tick = c_globals->interval_per_tick * 2.0f;
	//auto unlag = fabs(closest_enemy()->simulation_time() - closest_enemy()->old_simulation_time() < interval_per_tick);




	if (vars::checkbox["#no_recoil"]->get_bool())
		no_recoil(cmd);

	aim(cmd);


	auto_pistol(cmd);
	
	

//	if (!unlag) return;
	if (!acceptable_spread()) return;
	
	if (vars::checkbox["#auto_shoot"]->get_bool() && aim(cmd) == true &&/*knife fix*/local_weapon->inaccuracy() != 0)
		cmd->buttons |= in_attack;

	return;
}




int rage_bot::hitbox(player_t* player, int mode)
{
	if (mode == 0)
	{
		if (bone_visible(player, hitboxes::hitbox_head)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_head)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_head;
		else if (bone_visible(player, hitbox_chest)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_chest)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_chest;
		else if (bone_visible(player, hitbox_pelvis)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_pelvis)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_pelvis;
		else if (bone_visible(player, hitbox_stomach )/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_head)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_stomach;
		else if (bone_visible(player, hitbox_upper_chest)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_upper_chest)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_upper_chest;
		else if (bone_visible(player, hitbox_lower_chest)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_lower_chest)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_lower_chest;
		else if (bone_visible(player, hitbox_right_thigh)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_right_thigh)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_right_thigh;
		else if (bone_visible(player, hitbox_left_thigh)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_left_thigh)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_left_thigh;
		else if (bone_visible(player, hitbox_left_foot) /*|| autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_left_foot)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_left_foot;
		else if (bone_visible(player, hitbox_right_foot)/* || autowall::CanHitFloatingPoint(player->get_hitbox_position(hitbox(player, hitbox_right_foot)), csgo::local_player->get_eye_pos(), player)*/)
			return hitbox_right_foot;
	}
	else
	{
		if (bone_visible(player, hitbox_chest))
			return hitbox_chest;
		else if (bone_visible(player, hitbox_chest))
			return hitbox_chest;
		else if (bone_visible(player, hitbox_pelvis))
			return hitbox_pelvis;
		else if (bone_visible(player, hitbox_stomach))
			return hitbox_stomach;
		else if (bone_visible(player, hitbox_upper_chest))
			return hitbox_upper_chest;
		else if (bone_visible(player, hitbox_lower_chest))
			return hitbox_lower_chest;
		else if (bone_visible(player, hitbox_head))
			return hitbox_head;
		else if (bone_visible(player, hitbox_right_thigh))
			return hitbox_right_thigh;
		else if (bone_visible(player, hitbox_left_thigh))
			return hitbox_left_thigh;
		else if (bone_visible(player, hitbox_left_foot))
			return hitbox_left_foot;
		else if (bone_visible(player, hitbox_right_foot))
			return hitbox_right_foot;
	}
	
	return 69;
}
