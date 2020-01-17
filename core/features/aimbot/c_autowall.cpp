#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"
#include "../../../dependencies/interfaces/i_surfacedata.hpp"

#define CHECK_VALID( _v ) 0
#define FastSqrt(x)	(sqrt)(x)

inline float VectorLength(const vec3_t& v)
{
	CHECK_VALID(v);
	return (float)FastSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


void trace_the_line(vec3_t& vec_abs_start, vec3_t& vec_abs_end, unsigned int mask, player_t* ignore, trace_t* ptr)
{
	trace_filter filter;
	ray_t ray;
	ray.initialize(vec_abs_start, vec_abs_end);
	filter.skip = ignore;
	interfaces::trace_ray->trace_ray(ray, mask, &filter, ptr);
}

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case hitgroup_head:
		return 4.f;
	case hitgroup_stomach:
	case hitgroup_chest:
		return 1.25f;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		return 0.75f;
	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->armor() > 0.0f && hitgroup < hitgroup_leftleg)
	{
		if (hitgroup == hitgroup_head && !enemy->has_helmet())
			return;

		float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
		if ((current_damage - armorscaled) * 0.5f > enemy->armor())
			armorscaled = current_damage - (enemy->armor() * 2.0f);
		current_damage = armorscaled;
	}
}

void UTIL_ClipTraceToPlayers(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int mask, i_trace_filter* filter, trace_t* tr)
{
	static DWORD dwAddress = (DWORD)utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "x53 x8B xDC x83 xEC x08 x83 xE4 xF0 x83 xC4 x04 x55 x8B x6B x04 x89 x6C x24 x04 x8B xEC x81 xEC x00 x00 x00 x00 x8B x43 x10");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool TraceToExitalt(vec3_t& end, trace_t& tr, vec3_t start, vec3_t vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(vec3_t&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 EC 30 F3 0F 10 75");

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

bool vec3_ttovec3_tVisible(vec3_t src, vec3_t point, player_t* pEnt)
{
	//mask_solid
	trace_t TraceInit;
	TraceInit.entity = NULL;
	trace_filter filter1;
	ray_t ray;
	filter1.skip = csgo::local_player;
	ray.initialize(src, point);
	interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter1, &TraceInit);

	if (TraceInit.flFraction == 1.0f)
		return true;

	if (pEnt != nullptr && TraceInit.entity == pEnt)
		return true;

	return false;
}

bool HandleBulletPenetration(weapon_info_t* wpn_data, fire_bullet_data& data, bool extracheck = false, vec3_t point = vec3_t(0, 0, 0), player_t* pEnt = nullptr)
{   
	surfacedata_t* enter_surface_data = interfaces::surface_data->GetSurfaceData(data.enter_trace.surface.surfaceProps); // is nullptr
	
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	
	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= pow(wpn_data->m_range_modifier, (data.trace_length * 0.002));
	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;
	if (data.penetrate_count <= 0)
		return false;
	vec3_t dummy;
	trace_t trace_exit;
	console::log("made it before tracetoexitalt\n");
	if (!TraceToExitalt(dummy, data.enter_trace, data.enter_trace.end, data.direction, &trace_exit))
		return false;
	console::log("tracetoexitalt was true\n");
	surfacedata_t* exit_surface_data = interfaces::surface_data->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	console::log("here1\n");
	float final_damage_modifier = 0.16f;

	float combined_penetration_modifier = 0.0f;
	console::log("here2\n");
	/*if (((data.enter_trace.contents & CONTENTS_GRATE ) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else*/
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	console::log("here3\n");
	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)combined_penetration_modifier = 2.0f;
	}
	console::log("here4\n");
	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_penetration) * 1.25f);
	float thickness = VectorLength(trace_exit.end - data.enter_trace.end);
	console::log("here5\n");
	if (extracheck)
		if (!vec3_ttovec3_tVisible(trace_exit.end, point, pEnt))
			return false;
	console::log("here6\n");
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)
		return false;
	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f)
		return false;
	console::log("here7\n");
	data.src = trace_exit.end;
	console::log("here8\n");
	data.penetrate_count--;
	console::log("here9\n");
	
return true;
}

bool SimulateFireBullet(player_t* local, weapon_t* weapon, fire_bullet_data& data)
{
	data.penetrate_count = 4; // Max Amount Of Penitration
	data.trace_length = 0.0f; // wow what a meme
	auto* wpn_data = weapon->get_weapon_data(); // Get Weapon Info
	data.current_damage = (float)wpn_data->m_dmg;// Set Damage Memes
	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_range - data.trace_length;
		vec3_t End_Point = data.src + data.direction * data.trace_length_remaining;
		trace_the_line(data.src, End_Point, 0x4600400B, local, &data.enter_trace);
		UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		if (data.enter_trace.flFraction == 1.0f)
			break;
		if ((data.enter_trace.hitGroup <= 7) && (data.enter_trace.hitGroup > 0) && (local->team() != data.enter_trace.entity->team()))
		{
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= pow(wpn_data->m_range_modifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitGroup, data.enter_trace.entity, wpn_data->m_armor_ratio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(wpn_data, data, false, vec3_t(0, 0, 0), csgo::local_player))
			break;
	}
	return false;

}

float autowall::damage(const vec3_t& point)
{
	auto data = fire_bullet_data(csgo::local_player->get_eye_pos(), csgo::local_player);

	vec3_t angles;
	angles = math::calculate_angle_2(data.src, point);
	
	math::angle_vectors_alternative(angles, data.direction);
	
	data.direction.normalize();

	if (SimulateFireBullet(csgo::local_player, csgo::local_player->active_weapon(), data))
		return data.current_damage;

	return 0.f;
}

bool autowall::CanHitFloatingPoint(const vec3_t& point, const vec3_t& source, player_t* pEnt) // ez
{
	if (!csgo::local_player)
		return false;

	fire_bullet_data data = fire_bullet_data(csgo::local_player->get_eye_pos(), csgo::local_player);

	vec3_t angles = math::calculate_angle_2(data.src, point);
	math::angle_vectors_alternative(angles, data.direction);
	data.direction.normalize();

	weapon_t* pWeapon = (weapon_t*)csgo::local_player->active_weapon();;

	if (!pWeapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	weapon_info_t* weaponData = pWeapon->get_weapon_data();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->m_dmg;
	data.trace_length_remaining = weaponData->m_range - data.trace_length;
	vec3_t end = data.src + (data.direction * data.trace_length_remaining);
	trace_the_line(data.src, end, MASK_SHOT | CONTENTS_HITBOX, csgo::local_player, &data.enter_trace);

	if (vec3_ttovec3_tVisible(data.src, point, pEnt) || HandleBulletPenetration(weaponData, data, false/*true*/, point))
		return true;

	return false;
}

void CalcAngle34(vec3_t src, vec3_t dst, vec3_t& angles)
{
	vec3_t delta = src - dst;
	double hyp = delta.Length2D(); //delta.Length
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}
	