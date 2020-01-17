#pragma once
#include "../../dependencies/common_includes.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/FGUI/FGUI.hh"
#include "../menu/fgui_menu.hpp"
#include "../../source-sdk/math/vector3d.hpp"


/*how to get entity:

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
	}

*/

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::writeuser_cmd_delta::fn writeuser_cmd_delta_original = nullptr;
hooks::frame_stage::fn frame_stage_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::in_key_event::fn in_key_event_original = nullptr;

c_events* g_pevents;
vec3_t real_angle;

c_usercmd* cmd_ = nullptr;

unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }

bool hooks::initialize() {
	
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto overrideview_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	auto model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	auto writebuffer_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 24));
	auto frame_stage_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto draw_set_color_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 15));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto in_key_event_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 21));
	auto scene_end_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	
	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}
	if (MH_CreateHook(overrideview_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize override view. (outdated index?)");
		return false;
	}
	
	if (MH_CreateHook(writebuffer_target, &writeuser_cmd_delta::hook, reinterpret_cast<void**>(&writeuser_cmd_delta_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize buffer_hook. (outdated index?)");
		return false;
	}
	if (MH_CreateHook(model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize draw_execute. (outdated index?)");
		return false;
	}
	if (MH_CreateHook(frame_stage_target, &frame_stage::hook, reinterpret_cast<void**>(&frame_stage_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize frame_stage. (outdated index?)");
		return false;
	}
	if (MH_CreateHook(scene_end_target, &scene_end::hook, reinterpret_cast<void**>(&scene_end_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize scene_end. (outdated index?)");
		return false;
	}
	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(in_key_event_target, &in_key_event::hook, reinterpret_cast<void**>(&in_key_event_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize in_key_event. (outdated index?)");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}
	g_pevents->init();
	

	console::log("[setup] hooks initialized!\n");

	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

void bullet_impacts(QAngle& angle)
{
	vec3_t o;
	vec3_t b;
	c_debug_overlay->AddBoxOverlay(o, b, b, angle, 255, 255, 255, 255, 4);
	
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::movement::auto_strafe(cmd);
	prediction::start(cmd); {
		if (vars::checkbox["#ragehehe"]->get_bool())
			rage_bot::run(cmd);
	} prediction::end();
	
	rage_bot::auto_revolver(cmd);
	anti_aim::fakelag::run(cmd, send_packet);
	anti_aim::run(cmd, send_packet);
	if (vars::checkbox["#no_croch"])
		cmd->buttons |= in_bullrush;
	backtrack::backtrack(cmd);
	
	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);
	
	

	visuals::local::nightmode();

	cmd->viewangles.clamp();
	std::clamp(cmd->sidemove, -450.f, 450.f);
	if (send_packet)
		real_angle = cmd->viewangles;

	if (!vars::checkbox["#slide"]->get_bool())
	{
		cmd->buttons &= ~in_moveright;
		cmd->buttons &= ~in_moveleft;
		cmd->buttons &= ~in_forward;
		cmd->buttons &= ~in_back;

		if (cmd->forwardmove > 0.f)
			cmd->buttons |= in_forward;
		else if (cmd->forwardmove < 0.f)
			cmd->buttons |= in_back;
		if (cmd->sidemove > 0.f)
		{
			cmd->buttons |= in_moveright;
		}
		else if (cmd->sidemove < 0.f)
		{
			cmd->buttons |= in_moveleft;
		}
	}


	if (vars::checkbox["#double_tap"]->get_bool() && GetAsyncKeyState(VK_XBUTTON1))
		csgo::tick_abuse::m_nTickbaseShift = time_to_ticks(2.0f);
	else
		csgo::tick_abuse::m_nTickbaseShift = time_to_ticks(0.0f);

	if (vars::checkbox["#auto_stop"]->get_bool() && rage_bot::aim(cmd) == true && csgo::local_player->active_weapon()->inaccuracy() != 0)
	{
	//	if (!rage_bot::aim(cmd) == true || !rage_bot::should_stop == true) return;
		if (vars::checkbox["#auto_scope"]->get_bool() && csgo::local_player->is_sniper(csgo::local_player) && !csgo::local_player->is_scoped())
			cmd->buttons |= in_attack2;

		cmd->sidemove = 1;
		cmd->forwardmove = 1;


	}
	cmd->tick_count = time_to_ticks(backtrack::lerp_time() + csgo::local_player->simulation_time()); // test antar jag
	csgo::sendpacket = send_packet;
	cmd_ = cmd;
	return false;
}




void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));
	static bool* s_bOverridePostProcessingDisable = *(bool**)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "80 3D ? ? ? ? ? 53 56 57 0F 85") + 0x2);
	vars::multibox["#removals"]->get_selected(0) ? *s_bOverridePostProcessingDisable = true : *s_bOverridePostProcessingDisable = false;
	static unsigned int panelID, panelHudID;
	if (!panelHudID)
	{
		if (!strcmp("HudZoom", interfaces::panel->get_panel_name(panel)))
		{
			panelHudID = panel;
		}
	}

	if (panelHudID == panel && csgo::local_player && csgo::local_player->is_alive() && vars::multibox["#removals"]->get_selected(0))
	{
		if (csgo::local_player->is_scoped())
			return;
	}
	
	
	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		
		fgui::handler::render_window();

		if (interfaces::engine->is_connected() || interfaces::engine->is_in_game())
		{
			visuals::visual_main::esp_render();
			if (!csgo::local_player) return;
			if (!csgo::local_player->is_alive()) return;
			render::draw_text_string(10, 10, render::fonts::watermark_font, "lopeware -cool boi build-  ", false, color::white(255));
			visuals::local::indicators();
			render::draw_text_string(20, 20, render::fonts::watermark_font, std::to_string(cmd_->sidemove).c_str(), false, color::white());
			visuals::local::watermark();
		}

		break;
	}
	
	

	if (!c_engine->is_in_game()) // test
	{
		csgo::desync_chams::m_should_update_fake = true;
		// update real
	}


	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::lock_cursor::hook() {
	if(!(interfaces::engine->is_connected() || interfaces::engine->is_in_game()))
		return lock_cursor_original(interfaces::surface);;
	if (vars::container["#window"]->get_state()) {
		interfaces::surface->unlock_cursor();
		return;

		interfaces::inputsystem->enable_input(false);
	}
	else {
		interfaces::inputsystem->enable_input(true);
	}

	return lock_cursor_original(interfaces::surface);
}

int __fastcall hooks::in_key_event::hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding) {

	if (vars::container["#window"]->get_state())
		return 0;

	return in_key_event_original(event_code, key_num, current_binding);
}           


void __fastcall hooks::override_view::hook(void* ecx, void* edx, view_setup_t* setup)
{
	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game() && csgo::local_player)
	{ 
		setup->fov += vars::slider["#view_fov"]->get_value();
		misc::extra::thirdperson();
		if (csgo::local_player->is_scoped() && vars::multibox["#removals"]->get_selected(1))
			setup->fov += vars::slider["#view_fov"]->get_value() + 40;
		
	}
	return override_view_original(ecx, edx, setup); 
}

bool __fastcall hooks::writeuser_cmd_delta::hook(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool new_cmd)
{
	DWORD desired_return = (DWORD)utilities::pattern_scan(GetModuleHandleA("engine.dll"), "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?");

	if(!vars::checkbox["#double_tap"]->get_bool())
		return writeuser_cmd_delta_original(ecx, edx, slot, buf, from, to, new_cmd);
	if ((DWORD)_ReturnAddress() != desired_return)
		return writeuser_cmd_delta_original(ecx, edx, slot, buf, from, to, new_cmd);

	if (from != -1)
		return true;

	int tick_base = csgo::tick_abuse::m_nTickbaseShift;
	csgo::tick_abuse::m_nTickbaseShift = 0; // test

	int* backup_cmds = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x30);
	int* new_cmds = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x2C);

	*backup_cmds = 0; // test;

	int m_new_cmd, next_cmd = 4;
	console::log("here\n");
	if (tick_base > 0) // optimize
	{
		m_new_cmd = c_max(*new_cmds - tick_base, 1);
		*new_cmds = m_new_cmd;

		m_new_cmd = interfaces::clientstate->last_outgoing_command + interfaces::clientstate->choked_commands + 1;
		console::log("here1\n");
		for (to = next_cmd - m_new_cmd + 1; to <= next_cmd; to++)
		{
			console::log("here2\n");
			if (!writeuser_cmd_delta_original(ecx, edx, slot, buf, from, to, true))
				return false;
			console::log("here3\n");
			from = to;
		}
		return true;
	}

	from = -1;

	int total_new_cmds = c_min(*new_cmds + abs(tick_base), 62);
	m_new_cmd = *new_cmds;
	*new_cmds = total_new_cmds;
	next_cmd = interfaces::clientstate->last_outgoing_command + interfaces::clientstate->choked_commands + 1;
	console::log("here4\n");
	for (to = next_cmd - m_new_cmd + 1; to <= next_cmd; to++)
	{
		console::log("here5\n");
		if (!writeuser_cmd_delta_original(ecx, edx, slot, buf, from, to, true))
			return false;
		console::log("here6\n");
		from = to;
	}

	c_usercmd* last_realcmd = interfaces::input->get_user_cmd(slot, from);
	c_usercmd from_cmd, to_cmd;

	if (!last_realcmd)
		return true;

	from_cmd = *last_realcmd;
	to_cmd = from_cmd;

	to_cmd.command_number++;
	to_cmd.tick_count += 3 * 64/*the tickrate*/;
	console::log("here7\n");
	if (m_new_cmd < total_new_cmds)
		return true;

	int shift = total_new_cmds - m_new_cmd + 1;
	console::log("here8\n");
	while (shift > 0)
	{
		from_cmd.randomseed = to_cmd.randomseed;
		from_cmd.viewangles = to_cmd.viewangles;
		from_cmd.mousedx = to_cmd.mousedx;
		from_cmd.hasbeenpredicted = to_cmd.hasbeenpredicted;
		from_cmd.upmove = to_cmd.upmove;
		from_cmd.pad = to_cmd.pad;
		from_cmd.weaponsubtype = to_cmd.weaponsubtype;
		from_cmd.tick_count = to_cmd.tick_count;
		from_cmd.command_number = to_cmd.command_number;
		console::log("here9\n");
		to_cmd.command_number++;
		to_cmd.tick_count++;
		console::log("here10\n");
		shift--;
		console::log("here11\n");
	}
	console::log("shifted\n");
	return true;

}


