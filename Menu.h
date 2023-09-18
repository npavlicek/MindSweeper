#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "TextInput.h"
#include "Button.h"

namespace ms {
	struct MenuState {
		int vWidthSetting;
		int vHeightSetting;
		int vDifficultySetting;
	};

	class Menu {
	public:
		Menu();
		void init(sf::Vector2f pWindowSize);
		void resize(sf::Vector2f pWindowSize);
		void draw(sf::RenderWindow& pWindow);
		void click(sf::Vector2i pPos);
		void input(int pUnicode);
		/// <summary>
		/// Returns true if game should start, determined by if the play button has been pressed.
		/// </summary>
		/// <param name="pState">Menu state struct</param>
		/// <returns>True, if game should start</returns>
		bool pollState(MenuState &pState);
	private:
		TextInput cWidthInput;
		TextInput cHeightInput;
		TextInput cDifficultyPercentInput;

		sf::Text cWidthInputLabel;
		sf::Text cHeightInputLabel;
		sf::Text cDifficultyPercentInputLabel;

		Button cButtonConfirm;

		sf::Font cFont;

		bool cStart = false;
	};
}