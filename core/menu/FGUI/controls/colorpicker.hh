/*                                       *
 * fgui - an extensive oop gui framework *
 *                                       */

#pragma once

// framework includes
#include "element.hh"
#include "../dependencies/color.hh"

namespace fgui {

	class colorpicker : public fgui::element {
	public:
		colorpicker();

		// draw the element
		void draw();

		// set a custom color for the color picker
		inline void set_color(const int &red, const int &green, const int &blue, const int &alpha = 255) noexcept {

			m_color = { red, green, blue, alpha };
		}

		inline void set_color(const fgui::color &color) noexcept {

			m_color = color;
		}

		// returns the current selected color
		inline fgui::color get_color() const noexcept {

			return m_color;
		}

		// sets a custom state for the color picker (if the color pallet is opened or not)
		inline void set_state(const fgui::state &state) noexcept {

			m_opened = state;
		}

		// returns the current state of the color picker (if the color pallet is opened or not)
		inline fgui::state get_state() const noexcept {

			return m_opened;
		}

		// handle keyboard and mouse input
		void handle_input();

		// handle the element updates
		void update();

		// element tooltip
		void tooltip();

		// save the element state
		void save(nlohmann::json& json_module);

		// load the element state
		void load(const std::string_view file_name);
	private:
		fgui::state m_opened;
		fgui::color m_color = { 0, 0, 0 };
	};
}