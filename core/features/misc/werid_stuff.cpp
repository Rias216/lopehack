#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"

int weird_stuff::get_key(std::string key_value)
{
	if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_B)
		return 0x42;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_A)
		return 0x41;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_C)
		return 0x43;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_A)
		return 0x41;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_CAPSLOCK)
		return VK_CAPITAL;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_CAPSLOCKTOGGLE)
		return VK_CAPITAL;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_D)
		return 0x44;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_DOWN)
		return VK_DOWN;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_E)
		return 0x45;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_F)
		return 0x46;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_G)
		return 0x47;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_H)
		return 0x48;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_I)
		return 0x49;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_J)
		return 0x4A;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_K)
		return 0x4B;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_L)
		return 0x4C;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_LALT)
		return VK_MENU;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_LCONTROL)
		return VK_CONTROL;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_LEFT)
		return VK_LEFT;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_LSHIFT)
		return VK_SHIFT;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_M)
		return 0x4D;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_N)
		return 0x4E;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_O)
		return 0x4F;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_P)
		return 0x50;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_PAGEDOWN)
		return VK_NEXT;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_PAGEUP)
		return VK_PRIOR;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_Q)
		return 0x51;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_SCROLLLOCK)
		return VK_MBUTTON;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_SCROLLLOCKTOGGLE)
		return VK_MBUTTON;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_T)
		return 0x54;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_UP)
		return VK_UP;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_V)
		return 0x56;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::KEY_X)
		return 0x58;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::MOUSE_4)
		return VK_XBUTTON1;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::MOUSE_5)
		return VK_XBUTTON2;
	else if (vars::keybinder[key_value]->get_key() == fgui::external::MOUSE_MIDDLE)
		return VK_MBUTTON;

}