#include "Grid.h"

ms::Grid::Grid() {
	std::string textureList[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "marked", "covered", "bomb" };

	for (auto& elem : textureList) {
		cTextures.insert({ elem, sf::Texture() });
	}

	for (auto i = cTextures.begin(); i != cTextures.end(); i++) {
		if (!i->second.loadFromFile("./res/" + i->first + ".png")) {
			std::cout << "error loading from file: " << i->first << ".png" << std::endl;
		}
	}
}

void ms::Grid::create(int pRows, int pColumns, int pNumBombs, int pCellSize) {
	cCellSize = pCellSize;
	cFirstClick = true;
	cRowVector.clear();

	if (pRows < 0 || pColumns < 0 || pNumBombs < 0)
		throw std::invalid_argument("Arguments for creating a grid must be greater than 0!");

	if (pNumBombs >= pRows * pColumns)
		throw std::invalid_argument("Number of bombs must be less than the total number of cells!");

	cRows = pRows;
	cCols = pColumns;
	cBombs = pNumBombs;

	std::cout << cBombs << std::endl;

	// Grid generation
	// Initialize Cell vector
	for (auto y = 0; y < pRows; y++) {
		std::vector<ms::Cell> row;
		for (auto x = 0; x < pColumns; x++) {
			ms::Cell cell(sf::Vector2f(x * 32.f, y * 32.f));
			cell.setType("zero");
			cell.setTexture(getTex("covered"));
			row.push_back(cell);
		}
		cRowVector.push_back(row);
	}

	//gen();
}

void ms::Grid::regen() {
	cFirstClick = true;
	cFailure = false;
	for (auto& elem : cRowVector) {
		for (auto& subElem : elem) {
			subElem.setTexture(getTex("covered"));
			subElem.setType("zero");
			subElem.setCovered(true);
			subElem.setMarked(false);
		}
	}
	//gen();
}

void ms::Grid::revealAll(bool pRevBombs) {
	for (auto& x : cRowVector) {
		for (auto& y : x) {
			if (!pRevBombs) {
				y.setTexture(getTex(y.getType()));
			}
			else if (pRevBombs && y.isBomb()) {
				y.setTexture(getTex(y.getType()));
			}
		}
	}
}

void ms::Grid::gen(int pX, int pY) {
	// Decide bombs
	std::default_random_engine eng(time(NULL));
	std::uniform_int_distribution<int> dist_rows(0, cRows - 1);
	std::uniform_int_distribution<int> dist_columns(0, cCols - 1);

	std::normal_distribution<double> nRows(cRows / 2, 3);
	std::normal_distribution<double> nCols(cCols / 2, 3);

	int placedBombs = 0;

	while (placedBombs < cBombs) {
		auto row = dist_rows(eng);
		auto col = dist_columns(eng);
		//std::cout << "Row: " << row << " Col: " << col << std::endl;

		// Generate a blank 3x3 grid at the supplied coordinates (WHERE THE USER FIRST CLICKED) by utilizing the c++ `goto`	keyword to skip the `if` block below to negate cell generation
		for (int xOff = -1; xOff <= 1; xOff++) {
			for (int yOff = -1; yOff <= 1; yOff++) {
				int newX = pX + xOff;
				int newY = pY + yOff;
				if (col == newX && row == newY) {
					goto cnt;
				}
			}
		}

		// Generate the bomb cells
		if (isValidCell(col, row)) {
			auto& cell = getCell(col, row);
			if (!cell.isBomb()) {
				placedBombs++;
				cell.setType("bomb");
				//cell.setTexture(getTex(cell.getType()));
			}
		}

	cnt:;
	}
	genCellNumbers();
}

void ms::Grid::genCellNumbers() {
	//Generate Cell Numbers
	for (int row = 0; row < cRows; row++) {
		for (int col = 0; col < cCols; col++) {
			auto& cell = getCell(col, row);
			if (!cell.isBomb()) {
				switch (detectBombs(col, row)) {
				case 0:
					cell.setType("zero");
					break;
				case 1:
					cell.setType("one");
					break;
				case 2:
					cell.setType("two");
					break;
				case 3:
					cell.setType("three");
					break;
				case 4:
					cell.setType("four");
					break;
				case 5:
					cell.setType("five");
					break;
				case 6:
					cell.setType("six");
					break;
				case 7:
					cell.setType("seven");
					break;
				case 8:
					cell.setType("eight");
					break;
				default:
					cell.setType("zero");
				}
			}
			//cell.setTexture(getTex(cell.getType()));
		}
	}
}

// Detect bombs around the provided coordinates
// O: the provided coordinates
// X: The surrounding cells that are counted
// 
// X X X
// X O X
// X X X
int ms::Grid::detectBombs(int pX, int pY) {
	int detectedBombs = 0;
	for (int xOff = -1; xOff <= 1; xOff++) {
		for (int yOff = -1; yOff <= 1; yOff++) {
			int newX = pX + xOff;
			int newY = pY + yOff;
			// !(newY < 0 || newY > numRows - 1 || newX < 0 || newX > numCols - 1)
			if (isValidCell(newX, newY)) {
				if (getCell(newX, newY).isBomb()) {
					detectedBombs++;
				}
			}
		}
	}
	return detectedBombs;
}

bool ms::Grid::isValidCell(int pX, int pY) {
	if (pX < 0 || pY < 0 || pY > cRows - 1 || pX > cCols - 1) {
		return false;
	}
	else {
		return true;
	}
}

