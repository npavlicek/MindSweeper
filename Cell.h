#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace ms {
	class Cell {
	public:
		Cell(sf::Vector2f pPos) { cCell.setPosition(pPos); }
		void draw(sf::RenderWindow& pWindow) { pWindow.draw(cCell); }

		sf::Sprite& getSprite() { return cCell; }
		std::string getType() { return cType; }
		
		bool isMarked() { return cMarked; }
		bool isBomb() { if (cType == "bomb") { return true; } }
		bool isCovered() { return cCovered; }

		void setMarked(bool pMarked) { cMarked = pMarked; }
		void setCovered(bool pCovered) { cCovered = pCovered; }
		void setType(std::string pType) { cType = pType; }
		void setTexture(sf::Texture& pTexture) { cCell.setTexture(pTexture); }
	private:
		sf::Sprite cCell;

		std::string cType;
		bool cBomb = false;
		bool cMarked = false;
		bool cCovered = true;
	};
}
