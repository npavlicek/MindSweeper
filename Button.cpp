#include "Button.h"

#include <iostream>

ms::Button::Button() {
	cFont.loadFromFile("./res/font/Prompt-Regular.ttf");
}

void ms::Button::init(std::string pText, sf::Vector2f pPos, sf::Vector2f pSize) {
	cButtonRect.setPosition(pPos);
	cButtonRect.setSize(pSize);
	cButtonRect.setOutlineThickness(1.f);
	cButtonRect.setOutlineColor(sf::Color(0x666666ff));

	cButtonText.setCharacterSize(30.f);
	cButtonText.setFont(cFont);
	cButtonText.setString(pText);
	cButtonText.setPosition(pPos.x + ((pSize.x / 2) - cButtonText.getGlobalBounds().width / 2), pPos.y + ((pSize.y / 2) - cButtonText.getGlobalBounds().height / 2) - 6.f);
	cButtonText.setFillColor(sf::Color::Black);
}

bool ms::Button::click(sf::Vector2i pPos) {
	return cButtonRect.getGlobalBounds().contains((sf::Vector2f)pPos);
}

void ms::Button::draw(sf::RenderWindow& pWindow) {
	if (cButtonRect.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(pWindow))) {
		cButtonRect.setFillColor(sf::Color(0xddddddff));
	}
	else {
		cButtonRect.setFillColor(sf::Color(0xffffffff));
	}
	pWindow.draw(cButtonRect);
	pWindow.draw(cButtonText);
}