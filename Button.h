#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace ms {
	class Button {
	public:
		Button();
		void init(std::string pText, sf::Vector2f pPos, sf::Vector2f pSize);
		bool click(sf::Vector2i pPos);
		void draw(sf::RenderWindow& pWindow);
	private:
		sf::RectangleShape cButtonRect;
		sf::Text cButtonText;
		sf::Font cFont;
	};
}