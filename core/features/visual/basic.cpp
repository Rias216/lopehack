#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"






void visuals::basic::esp_box_setup(player_t* player)
{
	box.bottom = 0;
	box.top = 0;
	box.left = 0;
	box.right = 0;

	vec3_t w2s_bottom, w2s_top;

	c_debug_overlay->world_to_screen(player->origin(), w2s_bottom);
	c_debug_overlay->world_to_screen(player->get_hitbox_position(0) + vec3_t(0, 0, 10), w2s_top);

	int middle = w2s_bottom.y - w2s_top.y;
	int width = middle / 4;

	box.bottom = middle;
	box.top = w2s_top.y;
	box.left = w2s_bottom.x - width;
	box.right = width * 2;
}

void visuals::basic::esp_box(player_t* player)
{
	if (!player) return;
	if (player == csgo::local_player) return;
	if (!player->is_alive()) return;
	if (player->team() == csgo::local_player->team()) return;

	
	if (player->dormant())
	{
		render::draw_outline(box.left + 1, box.top + 1, box.right - 2, box.bottom - 2, color(0, 0, 0, 70));
		render::draw_outline(box.left - 1, box.top - 1, box.right + 2, box.bottom + 2, color(0, 0, 0, 70));
	}
	else
	{
		render::draw_outline(box.left + 1, box.top + 1, box.right - 2, box.bottom - 2, color(0, 0, 0, vars::slider["#box_esp_outline"]->get_value()));
		render::draw_outline(box.left - 1, box.top - 1, box.right + 2, box.bottom + 2, color(0, 0, 0, vars::slider["#box_esp_outline"]->get_value()));
	}
	
	
	
	if (player->dormant())
		render::draw_outline(box.left, box.top, box.right, box.bottom, color(vars::colorpicker["#box_esp_color"]->get_color().m_red, vars::colorpicker["#box_esp_color"]->get_color().m_green, vars::colorpicker["#box_esp_color"]->get_color().m_blue, 70));
	else
		render::draw_outline(box.left, box.top, box.right, box.bottom, color(vars::colorpicker["#box_esp_color"]->get_color().m_red, vars::colorpicker["#box_esp_color"]->get_color().m_green, vars::colorpicker["#box_esp_color"]->get_color().m_blue, vars::colorpicker["#box_esp_color"]->get_color().m_alpha));
}


void visuals::basic::esp_name(player_t* player)
{
	static player_info_t p_info;
	if (!player) return;
	if (player == csgo::local_player) return;
	if (!player->is_alive()) return;
	if (player->team() == csgo::local_player->team()) return;
	if (!&p_info) return;
	c_engine->get_player_info(player->index(), &p_info);


	player_t* target = rage_bot::closest_enemy();

	if (target)
	{
		if (player == target)
		{
				render::draw_text_string(box.left + (box.right / 2), box.top - 25, render::fonts::watermark_font, "target", true, color::red());

		}
			
	}

	if (player->dormant())
		render::draw_text_string(box.left + (box.right / 2), box.top - 12, render::fonts::watermark_font, p_info.name, true, color(vars::colorpicker["#name_esp_color"]->get_color().m_red, vars::colorpicker["#name_esp_color"]->get_color().m_green, vars::colorpicker["#name_esp_color"]->get_color().m_blue, 70));
	else if(!player->dormant())
		render::draw_text_string(box.left + (box.right / 2), box.top - 12, render::fonts::watermark_font, p_info.name, true, color(vars::colorpicker["#name_esp_color"]->get_color().m_red, vars::colorpicker["#name_esp_color"]->get_color().m_green, vars::colorpicker["#name_esp_color"]->get_color().m_blue, vars::colorpicker["#name_esp_color"]->get_color().m_alpha));
}