void ms::Grid::click(sf::Vector2i pMousePos, int pMouseButton) {
	sf::Vector2i cellLoc = getCellIndicesFromCoords(pMousePos.x, pMousePos.y);
	if (isValidCell(cellLoc.x, cellLoc.y)) {
		Cell& cell = getCell(cellLoc.x, cellLoc.y);
		if (!cell.isMarked() && cell.isCovered()) {
			// LEFT CLICK
			if (pMouseButton == sf::Mouse::Left) {
				// IF THIS IS THE FIRST CLICK GENERATE THE GRID, CELL NUMBERS, AND THEN REVEAL THE MINE FREE ZONE
				if (cFirstClick) {
					gen(cell.getSprite().getPosition().x / 32, cell.getSprite().getPosition().y / 32);
					genCellNumbers();
					revealMFZ(cell.getSprite().getPosition().x / 32, cell.getSprite().getPosition().y / 32);

					cFirstClick = false;
				}
				// reveal the mine free zone around the empty cell that has been clicked
				else if (cell.getType() == "zero") {
					revealMFZ(cell.getSprite().getPosition().x / 32, cell.getSprite().getPosition().y / 32);
				}
				// Regular non-zero cell reveal
				else {
					cell.setCovered(false);
					std::cout << cell.getType() << std::endl;
					cell.setTexture(getTex(cell.getType()));
					if (cell.getType() == "bomb") {
						cFailure = true;
					}
				}
			}
			// MARKING COVERED CELLS
			else if (pMouseButton == sf::Mouse::Right) {
				cell.setMarked(true);
				cell.setTexture(getTex("marked"));
			}
		}
		// RIGHT CLICK IF CELL IS MARKED AND COVERED
		else if (cell.isMarked() && cell.isCovered() && pMouseButton == sf::Mouse::Right) {
			cell.setMarked(false);
			cell.setTexture(getTex("covered"));
		}
	}
}

// MFZ: Mine Free Zone
// THIS FUNCTION NEEDS EXTREME REVISION, TEMPORARY CODE JUST TO MAKE IT WORK
void ms::Grid::revealMFZ(int pX, int pY) {
	if (isValidCell(pX, pY)) {
		if (getCell(pX, pY).isCovered() && getCell(pX, pY).getType() == "zero") {
			ms::Cell& cell = getCell(pX, pY);
			cell.setCovered(false);

			cell.setTexture(getTex(cell.getType()));

			revealMFZ(pX - 1, pY);
			if (isValidCell(pX - 1, pY)) {
				getCell(pX - 1, pY).setCovered(false);
				getCell(pX - 1, pY).setTexture(getTex(getCell(pX - 1, pY).getType()));
			}
			revealMFZ(pX, pY - 1);
			if (isValidCell(pX, pY - 1)) {
				getCell(pX, pY - 1).setCovered(false);
				getCell(pX, pY - 1).setTexture(getTex(getCell(pX, pY - 1).getType()));
			}
			revealMFZ(pX, pY + 1);
			if (isValidCell(pX, pY + 1)) {
				getCell(pX, pY + 1).setCovered(false);
				getCell(pX, pY + 1).setTexture(getTex(getCell(pX, pY + 1).getType()));
			}
			revealMFZ(pX + 1, pY);
			if (isValidCell(pX + 1, pY)) {
				getCell(pX + 1, pY).setCovered(false);
				getCell(pX + 1, pY).setTexture(getTex(getCell(pX + 1, pY).getType()));
			}
			// Top left
			revealMFZ(pX - 1, pY - 1);
			if (isValidCell(pX - 1, pY - 1)) {
				getCell(pX - 1, pY - 1).setCovered(false);
				getCell(pX - 1, pY - 1).setTexture(getTex(getCell(pX - 1, pY - 1).getType()));
			}
			// Top right
			revealMFZ(pX + 1, pY - 1);
			if (isValidCell(pX + 1, pY - 1)) {
				getCell(pX + 1, pY - 1).setCovered(false);
				getCell(pX + 1, pY - 1).setTexture(getTex(getCell(pX + 1, pY - 1).getType()));
			}
			// Bottom left
			revealMFZ(pX - 1, pY + 1);
			if (isValidCell(pX - 1, pY + 1)) {
				getCell(pX - 1, pY + 1).setCovered(false);
				getCell(pX - 1, pY + 1).setTexture(getTex(getCell(pX - 1, pY + 1).getType()));
			}
			// Bottom right
			revealMFZ(pX + 1, pY + 1);
			if (isValidCell(pX + 1, pY + 1)) {
				getCell(pX + 1, pY + 1).setCovered(false);
				getCell(pX + 1, pY + 1).setTexture(getTex(getCell(pX + 1, pY + 1).getType()));
			}
		}
	}
	//std::cout << "X: " << pX << " Y: " << pY << std::endl;
}

bool ms::Grid::detectVictory() {
	int markedBombs = 0;
	for (auto& elem : cRowVector) {
		for (auto& sub_elem : elem) {
			if (sub_elem.getType() == "bomb" && sub_elem.isMarked())
				markedBombs++;
		}
	}
	if (markedBombs == cBombs)
		return true;
	return false;
}

bool ms::Grid::detectFailure() {
	return cFailure;
}

void ms::Grid::draw(sf::RenderWindow& pWindow) {
	for (auto& elem : cRowVector) {
		for (auto& sub_elem : elem) {
			sub_elem.draw(pWindow);
		}
	}
}