#include "Menu.h"

ms::Menu::Menu() {
	cFont.loadFromFile("./res/font/Prompt-Regular.ttf");
}

void ms::Menu::init(sf::Vector2f pWindowSize) {
	sf::Vector2f inputSize;
	inputSize.x = pWindowSize.x * .2;
	inputSize.y = inputSize.x * .5;

	auto widthInputPos = sf::Vector2f((pWindowSize.x / 3) * 1 - (inputSize.x / 2), 100.f);
	cWidthInput.init(widthInputPos, inputSize, 2, "20");
	cWidthInputLabel.setString("Width");
	cWidthInputLabel.setFont(cFont);
	cWidthInputLabel.setFillColor(sf::Color::Black);
	cWidthInputLabel.setCharacterSize(20);
	cWidthInputLabel.setPosition(widthInputPos.x, widthInputPos.y - 25);

	auto heightInputPos = sf::Vector2f((pWindowSize.x / 3) * 2 - (inputSize.x / 2), 100.f);
	cHeightInput.init(heightInputPos, inputSize, 2, "15");
	cHeightInputLabel.setString("Height");
	cHeightInputLabel.setFont(cFont);
	cHeightInputLabel.setFillColor(sf::Color::Black);
	cHeightInputLabel.setCharacterSize(20);
	cHeightInputLabel.setPosition(heightInputPos.x, heightInputPos.y - 25);

	auto difficultyPercentInputPos = sf::Vector2f((pWindowSize.x / 2) * 1 - (inputSize.x / 2), 250.f);
	cDifficultyPercentInput.init(difficultyPercentInputPos, inputSize, 2, "15");
	cDifficultyPercentInputLabel.setString("Difficulty (0-88%)");
	cDifficultyPercentInputLabel.setFont(cFont);
	cDifficultyPercentInputLabel.setFillColor(sf::Color::Black);
	cDifficultyPercentInputLabel.setCharacterSize(20);
	cDifficultyPercentInputLabel.setPosition(difficultyPercentInputPos.x, difficultyPercentInputPos.y - 25);

	cButtonConfirm.init("Play", sf::Vector2f((pWindowSize.x / 2) * 1 - (inputSize.x / 2), 350.f), inputSize);
}

void ms::Menu::resize(sf::Vector2f pWindowSize) {
	sf::Vector2f inputSize;
	inputSize.x = pWindowSize.x * .2;
	inputSize.y = inputSize.x * .5;
	auto widthInputPos = sf::Vector2f((pWindowSize.x / 3) * 1 - (inputSize.x / 2), 100.f);
	auto heightInputPos = sf::Vector2f((pWindowSize.x / 3) * 2 - (inputSize.x / 2), 100.f);
	auto difficultyPercentInputPos = sf::Vector2f((pWindowSize.x / 2) * 1 - (inputSize.x / 2), 250.f);

}

void ms::Menu::click(sf::Vector2i pPos) {
	cWidthInput.click(pPos);
	cHeightInput.click(pPos);
	cDifficultyPercentInput.click(pPos);
	cStart = cButtonConfirm.click(pPos);
}

void ms::Menu::input(int pUnicode) {
	cWidthInput.input(pUnicode);
	cHeightInput.input(pUnicode);
	cDifficultyPercentInput.input(pUnicode);
}

void ms::Menu::draw(sf::RenderWindow& pWindow) {
	pWindow.draw(cWidthInputLabel);
	pWindow.draw(cHeightInputLabel);
	pWindow.draw(cDifficultyPercentInputLabel);

	cWidthInput.draw(pWindow);
	cHeightInput.draw(pWindow);
	cDifficultyPercentInput.draw(pWindow);
	cButtonConfirm.draw(pWindow);
}

bool ms::Menu::pollState(MenuState& pState) {
	if (cStart) {
		cStart = false;

		pState.vWidthSetting = std::stoi(cWidthInput.getText());
		pState.vHeightSetting = std::stoi(cHeightInput.getText());
		pState.vDifficultySetting = std::stoi(cDifficultyPercentInput.getText());

		return true;
	}
	return false;
}
