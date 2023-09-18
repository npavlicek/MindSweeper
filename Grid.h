#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <exception>
#include <set>

#include "Cell.h"

namespace ms {
	class Grid {
	public:
		Grid();
		void create(int pRows, int pColumns, int pNumBombs, int pCellSize);
		void draw(sf::RenderWindow& pWindow);
		void click(sf::Vector2i pMousePos, int pMouseButton);
		bool detectVictory();
		bool detectFailure();
		/// <summary>
		/// Reveals all cells or just bombs.
		/// </summary>
		/// <param name="pRevBombs">Set to true if you would only like to reveal the bombs</param>
		void revealAll(bool pRevBombs);
		void regen();
	private:
		std::vector<std::vector<Cell>> cRowVector;
		std::map<std::string, sf::Texture> cTextures;

		int cRows = 0, cCols = 0, cBombs = 0;
		int cCellSize = 0;
		bool cFirstClick = true;

		bool cFailure = false;

		bool isValidCell(int pX, int pY);
		void gen(int pX, int pY);
		void genCellNumbers();
		void revealMFZ(int pX, int pY);
		sf::Texture& getTex(const std::string& name) { return cTextures.at(name); }
		sf::Vector2i getCellIndicesFromCoords(int pX, int pY) {
			int cellX = pX / cCellSize, cellY = pY / cCellSize;
			return sf::Vector2i{cellX, cellY};
		}
		Cell& getCell(int pX, int pY) { return cRowVector.at(pY).at(pX); }
		int detectBombs(int pX, int pY);
	};
}