void visuals::basic::esp_weapon(player_t* player)
{
	
	vec3_t screen;
	int a = 255;
	if (!player) return;
	if (player == csgo::local_player) return;
	if (!player->is_alive()) return;
	if (player->team() == csgo::local_player->team()) return;
	if (!player->active_weapon()) return;
	if (!player->active_weapon()->get_weapon_data()) return;
	if (!interfaces::debug_overlay->world_to_screen(player->vec_origin(), screen)) 
		return;
	
	if (player->dormant())
		render::draw_text_string(screen.x, screen.y, render::fonts::watermark_font, player->active_weapon()->get_weapon_data()->m_weapon_name , true, color(vars::colorpicker["#weapon_esp_color"]->get_color().m_red, vars::colorpicker["#weapon_esp_color"]->get_color().m_green, vars::colorpicker["#weapon_esp_color"]->get_color().m_blue, 70));
	else
		render::draw_text_string(screen.x, screen.y, render::fonts::watermark_font, player->active_weapon()->get_weapon_data()->m_weapon_name, true, color(vars::colorpicker["#weapon_esp_color"]->get_color().m_red, vars::colorpicker["#weapon_esp_color"]->get_color().m_green, vars::colorpicker["#weapon_esp_color"]->get_color().m_blue, vars::colorpicker["#weapon_esp_color"]->get_color().m_alpha));

}
void GradientV(int x, int y, int w, int h, color c1, color c2)
{
	render::draw_filled_rect(x, y, w, h, c1);
	
	BYTE first = c2.r;
	BYTE second = c2.g;
	BYTE third = c2.b;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		render::draw_filled_rect(x, y + i, w, 1, color(first, second, third, ia));
	}
}
void visuals::basic::esp_health_bar(player_t* player)
{
	if (!player) return;
	if (player == csgo::local_player) return;
	if (!player->is_alive()) return;
	if (player->team() == csgo::local_player->team()) return;
	color c_color;
	if (player->health() > 70 && player->health() > 69)
		c_color = color::green();
	else if (player->health() < 69 && player->health() > 30)
		c_color = color(252, 173, 3, 255);
	else
		c_color = color::red();
	int value = player->health() * box.bottom / 100;

	std::string health = std::to_string(player->health());
	if (player->dormant())
		render::draw_text_string(box.left - 15, box.top - 1, render::fonts::watermark_font, "hp " + health, true, color(255, 255, 255, 70));
	else
	{
		if (vars::combobox["#health_esp"]->get_value() == 1)
		{
			render::draw_filled_rect(box.left - 5, box.top - 1, 3, box.bottom, color(0, 0, 0, 100));
			render::draw_filled_rect(box.left - 4, box.top + box.bottom - value, 1, value, c_color);
			render::draw_outline(box.left - 5, box.top - 1, 3, box.bottom, color(0, 0, 0, 200));
			if (player->health() != 100)
				render::draw_text_string(box.left - 4, box.top + box.bottom - value + 2, render::fonts::font_smallest7, std::to_string(player->health()), true, color::white());
		}
		else if(vars::combobox["#health_esp"]->get_value() == 2)
		{
			render::draw_filled_rect(box.left - 5, box.top - 1, 3, box.bottom, color(0, 0, 0, 100)); 
			render::draw_outline(box.left - 5, box.top - 1, 3, box.bottom, color(0, 0, 0, 200));
			GradientV(box.left - 4, box.top + box.bottom - value, 1, value, color(vars::colorpicker["#health_esp_color_1"]->get_color().m_red, vars::colorpicker["#health_esp_color_1"]->get_color().m_green, vars::colorpicker["#health_esp_color_1"]->get_color().m_blue, vars::colorpicker["#health_esp_color_1"]->get_color().m_alpha), color(vars::colorpicker["#health_esp_color_2"]->get_color().m_red, vars::colorpicker["#health_esp_color_2"]->get_color().m_green, vars::colorpicker["#health_esp_color_2"]->get_color().m_blue, vars::colorpicker["#health_esp_color_2"]->get_color().m_alpha));
		}
	}
}

void visuals::basic::esp_ammobar(player_t* player)
{
	
	if (!player) return;
	if (player == csgo::local_player) return;
	if (!player->is_alive()) return;
	if (player->team() == csgo::local_player->team()) return;
	if (!player->active_weapon()) return;

}

void visuals::basic::esp_glow()
{
	if (!csgo::local_player || !c_engine->is_in_game() or !c_engine->is_connected())
		return;

	if (vars::checkbox["#glow_esp"]->get_bool())
	{
		for (auto i = 0; i < c_glow_manager->size; i++)
		{
			glow_object_definition_t& glow_object = c_glow_manager->objects[i];
			player_t* player = reinterpret_cast<player_t*>(glow_object.entity);

			if (!player)
				continue;
			if (glow_object.unused())
				continue;
			if (!player->is_player())
				continue;

			glow_object.full_bloom_render = false;

			if (player->team() != csgo::local_player->team())
			{
				int class_id = player->client_class()->class_id;

			switch (class_id)
			{
			default:
				glow_object.alpha = 0.0f;
				break;
			case class_ids::ccsplayer:
				glow_object.alpha = vars::colorpicker["#glow_color"]->get_color().m_alpha / 255.f;
				break;
			}

			glow_object.set(vars::colorpicker["#glow_color"]->get_color().m_red / 255.f, vars::colorpicker["#glow_color"]->get_color().m_green / 255.f, vars::colorpicker["#glow_color"]->get_color().m_blue / 255.f, vars::colorpicker["#glow_color"]->get_color().m_alpha / 255.f);

		    }
		}
	}
}

void visuals::basic::esp_arrows(player_t* player)
{

}