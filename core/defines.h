#pragma once

#include "../dependencies/common_includes.hpp"

#define c_surface interfaces::surface
#define c_engine interfaces::engine
#define c_debug_overlay interfaces::debug_overlay
#define c_cvar interfaces::console
#define c_model_render interfaces::model_render
#define c_mat_system interfaces::material_system
#define c_render_view interfaces::render_view
#define c_glow_manager interfaces::glow_manager
#define c_globals interfaces::globals
#define time_to_ticks(dt) ((int)(0.5f + (float)(dt) / c_globals->interval_per_tick))
#define c_min(a,b)            (((a) < (b)) ? (a) : (b))
#define c_max(a,b)            (((a) > (b)) ? (a) : (b))
#define localplayer csgo::local_player