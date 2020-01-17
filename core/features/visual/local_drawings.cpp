#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"

void visuals::local::local_scope()
{
	if (!csgo::local_player->is_alive()) return;
	if (!csgo::local_player->is_scoped()) return;

	int width, height;
	c_engine->get_screen_size(width, height);

	render::draw_filled_rect(0, height / 2, width, 1, color::black());
	render::draw_filled_rect(width / 2, 0, 1, height, color::black());
}

auto load_sky = reinterpret_cast<void(__fastcall*)(const char*)>(utilities::pattern_scan(GetModuleHandleA("engine.dll"), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));

void visuals::local::nightmode()
{
	static std::string OldSkyname = "";
	static int OldNightmode;
	
	static int OldSky;
	if (!c_engine->is_connected() || !c_engine->is_in_game()) {
		OldSkyname = "";
		OldNightmode = 100;
		
		OldSky = 0;
		
		
		return;
	}
	static convar* r_DrawSpecificStaticProp;
	convar* bright;
	bright = c_cvar->get_convar("mat_fullbright");
	if (vars::checkbox["#full_bright"]->get_bool())
		bright->set_value(1);
	else
		bright->set_value(0);

	if (OldNightmode != vars::slider["#nightmode"]->get_value())
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = c_cvar->get_convar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->set_value(0);

		for (MaterialHandle_t i = c_mat_system->first_material(); i != c_mat_system->invalid_material_handle(); i = c_mat_system->next_material(i))
		{
			i_material* pMaterial = c_mat_system->get_material(i);
			if (!pMaterial)
				continue;
			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (vars::slider["#nightmode"]->get_value() < 100 || vars::checkbox["#skybox"])
				{
					if (vars::slider["#nightmode"]->get_value() < 100)
						load_sky("sky_csgo_night02");
					if (strstr(pMaterial->GetTextureGroupName(), "World"))
						pMaterial->color_modulate(vars::slider["#nightmode"]->get_value() / 100.f, vars::slider["#nightmode"]->get_value() / 100.f, vars::slider["#nightmode"]->get_value() / 100.f);

					if(strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
						pMaterial->color_modulate(1.f, 1.f, 1.f);

					if (strstr(pMaterial->GetTextureGroupName(), "Model"))
						pMaterial->color_modulate(1.0f, 1.0f, 1.0f);
				
					if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
						pMaterial->color_modulate(vars::colorpicker["#skybox_color"]->get_color().m_red / 255.f, vars::colorpicker["#skybox_color"]->get_color().m_green / 255.f, vars::colorpicker["#skybox_color"]->get_color().m_blue / 255.f);
				}
				else
					pMaterial->color_modulate(1.0f, 1.0f, 1.0f);
			}
		}
		OldNightmode = vars::slider["#nightmode"]->get_value();
		
	}
}

void visuals::local::watermark()
{

	int x, y;

	c_engine->get_screen_size(x, y);

	render::draw_filled_rect((x - 140), 25, 130, 40, color::black());
	render::draw_text_string(x - 135, 30, render::fonts::porter, "L", false, color(/*244*/0, 64, 244));
	render::draw_filled_rect((x - 300), 25, 2, 20, color(0, 64, 244));





	/*
	//F  
	render::draw_filled_rect(500, 150, 40, 40, color(0, 3, 28));
	
	render::draw_filled_rect(500, 150, 40, 40, color(0, 3, 56, 255));
	render::draw_text_string(514, 157, render::fonts::porter, "L", false, color(0, 64, 244));

	render::draw_outline(505, 155, 30, 30,  color(0, 3, 56));
	//FPS/PING/SPEED/TICK
	render::draw_filled_rect(337, 150, 160, 40, color(0, 3, 28));
	render::draw_outline(337, 150, 160, 40, color(0, 3, 56));
	render::draw_outline(342, 155, 150, 30, color(0, 3, 56));
	render::draw_text_string(352, 170, render::fonts::watermark_font, "fps       ping      speed     tick", false, color(255, 255, 255));
	render::draw_text_string(350, 158, render::fonts::watermark_font, "              41           0          64", false,  color(166, 228, 0));
	render::draw_text_string(350, 158, render::fonts::font_smallest7, "num",false, color(166, 228, 0));
	//TIME
	render::draw_filled_rect(245, 150, 89, 40, color(0, 3, 28));
	render::draw_outline(245, 150, 89, 40, color(0, 3, 56));
	render::draw_outline(250, 155, 79, 30, color(0, 3, 56));*/
	//render::draw_text_string(258, 170, color(255, 255, 255), &text, true, false, "current time");
	//render::draw_text_string(270, 158, color(0, 142, 222), &numb, true, false, "21:40:55");
}

void visuals::local::indicators()
{
	if (!c_engine->is_in_game() || !c_engine->is_connected()) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;
	if (!vars::checkbox["#show_indicators"]->get_bool()) return;
	int screen_y, screen_x;
	c_engine->get_screen_size(screen_x, screen_y);


	render::draw_text_string(10, screen_y / 2, render::fonts::visitor, "fakelag", false, color::white());
	render::draw_gradient_rect_vertical(10, screen_y / 2 + 14, interfaces::engine->get_net_channel_info()->choked_packets * 4 + 10, 4, color::blue(), color::red());
	render::draw_outline(10, screen_y / 2 + 14, 75, 5, color::black());
	render::draw_filled_rect(10, screen_y / 2 + 14, 74, 4, color::black(180));

	render::draw_text_string(1, screen_y / 2 + 28, render::fonts::watermark_font, "lby : " + std::to_string(csgo::local_player->lower_body_yaw()), false, color::green());
	render::draw_text_string(1, screen_y / 2 + 42, render::fonts::watermark_font, "eye : " + std::to_string(csgo::local_player->eye_angles().y), false, color::green());
	if (csgo::local_player->active_weapon() != nullptr)
		render::draw_text_string(1, screen_y / 2 + 56, render::fonts::watermark_font, "in accuracy : " + std::to_string((csgo::local_player->active_weapon()->inaccuracy() * 1000.f)), false, color::green());

	if (csgo::sendpacket)
		render::draw_text_string(1, screen_y / 2 + 70, render::fonts::watermark_font, "sendpacket", false, color::green());
	else
		render::draw_text_string(1, screen_y / 2 + 70, render::fonts::watermark_font, "sendpacket", false, color::red());

	render::draw_text_string(1, screen_y / 2 + 84, render::fonts::watermark_font, "goal feet : " + std::to_string(csgo::local_player->get_anim_state()->goal_feet_yaw), false, color::green());

	render::draw_text_string(1, screen_y / 2 + 98, render::fonts::watermark_font, "max rotation : " + std::to_string(csgo::local_player->max_rotation(csgo::local_player)), false, color::green());
}