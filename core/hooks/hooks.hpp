#pragma once

//class bf_write;


class c_events : public i_game_event_listener2
{
public :
	void fire_game_event(i_game_event* event);
	int debug_id = 42;
	void init()
	{
		interfaces::event_manager->add_listener(this, "bullet_impact", false);
		interfaces::event_manager->add_listener(this, "player_hurt", false);
	}
};
extern c_events* g_pevents;
namespace hooks {
	bool initialize();
	void release();

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}
	namespace override_view {
		using fn = void(__fastcall*)(void*, void*, view_setup_t*);
		void __fastcall hook(void* ecx, void* edx, view_setup_t* setup);
	}
	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}
	namespace writeuser_cmd_delta {
		using fn = bool(__fastcall*)(void*, void*, int, bf_write*, int, int, bool);
		bool __fastcall hook(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool new_cmd);
	}
	namespace scene_end {
		using fn = void(__fastcall*)(void*, void*);
		void __fastcall hook(void* ecx, void* edx);
	}
	namespace in_key_event {
		using fn = int(__stdcall*)(int, int, const char*);
		int __fastcall hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding);
	}

	namespace frame_stage {
		using fn = void(__fastcall*)(void*, void*, client_frame_stage_t);
		void __fastcall hook(void* ecx, void* edx, client_frame_stage_t curstage);
	}

	namespace draw_model_execute
	{
		using fn = void(__fastcall*)(void*, void*, IMatRenderContext*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		void __fastcall hook(void* ecx, void* edx, IMatRenderContext* context, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	}
}