void __fastcall hooks::scene_end::hook(void* ecx, void* edx) 
{
	
	if (!c_engine->is_in_game() || !c_engine->is_connected() || !csgo::local_player)
		return;

	

	i_material* enemy_mat = nullptr;
	i_material* enemy_mat_invis = nullptr;

	

	switch (vars::combobox["#enemy_cham_type"]->get_value())
	{
	case 0:
		enemy_mat = c_mat_system->find_material("simple_regular", "Model textures");
		enemy_mat_invis = c_mat_system->find_material("simple_ignorez", "Model textures");
		break;
	case 1:
		enemy_mat = c_mat_system->find_material("simple_flat", "Model textures");
		enemy_mat_invis = c_mat_system->find_material("simple_flat_ignorez", "Model textures");
		break;
	case 2:
		enemy_mat = c_mat_system->find_material("simple_reflective", nullptr);
		enemy_mat_invis = c_mat_system->find_material("simple_reflectiveignorez", nullptr);
		break;
	case 3:
		enemy_mat = c_mat_system->find_material("glowOverlay", nullptr);
		enemy_mat_invis = c_mat_system->find_material("glowOverlay_ignorez", nullptr);
		break;
	}	

	for (int i = 1; i <= 64; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity)
			return;
		if (entity == csgo::local_player) // local chams
		{
			
			entity->draw_model(0x1, 255);
			c_model_render->override_material(nullptr);

		}
		else if (entity->team() != csgo::local_player->team()) // enemy
		{
			if (!vars::checkbox["#chams_enemy"]->get_bool())
				return;

			if (vars::checkbox["#chams_enemy_invis"]->get_bool())
			{


				if (vars::combobox["#enemy_cham_type"]->get_value() == 3)
				{
					bool found_ = false;
					auto pVar = enemy_mat_invis->find_var("$envmaptint", &found_);
					if (found_)
						(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_red / 255.f, vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_green / 255.f, vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_blue / 255.f); //tfw no IMaterialVar class
				}
				else
					enemy_mat_invis->color_modulate(vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_red / 255.f, vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_green / 255.f, vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_blue / 255.f);

				enemy_mat_invis->alpha_modulate(vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_alpha / 255.f);
				interfaces::render_view->set_blend(vars::colorpicker["#enemy_cham_invis_color"]->get_color().m_alpha / 255.f);

				c_model_render->override_material(enemy_mat_invis);
				entity->draw_model(0x1, 255);
				c_model_render->override_material(nullptr);
			}


			if (vars::combobox["#enemy_cham_type"]->get_value() == 3)
			{
				bool _found_ = false;
				auto pVar = enemy_mat->find_var("$envmaptint", &_found_);
				if (_found_)
					(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, vars::colorpicker["#enemy_chams_color"]->get_color().m_red / 255.f, vars::colorpicker["#enemy_chams_color"]->get_color().m_green / 255.f, vars::colorpicker["#enemy_chams_color"]->get_color().m_blue / 255.f); //tfw no IMaterialVar class
			}
			else
				enemy_mat->color_modulate(vars::colorpicker["#enemy_chams_color"]->get_color().m_red / 255.f, vars::colorpicker["#enemy_chams_color"]->get_color().m_green / 255.f, vars::colorpicker["#enemy_chams_color"]->get_color().m_blue / 255.f);

			enemy_mat->alpha_modulate(vars::colorpicker["#enemy_chams_color"]->get_color().m_alpha / 255.f);
			interfaces::render_view->set_blend(vars::colorpicker["#enemy_chams_color"]->get_color().m_alpha / 255.f);

			c_model_render->override_material(enemy_mat);
			entity->draw_model(0x1, 255);


			c_model_render->override_material(nullptr);
		}
		}
	}





