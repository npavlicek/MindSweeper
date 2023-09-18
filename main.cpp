#include <iostream>
#include <math.h>
#include <exception>

#include <SFML/Graphics.hpp>

#include "Grid.h"
#include "Button.h"
#include "Menu.h"

/*
STYLE GUIDE:
Classes: Capitalize ex: OneClass
Namespaces: lowercase acronym ex: ms (mindsweeper)

Functions: camel case ex: thisFunction
Parameters: p + capitalize ex: pParam
Enums: Same as const vars below

Const Vars: upper case with underscores  ex: NUM_BOMBS
Public Variables: v + capitalize ex: vVar
Local Variables: camel case ex: thisVar
Private Variables: c + capitalize ex: cVar
*/

constexpr int CELL_SIZE = 32;

enum class GAME_STATES {
	PLAY,
	MENU,
	VICTORY,
	FAILURE
};

int main() {
	std::cout << "-- WELCOME TO MINDSWEEPER --" << std::endl;
	std::cout << "- Controls:" << std::endl;
	std::cout << "- ";

	int menuWidth = 640;
	int menuHeight = 480;

	int windowWidth = menuWidth;
	int windowHeight = menuHeight;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "VungSweeper", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(20.f);

	GAME_STATES currentState = GAME_STATES::MENU;

	ms::Grid grid;
	ms::Menu menu;
	menu.init((sf::Vector2f)window.getSize());

	sf::Texture victoryTex;
	victoryTex.loadFromFile("./res/victory.png");
	sf::Sprite victorySprite;
	victorySprite.setTexture(victoryTex);

	sf::Texture failureTex;
	failureTex.loadFromFile("./res/failure.png");
	sf::Sprite failureSprite;
	failureSprite.setTexture(failureTex);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased) {
				switch (currentState) {
				case GAME_STATES::MENU:
					menu.click(sf::Mouse::getPosition(window));
					break;
				case GAME_STATES::PLAY:
					grid.click(sf::Mouse::getPosition(window), event.mouseButton.button);
					break;
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::R && (currentState == GAME_STATES::PLAY || currentState == GAME_STATES::VICTORY || currentState == GAME_STATES::FAILURE)) {
					currentState = GAME_STATES::PLAY;
					grid.regen();
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					if (currentState == GAME_STATES::MENU) {
						currentState = GAME_STATES::PLAY;
						window.setView(sf::View(sf::Vector2f(windowWidth / 2, windowHeight / 2), sf::Vector2f(windowWidth, windowHeight)));
						window.setSize(sf::Vector2u(windowWidth, windowHeight));
					}
					else if (currentState == GAME_STATES::PLAY || currentState == GAME_STATES::VICTORY || currentState == GAME_STATES::FAILURE) {
						currentState = GAME_STATES::MENU;
						window.setView(sf::View(sf::Vector2f(menuWidth / 2, menuHeight / 2), sf::Vector2f(menuWidth, menuHeight)));
						window.setSize(sf::Vector2u(menuWidth, menuHeight));
					}
				}
				else if (event.key.code == sf::Keyboard::C) {
					grid.revealAll(false);
				}
				else if (event.key.code == sf::Keyboard::V) {
					grid.revealAll(true);
				}
			}
			else if (event.type == sf::Event::TextEntered && currentState == GAME_STATES::MENU) {
				menu.input(event.text.unicode);
			}
		}

		// Detect if play button has been pressed
		ms::MenuState state;
		if (menu.pollState(state)) {
			if (state.vDifficultySetting > 88) {
				std::cerr << "difficulty setting too high!" << std::endl;
				exit(EXIT_FAILURE);
			}
			
			currentState = GAME_STATES::PLAY;
			
			windowWidth = state.vWidthSetting * CELL_SIZE;
			windowHeight = state.vHeightSetting * CELL_SIZE;

			window.setView(sf::View(sf::Vector2f(windowWidth / 2, windowHeight / 2), sf::Vector2f(windowWidth, windowHeight)));
			window.setSize(sf::Vector2u(windowWidth, windowHeight));
			grid.create(state.vHeightSetting, state.vWidthSetting, floor((state.vHeightSetting * state.vWidthSetting) * (state.vDifficultySetting / 100.f)), CELL_SIZE);
			std::cout << "created grid" << std::endl;
		}

		// Detect victory state
		if (currentState == GAME_STATES::PLAY && grid.detectVictory()) {
			currentState = GAME_STATES::VICTORY;
			float scale = (float) windowHeight / menuHeight;
			scale *= 0.7;
			victorySprite.setScale(sf::Vector2f(scale, scale));
			victorySprite.setOrigin(sf::Vector2f(victorySprite.getLocalBounds().width / 2, victorySprite.getLocalBounds().height / 2));
			victorySprite.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 2));
		}

		// Detect failure state
		if (currentState == GAME_STATES::PLAY && grid.detectFailure()) {
			currentState = GAME_STATES::FAILURE;
			float scale = (float)windowHeight / menuHeight;
			scale *= 0.7;
			failureSprite.setScale(sf::Vector2f(scale, scale));
			failureSprite.setOrigin(sf::Vector2f(failureSprite.getLocalBounds().width / 2, failureSprite.getLocalBounds().height / 2));
			failureSprite.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 2));
			grid.revealAll(true);
		}

		window.clear(sf::Color(0x808080ff));

		switch (currentState) {
		case GAME_STATES::MENU:
			menu.draw(window);
			break;
		case GAME_STATES::PLAY:
			grid.draw(window);
			break;
		case GAME_STATES::VICTORY:
			grid.draw(window);
			window.draw(victorySprite);
			break;
		case GAME_STATES::FAILURE:
			grid.draw(window);
			window.draw(failureSprite);
			break;
		default:
			break;
		}

		window.display();
	}

	return 0;
}