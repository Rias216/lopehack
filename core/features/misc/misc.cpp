#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"
#define CAM_HULL_OFFSET		14.0 
#define DIST_FORWARD 0
#define DIST_RIGHT 1
#define DIST_UP 2
#define	DIST	 2
static vec3_t CAM_HULL_MIN(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
static vec3_t CAM_HULL_MAX(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);
void misc::movement::bunny_hop(c_usercmd* cmd) {

	if (!vars::checkbox["#misc_bhop"]->get_bool())
		return;

	static auto last_jumped = false;
	static auto should_fake = false;

	const auto move_type = csgo::local_player->move_type();
	const auto flags = csgo::local_player->flags();

	if (move_type != movetype_ladder && move_type != movetype_noclip)
	{
		if (!last_jumped && should_fake)
		{
			should_fake = false;
			cmd->buttons |= in_jump;
		}
		else if (cmd->buttons & in_jump)
		{
			
			if (flags & fl_onground)
			{
				last_jumped = true;
				should_fake = true;
			}
			else
			{
				cmd->buttons &= ~in_jump;
				last_jumped = false;
			}
		}
		else
		{
			last_jumped = false;
			should_fake = false;
		}
	}
};


template<class T, class U>
inline T clamp_value(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void misc::movement::auto_strafe(c_usercmd* cmd)
{
	if (!vars::checkbox["#auto_strafer"]->get_bool())
		return;

	
}




void misc::extra::thirdperson()
{
	if (GetKeyState(weird_stuff::get_key("#thirdperson_key")) && csgo::local_player->is_alive())
	{
		float range = 150; // gör till en slider

		
	
		if (interfaces::input->m_fCameraInThirdPerson)
			return;

		vec3_t angles;
		
		interfaces::engine->get_view_angles(angles);
		angles.z = 0.f;
		vec3_t forward = math::angle_vector(angles);
		
		vec3_t eye_pos = csgo::local_player->get_eye_pos();
		vec3_t maximium, minimium;

		for (int x = 0; x <= 2; x++)
		{
			maximium[x] = 16.f;
			minimium[x] = -16.f;
		}
		
		ray_t ray;
		ray.initialize_(eye_pos, eye_pos - forward * range, minimium, maximium);
		trace_filter filter;
		filter.skip = csgo::local_player;
		trace_t trace;

		interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

		if (trace.flFraction < 1.f)
			range *= trace.flFraction;

		angles.z = range;
		interfaces::input->m_fCameraInThirdPerson = true;
		interfaces::input->m_vecCameraOffset = angles;
		
	}
	else
	{
		interfaces::input->m_fCameraInThirdPerson = false;
		interfaces::input->m_vecCameraOffset.z = 0;
	}
}





