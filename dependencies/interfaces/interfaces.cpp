#include "interfaces.hpp"
#include "../common_includes.hpp"

i_base_client_dll* interfaces::client = nullptr;
i_input* interfaces::input = nullptr;
i_client_entity_list* interfaces::entity_list = nullptr;
iv_engine_client* interfaces::engine = nullptr;
i_client_mode* interfaces::clientmode = nullptr;
i_client_state* interfaces::clientstate = nullptr;
i_panel* interfaces::panel = nullptr;
i_surface* interfaces::surface = nullptr;
c_global_vars_base* interfaces::globals = nullptr;
i_material_system* interfaces::material_system = nullptr;
iv_model_info* interfaces::model_info = nullptr;
iv_model_render* interfaces::model_render = nullptr;
i_render_view* interfaces::render_view = nullptr;
i_console* interfaces::console = nullptr;
i_localize* interfaces::localize = nullptr;
i_game_event_manager2* interfaces::event_manager = nullptr;
i_inputsytem* interfaces::inputsystem = nullptr;
iv_debug_overlay* interfaces::debug_overlay = nullptr;
IDirect3DDevice9* interfaces::directx = nullptr;
trace* interfaces::trace_ray = nullptr;
glow_manager_t* interfaces::glow_manager = nullptr;
player_game_movement* interfaces::game_movement = nullptr;
player_prediction* interfaces::prediction = nullptr;
player_move_helper* interfaces::move_helper = nullptr;
i_weapon_system* interfaces::weapon_system = nullptr;
i_surface_props* interfaces::surface_data = nullptr;
IMemAlloc* interfaces::mem_alloc = nullptr;

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>("client_panorama.dll", "VClient018");
	entity_list = get_interface<i_client_entity_list, interface_type::index>("client_panorama.dll", "VClientEntityList003");
	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	event_manager = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	inputsystem = get_interface<i_inputsytem, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	trace_ray = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	surface_data = get_interface<i_surface_props, interface_type::index>("Vphysics.dll", "VPhysicsSurfaceProps001"); 
	game_movement = get_interface<player_game_movement, interface_type::index>("client_panorama.dll", "GameMovement001");
	prediction = get_interface<player_prediction, interface_type::index>("client_panorama.dll", "VClientPrediction001");

	/*custom interfaces*/
	clientmode = **reinterpret_cast<i_client_mode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 5);
	globals = **reinterpret_cast<c_global_vars_base * **>((*reinterpret_cast<uintptr_t * *>(client)[0] + 27));
	mem_alloc = *reinterpret_cast<IMemAlloc**> (GetProcAddress(GetModuleHandleA("tier0.dll"), ("g_pMemAlloc")));

	clientstate = **(i_client_state ***)(utilities::pattern_scan(GetModuleHandleA("engine.dll"), sig_client_state) + 1);
	directx = **(IDirect3DDevice9***)(utilities::pattern_scan(GetModuleHandleA("shaderapidx9.dll"), sig_directx) + 1);
	input = *(i_input**)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), sig_input) + 1);
	glow_manager = (glow_manager_t*)(*(uintptr_t*)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), sig_glow_manager) + 3));
	move_helper = **(player_move_helper***)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), sig_player_move_helper) + 2);
	weapon_system = *(i_weapon_system**)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), sig_weapon_data) + 2);


	std::ofstream("csgo\\materials\\regular_reflective.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
	std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_reflective.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$envmap"       "env_cubemap"
  "$model" "1"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast" "1"
  "$flat" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$nofog" "0"
  "$ignorez" "0"
  "$znearer" "0"
  "$wireframe" "0"
}
)#";
	std::ofstream("csgo\\materials\\simple_reflectiveignorez.vmt") << R"#("VertexLitGeneric"
{
      "$basetexture" "vgui/white_additive"
      "$ignorez" "1"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmaptint" "[1 1 1]"
      "$envmapcontrast" "1"
      "$envmapsaturation" "1.0"
      "$phong" "1"
      "$phongexponent" "30.0"
      "$phongboost"     "10.0"
      "$normalmapalphaenvmask" "1"
      "$nofog" "1"
      "$reflectivity" "[.5 .5 .5]"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1"
}
)#";
	std::ofstream("csgo/materials/glowOverlay.vmt") << R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
    "$ignorez" "0"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";
	std::ofstream("csgo/materials/glowOverlay_ignorez.vmt") << R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
    "$ignorez" "1"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";

	console::log("[setup] interfaces initialized!\n");

	return true;
}