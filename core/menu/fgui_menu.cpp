#include "fgui_menu.hpp"


void save_config()
{
	if (vars::combobox["#config_type"]->get_value() == 0)
		vars::container["#window"]->save_config("C://lopeware//config_0.json");
	else if(vars::combobox["#config_type"]->get_value() == 1)
		vars::container["#window"]->save_config("C://lopeware//config_1.json");
	else if (vars::combobox["#config_type"]->get_value() == 2)
		vars::container["#window"]->save_config("C://lopeware//config_2.json");
	else if (vars::combobox["#config_type"]->get_value() == 3)
		vars::container["#window"]->save_config("C://lopeware//config_3.json");
	else if (vars::combobox["#config_type"]->get_value() == 4)
		vars::container["#window"]->save_config("C://lopeware//config_4.json");
	else if (vars::combobox["#config_type"]->get_value() == 5)
		vars::container["#window"]->save_config("C://lopeware//config_5.json");
}

void load_config()
{
	if (vars::combobox["#config_type"]->get_value() == 0)
		vars::container["#window"]->load("C://lopeware//config_0.json");
	else if (vars::combobox["#config_type"]->get_value() == 1)
		vars::container["#window"]->load("C://lopeware//config_1.json");
	else if (vars::combobox["#config_type"]->get_value() == 2)
		vars::container["#window"]->load("C://lopeware//config_2.json");
	else if (vars::combobox["#config_type"]->get_value() == 3)
		vars::container["#window"]->load("C://lopeware//config_3.json");
	else if (vars::combobox["#config_type"]->get_value() == 4)
		vars::container["#window"]->load("C://lopeware//config_4.json");
	else if (vars::combobox["#config_type"]->get_value() == 5)
		vars::container["#window"]->load("C://lopeware//config_5.json");
}
void gui::initialize() {
	fgui::element_font title_font = { "Tahoma", 11, fgui::external::font_flags::SHADOW , false }; // Tahoma
	REGISTER_NOTIFICATIONS(title_font);
	ADD_WINDOW(vars::container["#window"], 50, 50, "lopeware recode -alpha-", 573, 680, fgui::external::key_code::KEY_INSERT, title_font);
	REGISTER_CURSOR(fgui::cursor_type::ARROW, fgui::input_state::UNLOCKED);

	REGISTER_TAB(vars::tabs["#tab_panel"], 7, 1, title_font, vars::container["#window"], -1);

	
	ADD_TAB(vars::tabs["#tab_panel"], "rage");
	{
		ADD_GROUPBOX(vars::container["#rage_main"], 15, 40, "main", 231, 370, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_main"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#ragehehe"], 15, 15, "enabled", "vars.testingoption", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#no_recoil"], 15, 35, "remove recoil", "vars.recoil", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#auto_shoot"], 15, 55, "auto shoot", "vars.autoshoot", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#silent_aim"], 15, 70, "silent aim", "vars.silnetaim", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#limit_spread"], 15, 90, "limit spread", "vars.limitspread", title_font, vars::container["#rage_main"], 0);
		ADD_TOOLTIP(vars::checkbox["#limit_spread"], "enable this more more accuracy");/*
		ADD_MULTIBOX(vars::multibox["#hit_boxes"], 15, 120, "hitboxes", "vars.hitscan", title_font, vars::container["#rage_main"], 0);
		ADD_ITEM(vars::multibox["#hit_boxes"], "head", false);
		ADD_ITEM(vars::multibox["#hit_boxes"], "chest", false);
		ADD_ITEM(vars::multibox["#hit_boxes"], "stomach", false);
		ADD_ITEM(vars::multibox["#hit_boxes"], "pelvis", false);
		ADD_ITEM(vars::multibox["#hit_boxes"], "legs", false);
		ADD_ITEM(vars::multibox["#hit_boxes"], "feet", false);*/
		ADD_COMBOBOX(vars::combobox["#hitscan_mode"], 15, 150, "hitscan mode", "vars.hitscan_mode", title_font, vars::container["#rage_main"], 0);
		ADD_ITEM(vars::combobox["#hitscan_mode"], "prefer head", 0);
		ADD_ITEM(vars::combobox["#hitscan_mode"], "prefer body", 1);
		ADD_CHECKBOX(vars::checkbox["#auto_stop"], 15, 180, "auto stop", "vars.stop_auto", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#rage_backtrack"], 15, 200, "backtrack", "vars.rage_backtrack", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#auto_scope"], 15, 220, "auto scope", "vars.autoscope", title_font, vars::container["#rage_main"], 0);
		ADD_CHECKBOX(vars::checkbox["#double_tap"], 15, 350, "double tap", "vars.dt", title_font, vars::container["#rage_main"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_auto"], 280, 40, "weapon - auto", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_auto"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_auto_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_auto", title_font, vars::container["#rage_weapon_auto"], 0);
		ADD_SLIDER(vars::slider["#weapon_auto_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_auto", title_font, vars::container["#rage_weapon_auto"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_scout"], 280, 150, "weapon - scout", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_scout"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_scout_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_scout", title_font, vars::container["#rage_weapon_scout"], 0);
		ADD_SLIDER(vars::slider["#weapon_scout_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_scout", title_font, vars::container["#rage_weapon_scout"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_awp"], 280, 260, "weapon - awp", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_awp"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_awp_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_awp", title_font, vars::container["#rage_weapon_awp"], 0);
		ADD_SLIDER(vars::slider["#weapon_awp_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_awp", title_font, vars::container["#rage_weapon_awp"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_heavy"], 280, 370, "weapon - heavy", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_heavy"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_heavy_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_heavy", title_font, vars::container["#rage_weapon_heavy"], 0);
		ADD_SLIDER(vars::slider["#weapon_heavy_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_heavy", title_font, vars::container["#rage_weapon_heavy"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_pistol"], 280, 480, "weapon - pistol", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_pistol"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_pistol_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_pistol", title_font, vars::container["#rage_weapon_pistol"], 0);
		ADD_SLIDER(vars::slider["#weapon_pistol_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_pistol", title_font, vars::container["#rage_weapon_pistol"], 0);


		ADD_GROUPBOX(vars::container["#rage_weapon_other"], 15, 480, "weapon - other", 200, 100, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#rage_weapon_other"], vars::tabs["#tab_panel"]);
		ADD_SLIDER(vars::slider["#weapon_other_hitchance"], 15, 15, "hitchance", 0, 0, 100, "vars.hitchance_other", title_font, vars::container["#rage_weapon_other"], 0);
		ADD_SLIDER(vars::slider["#weapon_other_mindmg"], 15, 40, "min damage", 0, 0, 150, "vars.mindmg_other", title_font, vars::container["#rage_weapon_other"], 0);



	}

	ADD_TAB(vars::tabs["#tab_panel"], "antiaim")
	{
		ADD_GROUPBOX(vars::container["#aa_main"], 15, 40, "angles", 231, 270, title_font, vars::container["#window"], 1, false, false, false);
		ADD_CONTROLLER(vars::container["#aa_main"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#aa_enable"], 15, 15, "enable", "vars.aaenable", title_font, vars::container["#aa_main"], 1);
		ADD_COMBOBOX(vars::combobox["#aa_pitch"], 15, 40, "pitch", "vars.pitch", title_font, vars::container["#aa_main"], 1);
		ADD_ITEM(vars::combobox["#aa_pitch"], "off", 0);
		ADD_ITEM(vars::combobox["#aa_pitch"], "default", 1);
		ADD_ITEM(vars::combobox["#aa_pitch"], "up", 2);
		ADD_COMBOBOX(vars::combobox["#aa_yaw"], 15, 70, "yaw", "vars.yaw", title_font, vars::container["#aa_main"], 1);
		ADD_ITEM(vars::combobox["#aa_yaw"], "off", 0);
		ADD_ITEM(vars::combobox["#aa_yaw"], "backwards", 1);
		ADD_COMBOBOX(vars::combobox["#aa_fakeyaw"], 15, 100, "fake yaw", "vars.fakeyaw", title_font, vars::container["#aa_main"], 1);
		ADD_ITEM(vars::combobox["#aa_fakeyaw"], "off", 0);
		ADD_ITEM(vars::combobox["#aa_fakeyaw"], "static", 1);
		//ADD_ITEM(vars::combobox["#aa_fakeyaw"], "lby", 2);
		//ADD_ITEM(vars::combobox["#aa_fakeyaw"], "jitter", 3);
		ADD_SLIDER(vars::slider["#aa_fakeyaw_amount"], 15, 120, "degrees", 0, 0, 60, "vars.fakeyawamount", title_font, vars::container["#aa_main"], 1);
		ADD_KEYBINDER(vars::keybinder["#aa_fakeyaw_invert"], 15, 150, "switch side", fgui::external::KEY_LALT, "vars.fakeinvert", title_font, vars::container["#aa_main"], 1);
		


		ADD_GROUPBOX(vars::container["#aa_fakelag"], 250, 40, "fakelag", 231, 300, title_font, vars::container["#window"], 1, false, false, false);
		ADD_CONTROLLER(vars::container["#aa_fakelag"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#enable_fakelag"], 15, 15, "enable", "vars.fakelag", title_font, vars::container["#aa_fakelag"], 1);
		ADD_COMBOBOX(vars::combobox["#fakelag_type"], 15, 40, "type", "vars.fakelag_type", title_font, vars::container["#aa_fakelag"], 1);
		ADD_ITEM(vars::combobox["#fakelag_type"], "factor", 0);
		ADD_ITEM(vars::combobox["#fakelag_type"], "random", 1);
		ADD_SLIDER(vars::slider["#fakelag_value"], 15, 70, "choke limit", 0, 0, 16, "vars.choke_limit", title_font, vars::container["#aa_fakelag"], 1);
		ADD_CHECKBOX(vars::checkbox["#choke_shot"], 15, 90, "on shot", "vars.on_shot", title_font, vars::container["#aa_fakelag"], 1);
		ADD_CHECKBOX(vars::checkbox["#choke_stand"], 15, 110, "on stand", "vars.on_stand", title_font, vars::container["#aa_fakelag"], 1);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "legit"); {
		ADD_GROUPBOX(vars::container["#groupbox"], 15, 40, "group box 1", 260, 370, title_font, vars::container["#window"], 2, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#checkbox"], 15, 15, "test checkbox", "vars.checkbox", title_font, vars::container["#groupbox"], 2);

		ADD_GROUPBOX(vars::container["#groupbox1"], 285, 40, "group box 2", 260, 370, title_font, vars::container["#window"], 2, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox1"], vars::tabs["#tab_panel"]);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "visuals"); {
		ADD_GROUPBOX(vars::container["#groupbox3"], 15, 40, "player", 260, 600/*185*/, title_font, vars::container["#window"], 3, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox3"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#box_esp"], 15, 15, "box", "vars.esp_box", title_font, vars::container["#groupbox3"], 1);
		ADD_COLORPICKER(vars::colorpicker["#box_esp_color"], 100, 15, fgui::color(255, 255, 255), "vars.box_esp_color", title_font, vars::container["#groupbox3"], 3);
		ADD_SLIDER(vars::slider["#box_esp_outline"], 15, 45, "outline alhpa", 255, 0, 255, "vars.esp_box_outline", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#name_esp"], 15, 65, "name", "vars.esp_name", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#name_esp_color"], 100, 65, fgui::color(255, 255, 255), "vars.box_esp_color", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#weapon_esp"], 15, 85, "weapon", "vars.esp_weapon", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#weapon_esp_color"], 100, 85, fgui::color(255, 255, 255), "vars.box_esp_color", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#esp_ammobar"], 15, 110, "ammo bar", "vars.ammo_bar", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#health_esp"], 15, 140, "health", "vars.esp_health", title_font, vars::container["#groupbox3"], 3)
		ADD_ITEM(vars::combobox["#health_esp"], "off", 0);
		ADD_ITEM(vars::combobox["#health_esp"], "normal", 1);
		ADD_ITEM(vars::combobox["#health_esp"], "gradient", 2);
		ADD_COLORPICKER(vars::colorpicker["#health_esp_color_1"], 15, 170, fgui::color(48, 10, 195), "vars.health_esp_color_1", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#health_esp_color_2"], 40, 170, fgui::color(6, 209, 212), "vars.health_esp_color_2", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#glow_esp"], 15, 190, "glow", "vars.glow_esp", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#glow_color"], 100, 190, fgui::color(255, 255, 255, 96), "vars.glow_color", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#chams_enemy"], 15, 210, "enemys visible", "vars.enemy_chams", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#enemy_chams_color"], 140, 210, fgui::color(255, 255, 255, 255), "vars.enemy_cham_color", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#chams_enemy_invis"], 15, 240, "enemys invisible", "vars.enemy_chams_invis", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#enemy_cham_invis_color"], 140, 240, fgui::color(255, 255, 255, 255), "vars.enemy_chams_invis_col", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#enemy_cham_type"], 15, 270, "material", "vars.enemy_cham_mat", title_font, vars::container["#groupbox3"], 3);
		ADD_ITEM(vars::combobox["#enemy_cham_type"], "textured", 0);
		ADD_ITEM(vars::combobox["#enemy_cham_type"], "flat", 1);
		ADD_ITEM(vars::combobox["#enemy_cham_type"], "metallic", 2);
		ADD_ITEM(vars::combobox["#enemy_cham_type"], "glow", 3);
		ADD_CHECKBOX(vars::checkbox["#shine_enemy"], 15, 300, "modulate shine color(wip)", "vars.mod_shine", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#shine_enemy_color"], 130, 300, fgui::color(255, 255, 255, 255), "vars.shine_color", title_font, vars::container["#groupbox3"], 3);
		ADD_CHECKBOX(vars::checkbox["#chams_local"], 15, 320, "local chams", "vars.local_chams", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#chams_local_color"], 100, 320, fgui::color(255, 255, 255, 255), "vars.local_cham_color", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#cham_local_type"], 15, 350, "local material", "vars.local_mat", title_font, vars::container["#groupbox3"], 3);
		ADD_ITEM(vars::combobox["#cham_local_type"], "textured", 0);
		ADD_ITEM(vars::combobox["#cham_local_type"], "flat", 1);
		ADD_ITEM(vars::combobox["#cham_local_type"], "glow", 2);
		ADD_CHECKBOX(vars::checkbox["#cham_desync"], 15, 370, "desync chams", "vars.desync_chams", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#cham_desync_color"], 100, 370, fgui::color(255, 255, 255, 180), "vars_desync_cham_col", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#cham_desync_mat"], 15, 400, "desync cham material", "vars.desync_mat", title_font, vars::container["#groupbox3"], 3);
		ADD_ITEM(vars::combobox["#cham_desync_mat"], "texured", 0);
		ADD_ITEM(vars::combobox["#cham_desync_mat"], "flat", 1);
		ADD_CHECKBOX(vars::checkbox["#cham_hand"], 15, 430, "hand chams", "vars.handchams", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#hand_color"], 100, 430, fgui::color(64, 5, 81, 255), "vars.hand_color", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#hand_color_2"], 150, 430, fgui::color(49, 18, 18, 255), "vars.hand_col2", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#hand_mat"], 15, 470, "hand material", "vars.hand_mat", title_font, vars::container["#groupbox3"], 3);
		ADD_ITEM(vars::combobox["#hand_mat"], "textured", 0);
		ADD_ITEM(vars::combobox["#hand_mat"], "flat", 1);
		ADD_ITEM(vars::combobox["#hand_mat"], "glow", 2);
		ADD_ITEM(vars::combobox["#hand_mat"], "glow + textured", 3);
		ADD_CHECKBOX(vars::checkbox["#cham_weapon"], 15, 500, "weapon chams", "vars.weaponchams", title_font, vars::container["#groupbox3"], 3);
		ADD_COLORPICKER(vars::colorpicker["#weapon_color"], 100, 500, fgui::color(255, 255, 255, 255), "vars.weaponcolor", title_font, vars::container["#groupbox3"], 3);
		ADD_COMBOBOX(vars::combobox["#weaponmat"], 15, 530, "weapon material", "vars.weapon_mat", title_font, vars::container["#groupbox3"], 3);
		ADD_ITEM(vars::combobox["#weaponmat"], "textured", 0);
		ADD_ITEM(vars::combobox["#weaponmat"], "flat", 1);
		ADD_ITEM(vars::combobox["#weaponmat"], "glow", 2);
		ADD_ITEM(vars::combobox["#weaponmat"], "glow + textured", 3);


		ADD_GROUPBOX(vars::container["#more_visuals"], 285, 40, "effects", 260, 600, title_font, vars::container["#window"], 3, false, false, false);
		ADD_CONTROLLER(vars::container["#more_visuals"], vars::tabs["#tab_panel"]);
		ADD_KEYBINDER(vars::keybinder["#thirdperson_key"], 15, 30, "thirdperson", fgui::external::KEY_V, "vars.key_thirdperson", title_font, vars::container["#more_visuals"], 3);
		ADD_SLIDER(vars::slider["#view_fov"], 15, 70, "view fov", 0, 0, 60, "vars.view_fov", title_font, vars::container["#more_visuals"], 3);
		ADD_CHECKBOX(vars::checkbox["#local_scoped"], 15, 90, "blend when scope", "vars.hehehehehe", title_font, vars::container["#more_visuals"], 3);
		ADD_MULTIBOX(vars::multibox["#removals"], 15, 120, "removals", "vars.removals", title_font, vars::container["#more_visuals"], 3);
		ADD_ITEM(vars::multibox["#removals"], "scope", 0);
		ADD_ITEM(vars::multibox["#removals"], "zoom", 0);
		
		ADD_SLIDER(vars::slider["#nightmode"], 15, 160, "nightmode", 100, 0, 100, "vars.nightmode", title_font, vars::container["#more_visuals"], 3);
		ADD_CHECKBOX(vars::checkbox["#full_bright"], 15, 175, "full bright", "vars.fullbight", title_font, vars::container["#more_visuals"], 3);
		ADD_SLIDER(vars::slider["#prop_trans"], 15, 200, "transparent props", 100, 0, 100, "vars.tran_prop", title_font, vars::container["#more_visuals"], 3);
		ADD_CHECKBOX(vars::checkbox["#skybox"], 15, 220, "modulate skybox", "vars.sky", title_font, vars::container["#more_visuals"], 3);
		ADD_COLORPICKER(vars::colorpicker["#skybox_color"], 100, 220, fgui::color(255, 255, 255, 255), "vars.skycolor", title_font, vars::container["#more_visuals"], 3);
		ADD_CHECKBOX(vars::checkbox["#bullet_impact"], 15, 240, "bullet impacts", "vars.impact", title_font, vars::container["#more_visuals"], 3);

	}

	ADD_TAB(vars::tabs["#tab_panel"], "misc"); {
		ADD_GROUPBOX(vars::container["#groupbox4"], 15, 40, "main", 260, 370, title_font, vars::container["#window"], 4, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox4"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#misc_bhop"], 15, 15, "auto hop", "vars.bhop_misc", title_font, vars::container["#groupbox4"], 4);
		ADD_CHECKBOX(vars::checkbox["#auto_strafer"], 15, 35, "auto strafe", "vars.auto_strafe", title_font, vars::container["#groupbox4"], 4);
		ADD_CHECKBOX(vars::checkbox["#no_croch"], 15, 55, "remove duck cooldown", "vars.no_crouch", title_font, vars::container["#groupbox4"], 4);
		ADD_CHECKBOX(vars::checkbox["#show_indicators"], 15, 80, "indicators", "vars.indicators", title_font, vars::container["#groupbox4"], 4);
		ADD_CHECKBOX(vars::checkbox["#slide"], 15, 100, "slide", "vars.slide", title_font, vars::container["#groupbox4"], 4);
		ADD_GROUPBOX(vars::container["#groupbox_setings"], 285, 40, "settings", 260, 370, title_font, vars::container["#window"], 4, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox_setings"], vars::tabs["#tab_panel"]);
		ADD_COMBOBOX(vars::combobox["#config_type"], 15, 20, "config", "vars.type_config", title_font, vars::container["#groupbox_setings"], 4);
		ADD_ITEM(vars::combobox["#config_type"], "1", 0);
		ADD_ITEM(vars::combobox["#config_type"], "2", 1);
		ADD_ITEM(vars::combobox["#config_type"], "3", 2);
		ADD_ITEM(vars::combobox["#config_type"], "4", 3);
		ADD_ITEM(vars::combobox["#config_type"], "5", 4);
		ADD_ITEM(vars::combobox["#config_type"], "6", 5);

		ADD_BUTTON(vars::button["#config_save"], 15, 50, "save", 60, 20, title_font, vars::container["#groupbox_setings"], 4);
		ADD_FUNCTION(vars::button["#config_save"], save_config);
		ADD_BUTTON(vars::button["#config_load"],  130, 50, "load", 60, 20, title_font, vars::container["#groupbox_setings"], 4);
		ADD_FUNCTION(vars::button["#config_load"], load_config);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "skins"); {
		ADD_GROUPBOX(vars::container["#model"], 15, 40, "model", 260, 370, title_font, vars::container["#window"], 5, false, false, false);
		ADD_CONTROLLER(vars::container["#model"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#models_enabled"], 15, 15, "change model", "vars.model_change", title_font, vars::container["#model"], 5);
		ADD_COMBOBOX(vars::combobox["#model_type"], 15, 40, "model type", "vars.modeltype", title_font, vars::container["#model"], 5);
		ADD_ITEM(vars::combobox["#model_type"], "balkan", 0);
		ADD_ITEM(vars::combobox["#model_type"], "fbi", 1);
		ADD_ITEM(vars::combobox["#model_type"], "heavy", 2);
		ADD_ITEM(vars::combobox["#model_type"], "st6", 3);
		ADD_ITEM(vars::combobox["#model_type"], "cool guy", 4);
		ADD_ITEM(vars::combobox["#model_type"], "balkan2", 5);
		ADD_ITEM(vars::combobox["#model_type"], "phoenix", 6);



	}
}