void __fastcall hooks::draw_model_execute::hook(void* ecx, void* edx, IMatRenderContext* context, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix)
{
	const char* model_name = interfaces::model_info->get_model_name((model_t*)info.model); 

	
	if (!(c_engine->is_in_game() && c_engine->is_connected()) && !csgo::local_player)
		return draw_model_execute_original(ecx, edx, context, state, info, matrix);

	
	visuals::basic::esp_glow();
	i_material* hand_mat = nullptr;
	i_material* desync = nullptr;
	i_material* glow_mat = nullptr;
	switch (vars::combobox["#hand_mat"]->get_value())
	{
	case 0:
		hand_mat = c_mat_system->find_material("simple_regular", nullptr);
		break;
	case 1:
		hand_mat = c_mat_system->find_material("simple_flat", nullptr);
		break;
	case 2:
		hand_mat = c_mat_system->find_material("glowOverlay", nullptr);
		break;
	case 3:
		hand_mat = c_mat_system->find_material("simple_regular", nullptr);
		break;
	}
	glow_mat = c_mat_system->find_material("glowOverlay", nullptr);
	desync = c_mat_system->find_material("simple_flat", nullptr);
	if (strstr(model_name, "models/player/") && csgo::local_player &&  vars::checkbox["#local_scoped"]->get_bool())
	{
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));

		if (player && player == csgo::local_player && csgo::local_player->is_scoped())
			c_render_view->set_blend(0.25f);

	}

	
	if (strstr(model_name, "models/player/") && csgo::local_player && vars::checkbox["#cham_desync"]->get_bool())
	{
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		animations::fake_var fake;
		
		if (player && player == csgo::local_player)
		{
			if (csgo::local_player->is_alive() && c_globals->cur_time - csgo::local_player->spawn_time() > 3.f)
			{
				if (csgo::desync_chams::m_got_fake_matrix)
				{
					for (auto& i : csgo::desync_chams::m_fake_matrix)
					{
						i[0][3] += info.origin.x;
						i[1][3] += info.origin.y;
						i[2][3] += info.origin.z;
					}
				     
					if (!desync || desync == nullptr)
						return;

					desync->color_modulate(vars::colorpicker["#cham_desync_color"]->get_color().m_red / 255.f, vars::colorpicker["#cham_desync_color"]->get_color().m_green / 255.f, vars::colorpicker["#cham_desync_color"]->get_color().m_blue / 255.f);
					c_render_view->set_blend(vars::colorpicker["#cham_desync_color"]->get_color().m_alpha / 255.f);
					c_model_render->override_material(desync);
					draw_model_execute_original(ecx, edx, context, state, info, csgo::desync_chams::m_fake_matrix);

					//c_model_render->override_material(nullptr);
					//c_render_view->set_blend(1.f);



				}


			}
		}
	}






	
	if (strstr(model_name, "models/props"))
	{
		c_render_view->set_blend(vars::slider["#prop_trans"]->get_value() / 100);
	}

	if (strstr(model_name, "arms") && vars::checkbox["#cham_hand"]->get_bool())
	{
		if (!hand_mat)
			return;

		hand_mat->set_material_var_flag(material_var_ignorez, false);
		if (vars::combobox["#hand_mat"]->get_value() == 2)
		{
			bool found = false;
			auto pVar = hand_mat->find_var("$envmaptint", &found);
			if (found)
				(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, vars::colorpicker["#hand_color"]->get_color().m_red / 255.f, vars::colorpicker["#hand_color"]->get_color().m_green / 255.f, vars::colorpicker["#hand_color"]->get_color().m_blue / 255.f); //tfw no IMaterialVar class
		}
		else
			hand_mat->color_modulate(vars::colorpicker["#hand_color"]->get_color().m_red / 255.f, vars::colorpicker["#hand_color"]->get_color().m_green / 255.f, vars::colorpicker["#hand_color"]->get_color().m_blue / 255.f);
		c_render_view->set_blend(vars::colorpicker["#hand_color"]->get_color().m_alpha / 255.f);
		c_model_render->override_material(hand_mat);

		if (vars::combobox["#hand_mat"]->get_value() == 3)
			draw_model_execute_original(ecx, edx, context, state, info, matrix);

		if (vars::combobox["#hand_mat"]->get_value() == 3)
		{
			bool found_ = false;
			auto pVar = glow_mat->find_var("$envmaptint", &found_);
			if (found_)
				(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, vars::colorpicker["#hand_color_2"]->get_color().m_red / 255.f, vars::colorpicker["#hand_color_2"]->get_color().m_green / 255.f, vars::colorpicker["#hand_color_2"]->get_color().m_blue / 255.f); //tfw no IMaterialVar class

			c_model_render->override_material(glow_mat);
		}
	}

	if (strstr(model_name, "weapons/v") && csgo::local_player && vars::checkbox["#cham_weapon"]->get_bool())
	{

	}

	draw_model_execute_original(ecx, edx, context, state, info, matrix);
	
	return draw_model_execute_original(ecx, edx, context, state, info, matrix);
}

