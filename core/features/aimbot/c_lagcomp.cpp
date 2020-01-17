#include "../features.hpp"
#include "../../menu/fgui_menu.hpp"

#define flow_outgoing 0
#define flow_incoming 1

float backtrack::lerp_time()
{
	
	int updaterate = c_cvar->get_convar("cl_updaterate")->get_int();
	convar* minupdate = c_cvar->get_convar("sv_minupdaterate");
	convar* maxupdate = c_cvar->get_convar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->get_int();

	float ratio = c_cvar->get_convar("cl_interp_ratio")->get_float();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = c_cvar->get_convar("cl_interp")->get_float();
	convar* cmin = c_cvar->get_convar("sv_client_min_interp_ratio");
	convar* cmax = c_cvar->get_convar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->get_float() != 1)
		ratio = std::clamp(ratio, cmin->get_float(), cmax->get_float());

	return max(lerp, (ratio / updaterate));
}


bool backtrack::valid_tick(float sim_time)
{
	auto nci = c_engine->get_net_channel_info();

	if (!nci)
		return false;

	const auto sv_maxunlag = c_cvar->get_convar("sv_maxunlag");
	
	auto Correct = std::clamp(interfaces::clientstate->net_channel->get_average_latency(flow_outgoing) + interfaces::clientstate->net_channel->get_average_latency(flow_incoming), 0.f, sv_maxunlag->get_float());
	
	return (fabs(Correct - (c_globals->cur_time - sim_time)) <= 0.2f);
}

