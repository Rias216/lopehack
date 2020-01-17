#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"

void visuals::visual_main::esp_render()
{

	for (int i = 1; i <= 64; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		
		visuals::basic::esp_box_setup(entity);
		
		
		if (vars::checkbox["#name_esp"]->get_bool())
			visuals::basic::esp_name(entity);

		if (vars::checkbox["#weapon_esp"]->get_bool())
			visuals::basic::esp_weapon(entity);

		if (vars::combobox["#health_esp"]->get_value() > 0)
			visuals::basic::esp_health_bar(entity);

		if (vars::checkbox["#box_esp"]->get_bool())
			visuals::basic::esp_box(entity);

		if (vars::multibox["#removals"]->get_selected(0))
			visuals::local::local_scope();

		
	} 
}