#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>

namespace ms {
	class TextInput {
	public:
		TextInput();
		void init(sf::Vector2f pPos, sf::Vector2f pSize, int pMaxLength, std::string pPlaceholderText = "");
		void click(sf::Vector2i pPos);
		void input(int pUnicode);
		std::string getText() { return cCurrentText; }
		void draw(sf::RenderWindow& pWindow);
	private:
		bool cSelected = false;
		bool cDrawCarrot = true;
		int cMaxLength = 0;

		std::string cCurrentText;

		std::chrono::time_point<std::chrono::steady_clock> cStart;
		std::chrono::time_point<std::chrono::steady_clock> cEnd;

		sf::RectangleShape cTextBox;
		sf::RectangleShape cCarrot;
		sf::Text cText;
		sf::Font cFont;
	};
}