void __fastcall hooks::frame_stage::hook(void* ecx, void* edx, client_frame_stage_t curstage) {

	if (c_engine->is_in_game() && c_engine->is_connected() && csgo::local_player)
	{

		if (curstage == FRAME_RENDER_START)
		{
			
			if (GetKeyState(weird_stuff::get_key("#thirdperson_key"))) // är i thirdperson
			{
			
				if(*(bool*)((uintptr_t)interfaces::input + 0xAD))
					*(vec3_t*)((uintptr_t)csgo::local_player + 0x31D8) = real_angle;

			
			}


		}

		if (curstage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (vars::checkbox["#models_enabled"]->get_bool())
				skinchanger::model_changer();

			//skinchanger::knife_changer();
		}
	}

	frame_stage_original(ecx, edx, curstage);
}


void c_events::fire_game_event(i_game_event* event)
{
	if (!event) return;
	if (!csgo::local_player) return;

	if (strstr(event->get_name(), "player_hurt"))
	{
		auto attacker = interfaces::entity_list->get_client_entity(event->get_int("attacker"));

		if (!attacker || attacker != csgo::local_player) return;

		c_surface->play_sound("buttons\\arena_switch_press_02.wav");
	}
	else if (strstr(event->get_name(), "bullet_impact"))
	{
		
	}
}