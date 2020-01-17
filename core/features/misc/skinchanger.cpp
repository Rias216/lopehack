#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"

const char* models_to_change[] = {
		("models/player/custom_player/legacy/tm_balkan_varianta.mdl"),
		("models/player/custom_player/legacy/ctm_fbi_variantb.mdl"),
		("models/player/custom_player/legacy/ctm_heavy.mdl"),
		("models/player/custom_player/legacy/ctm_st6.mdl"),
		("models/player/custom_player/legacy/tm_balkan_variantg.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_variantf.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_variantg.mdl"),
		("models/player/custom_player/legacy/tm_phoenix_varianth.mdl"),
};




void skinchanger::model_changer()
{
	if (!c_engine->is_in_game() || !c_engine->is_connected()) return;
	if (!csgo::local_player) return;

	if (csgo::local_player->is_player()) {
		auto model_index = interfaces::model_info->get_model_index(models_to_change[vars::combobox["#model_type"]->get_value()]);

		if (model_index)
			csgo::local_player->set_model_index(model_index);
	}
}