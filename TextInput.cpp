#include "TextInput.h"

#include <iostream>

ms::TextInput::TextInput() {
	cFont.loadFromFile("./res/font/Prompt-Regular.ttf");
}

void ms::TextInput::init(sf::Vector2f pPos, sf::Vector2f pSize, int pMaxLength, std::string pPlaceholderText) {
	cMaxLength = pMaxLength;

	cCurrentText = pPlaceholderText;
	cText.setString(cCurrentText);

	cTextBox.setPosition(pPos);
	cTextBox.setSize(pSize);
	cTextBox.setFillColor(sf::Color(0xffffffff));
	cTextBox.setOutlineThickness(1.f);
	cTextBox.setOutlineColor(sf::Color(0x666666ff));

	cText.setPosition(sf::Vector2f(pPos.x + 3.f, pPos.y - (pSize.y * 0.15f)));
	cText.setFont(cFont);
	cText.setFillColor(sf::Color::Black);
	cText.setCharacterSize((unsigned int)pSize.y);

	cCarrot.setSize(sf::Vector2f(1.f, pSize.y - (pSize.y * 0.15f)));
	cCarrot.setOrigin(sf::Vector2f(0.f, cCarrot.getSize().y / 2));
	cCarrot.setPosition(sf::Vector2f(pPos.x + 5.f, pPos.y + (pSize.y / 2)));
	cCarrot.setFillColor(sf::Color::Black);

	cStart = std::chrono::steady_clock::now();
}

void ms::TextInput::click(sf::Vector2i pPos) {
	if (cTextBox.getGlobalBounds().contains((float)pPos.x, (float)pPos.y)) {
		cStart = std::chrono::steady_clock::now();
		cSelected = true;
		cTextBox.setFillColor(sf::Color(0xddddddff));
	}
	else {
		cSelected = false;
		cDrawCarrot = true;
		cTextBox.setFillColor(sf::Color(0xffffffff));
	}
}

void ms::TextInput::input(int pUnicode) {
	if (cSelected) {
		if (pUnicode >= 0x30 && pUnicode <= 0x39 && (cCurrentText.length() <= cMaxLength - 1)) {
			cCurrentText.push_back(static_cast<char>(pUnicode));
			cText.setString(cCurrentText);
			if (cText.getGlobalBounds().width + 15.f > cTextBox.getSize().x) {
				cCurrentText.pop_back();
				cText.setString(cCurrentText);
			}
		}
		else if (pUnicode == 0x8 && cCurrentText.length() > 0) {
			cCurrentText.pop_back();
			cText.setString(cCurrentText);
		}
	}
}

void ms::TextInput::draw(sf::RenderWindow& pWindow) {
	pWindow.draw(cTextBox);
	pWindow.draw(cText);
	if (cSelected) {
		float carrotOffset = 0;
		if (cCurrentText.length() == 0) {
			carrotOffset = 5.f;
		}
		else {
			carrotOffset = 10.f;
		}
		cCarrot.setPosition(sf::Vector2f(cText.getGlobalBounds().width + cTextBox.getPosition().x + carrotOffset, cTextBox.getPosition().y + (cTextBox.getSize().y / 2)));

		cEnd = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(cEnd - cStart).count();
		if (elapsed >= 320) {
			cStart = std::chrono::steady_clock::now();
			cDrawCarrot = !cDrawCarrot;
		}
		if (cDrawCarrot) {
			pWindow.draw(cCarrot);
		}
	}
}