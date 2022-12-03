#include "Game.h"
thread scene;
bool OnMusic = true, playing = true;
Game::Game()
{
	bgColor = COLOUR::BG_WHITE;
	BuildConsole(210, 49, 16, COLOUR::BG_WHITE);
	FillArea(Position(0, 0), L' ', getWidth(), getHeight(), bgColor);
	readSaveFile();
	state = mainMenu;
	scene = thread(&Game::drawMenu, this);
	playTheme();
}
Game::Game(int width_, int height_, int fontSize, short color)
{
	BuildConsole(width_, height_, fontSize, color);
	bgColor = color;
	FillArea(Position(0, 0), L' ', getWidth(), getHeight(), bgColor);
	readSaveFile();
	state = mainMenu;
	scene = thread(&Game::drawMenu, this);
	playTheme();
}
Game::~Game()
{
	fstream file("Saves/savegame.txt", ios::out);
	for (auto i : saveFile)
	{
		// check if the player did save this file, else delete it from the RAM
		fstream file_i("Saves/" + i + ".txt", ios::in);
		if (!file_i.fail())
			file << i << endl;
		file_i.close();
	}
	file.close();

	for (auto i : animalList)
		delete i;
	animalList.clear();
	for (auto i : vehicleList)
		delete i;
	vehicleList.clear();
}
void Game::drawBorder()
{
	// drawing border, first frame to create border
	short borderColor = bgColor | FG_DARK_GREEN;
	for (int y = 0; y < getHeight(); y++)
	{
		for (int x = 0; x < getWidth(); x++)
		{
			if (x == 0 && y == 0) // left top corner
				addChar(Position(x, y), L'╔', borderColor);
			else if (x == getWidth() - 1 && y == 0) // right top corner
				addChar(Position(x, y), L'╗', borderColor);
			else if (x == 0 && y == getHeight() - 1) // left low corner
				addChar(Position(x, y), L'╚', borderColor);
			else if (x == getWidth() - 1 && y == getHeight() - 1) // right low corner
				addChar(Position(x, y), L'╝', borderColor);
			else if (x == 0 || x == getWidth() - 1)
				addChar(Position(x, y), L'║', borderColor);
			else if (y == 0 || y == getHeight() - 1)
				addChar(Position(x, y), L'═', borderColor);
		}
	}
}
void Game::readSaveFile()
{
	fstream file("Saves/savegame.txt", ios::in);
	string temp;
	while (!file.eof())
	{
		getline(file, temp, '\n');
		if (temp != "")
		{
			fstream file_i("Saves/" + temp + ".txt", ios::in);
			if (!file_i.fail())
				saveFile.push_back(temp);
			file_i.close();
		}
	}
	file.close();
}
string name = "", curFileName; // for new file
char MOVING;
int counter = 2, index = counter;
int offset_box = 30; // for box covering
void Game::Start()
{
	while (playing)
	{
		char key = getch();
		if (state == mainMenu)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < 4)
				counter++;

			if (key == '\r')
			{
				if (counter == 1)
				{
					state = playMenu;
					scene.join();                                 // shutdown mainMenu thread
					scene = thread(&Game::drawMenuPlay, this); // start the playMenu thread
				}
				if (counter == 2)
				{
					counter = 1;
					state = optionMenu;
					scene.join();                                   // shutdown mainMenu thread
					scene = thread(&Game::drawMenuOption, this); // start the optionMenu thread
				}
				if (counter == 3)
				{
					state = credits;
					scene.join();
					scene = thread(&Game::drawMenuCredit, this);
				}
				if (counter == 4) // exit the game
				{
					state = exitGame;
					scene.join();      // shutdown mainMenu thread
					break;
				}
			}
		}
		else if (state == playMenu)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < 3)
				counter++;

			if (key == '\r')
			{
				if (counter == 1)
				{
					// shutdown menu thread start game thread
					state = newGame;
					scene.join();
					scene = thread(&Game::drawNewGame, this);
				}
				if (counter == 2)
				{
					state = loadGame;
					scene.join();
					scene = thread(&Game::drawLoadMenu, this);
					counter = 1;
				}
				if (counter == 3)
				{
					state = mainMenu; // back to the main menu thread
					scene.join();
					scene = thread(&Game::drawMenu, this);
				}
			}
		}
		else if (state == optionMenu)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < 2)
				counter++;
			if (key == '\r')
			{
				if (counter == 1)
				{
					if (OnMusic)
					{
						close();
						OnMusic = false;
					}
					else
					{
						newPlay();
						OnMusic = true;
					}
				}
				else
				{
					state = mainMenu;
					scene.join();
					scene = thread(&Game::drawMenu, this);
				}
			}
		}
		else if (state == loadGame)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < saveFile.size() + 1)
				counter++;
			if (key == '\r')
			{
				if (counter == 1)
				{
					state = playMenu;
					counter = 2;
					scene.join();
					scene = thread(&Game::drawMenuPlay, this);
				}
				else
				{
					// load data to the player and start the game thread
					// or u can make another Menu list like remove save files, edit save file name
					state = editSave;
					counter = 1;
					scene.join();
					scene = thread(&Game::drawEditSave, this);
				}
			}
		}
		else if (state == editSave)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < 3)
				counter++;
			if (key == '\r')
			{
				if (counter == 1)
				{
					state = loadGame;
					counter = index + 2;
					scene.join();
					scene = thread(&Game::drawLoadMenu, this);
				}
				else if (counter == 2)
				{
					// delete the save file first
					string _name = "Saves/" + saveFile[index] + ".txt";
					int k = _name.length();
					char* Fname = new char[k + 1];
					strcpy(Fname, _name.c_str());
					remove(Fname);
					delete[] Fname;
					// pop it from the RAM
					saveFile.erase(saveFile.begin() + index);
					state = loadGame;
					counter = index + 1;
					scene.join();
					scene = thread(&Game::drawLoadMenu, this);
				}
				else if (counter == 3)
				{
					// load data, should have placed inside a function or something
					curFileName = saveFile[index];
					string _name = curFileName + ".txt";
					fstream file("Saves/" + _name, ios::in);
					int _x = 0, _y = 0, health, level;
					file >> _x >> _y >> health >> level;
					LoadPlayer(Position(_x,_y), health, level); // temporary test
					// load game data
					string _type = " ";
					int _velocity = 0, _dir = 0;
					for (auto i : vehicleList)
						delete i;
					vehicleList.clear();
					for (auto j : animalList)
						delete j;
					animalList.clear();
					for (auto k : lights)
						delete k;
					lights.clear();
					while (!file.eof())
					{
						//file >> _type >> _x >> _y >> _dir >> _velocity;
						file >> _type;
						if (_type == "Car" || _type == "Truck" || _type == "Deer") {
							file>> _x >> _y >> _dir >> _velocity;
							if (_type == "Car")
							{
								if (_dir == 1)
									vehicleList.push_back(new Car(_x, _y, right_, _velocity));
								else
									vehicleList.push_back(new Car(_x, _y, left_, _velocity));
							}
							else if (_type == "Truck")
							{
								if (_dir == 1)
									vehicleList.push_back(new Truck(_x, _y, right_, _velocity));
								else
									vehicleList.push_back(new Truck(_x, _y, left_, _velocity));
							}
							else if (_type == "Deer")
							{
								if (_dir == 1)
									animalList.push_back(new Deer(_x, _y, right_, _velocity));
								else
									animalList.push_back(new Deer(_x, _y, left_, _velocity));
							}
						}
						else if (_type == "lights")
						{
							int tR = 0, tG = 0, _stop = 0, cT = 0;
							file >> _x >> _y >> tR >> tG >> _stop >> cT;
							if (tR != 0)
								lights.push_back(new TrafficLight(_x, _y, tR, tG, _stop, cT));
						}
					}
					file.close();

					// start the game
					state = playGame;
					scene.join();
					scene = thread(&Game::drawGame, this);
				}
			}
		}
		else if (state == credits)
		{
			if (key == '\r')
			{
				state = mainMenu;
				scene.join();
				scene = thread(&Game::drawMenu, this);
			}
		}
		else if (state == newGame)
		{
			if (key == '\r' && !name.empty())
			{
				// start as default of level 1
				for (auto i : vehicleList)
					delete i;
				vehicleList.clear();
				for (auto j : animalList)
					delete j;
				animalList.clear();
				for (auto k : lights)
					delete k;
				lights.clear();
				
				Level1();

				state = playGame;
				scene.join();
				scene = thread(&Game::drawGame, this);
				saveFile.push_back(name);
				curFileName = name;
				index = saveFile.size() - 1;
				CreatePlayer();
				name = "";
			}
			else if (key != '\b' && name.length() < 20)
				name += key;
			else if (!name.empty())
				name.pop_back();
		}
		else if (state == pauseMenu)
		{
			if (key == 'w' && counter > 1)
				counter--;
			if (key == 's' && counter < 3)
				counter++;

			if (key == '\r')
			{
				if (counter == 1)
				{
					// shutdown menu thread start game thread
					state = playGame;
					scene.join();
					scene = thread(&Game::drawGame, this);
				}
				if (counter == 2)
				{
					// write out the save file
					string _name = curFileName + ".txt";
					fstream file("Saves/" + _name, ios::out);
					/*file << player->x << " " << player->y << " " << player->getHealth() << " " << player->getLevel();
					for (auto i : animalList)
						file << " "<< i->getName() << " " << i->x << " " << i->y << " " << i->getDir() << " " << i->velocity();
					for (auto i  : vehicleList)
						file << " "<< i->getName() << " " << i->x << " " << i->y << " " << i->getDir() << " " << i->velocity();
					for (auto i : lights)
						file << " " << i->getName() << " " << i->x << " " << i->y << " " << i->getRedTime() << " " << i->getGreenTime() << " " << int(i->isStop()) << " " << i->curTime();
					file.close();*/
					file << player->x << " " << player->y << " " << player->getHealth() << " " << player->getLevel() << endl;
					for (auto i : animalList)
						file << i->getName() << " " << i->x << " " << i->y << " " << i->getDir() << " " << i->velocity() << endl;
					for (auto i : vehicleList)
						file << i->getName() << " " << i->x << " " << i->y << " " << i->getDir() << " " << i->velocity() << endl;
					for (auto i : lights)
						file << i->getName() << " " << i->x << " " << i->y << " " << i->getRedTime() << " " << i->getGreenTime() << " " << int(i->isStop()) << " " << i->curTime() << endl;
					file.close();

					// announcement
					drawButton(getWidth() / 2, title.getHeight() + 7, L"ĐÃ LƯU FILE GAME", bgColor | FG_RED);
					drawConsole();
					playSaveSound();
					Sleep(1000);
					FillArea(Position(getWidth() / 2 - 1, title.getHeight() + 7), L' ', 21, 3, bgColor);
					// saveFile.push_back(name);
				}
				if (counter == 3)
				{
					state = mainMenu; // back to the main menu thread
					scene.join();
					scene = thread(&Game::drawMenu, this);
					// pop the unsaved file out
					fstream file_i("Saves/" + curFileName + ".txt", ios::in);
					if (file_i.fail())
						saveFile.erase(saveFile.begin() + index);
					file_i.close();

					// clear the dumb shit
					for (auto i : vehicleList)
						delete i;
					vehicleList.clear();
					for (auto j : animalList)
						delete j;
					animalList.clear();
					for (auto k : lights)
						delete k;
					lights.clear();
				}
			}
		}
		else if (state == playGame) // the main game back end system
		{
			// load default level if it be empty
			if (vehicleList.empty() && animalList.empty())
			{
				if (player->getLevel() == 1) Level1();
				else if (player->getLevel() == 2) Level2();
				else if (player->getLevel() == 3) Level3();
				else Level4();

			}

			MOVING = toupper(key);
			if (key == 27)
			{
				// do the pause menu
				state = pauseMenu;
				scene.join();
				scene = thread(&Game::drawPauseMenu, this);
				counter = 1;
			}
			// start the new Game, thread, input new nameFile save for the game
			// if (key == 'w') // get the input file here
			if (player->isDead())
			{
				state = mainMenu;
				scene.join();
				scene = thread(&Game::drawMenu, this);
				counter = 1;
			}

		}
	}
}
void Game::drawMenu()
{
	drawBorder();
	//for (int i = 40; i > 0; i--) // hehe for fun
	//	drawBox(i, i, getWidth() - 1 - i, getHeight() - 1 - i, bgColor | FG_BLUE);
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, 17, 14, bgColor | FG_BLUE);

	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;
	while (state == mainMenu)
	{
		if (counter == 1)
			drawButton(getWidth() / 2 - 25, title.getHeight() + 4, L"PLAY GAME", hoverCol);
		else
			drawButton(getWidth() / 2 - 25, title.getHeight() + 4, L"PLAY GAME", defaultCol);

		if (counter == 2)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"OPTION", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"OPTION", defaultCol);

		if (counter == 3)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 10, L"CREDITS", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 10, L"CREDITS", defaultCol);

		if (counter == 4)
			drawButton(getWidth() / 2 - 25, title.getHeight() + 13, L"QUIT GAME", hoverCol);
		else
			drawButton(getWidth() / 2 - 25, title.getHeight() + 13, L"QUIT GAME", defaultCol);

		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawMenuOption()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, 20, 8, bgColor | FG_BLUE);

	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;
	while (state == optionMenu)
	{
		wstring text = (OnMusic) ? L"MUSIC: ON " : L"MUSIC: OFF";
		if (counter == 1)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, text, hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, text, defaultCol);

		if (counter == 2)
			drawButton(getWidth() / 2 - 25, title.getHeight() + 7, L"BACK TO MENU", hoverCol);
		else
			drawButton(getWidth() / 2 - 25, title.getHeight() + 7, L"BACK TO MENU", defaultCol);
		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawMenuCredit()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_DARK_YELLOW;

	const picture temp(vector<wstring>{
		L"21127668 - ĐINH QUANG PHONG",
		L"2112xxxx - VÕ CAO TRÍ",
		L"2112xxxx - HOÀNG LÊ CÁT THANH",
		L"2112xxxx - HUỲNH MINH QUANG"
	});
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, temp.getWidth() + 4, temp.getHeight() + 6, bgColor | FG_BLUE);
	while (state == credits)
	{
		drawButton(getWidth() / 2 - 25, title.getHeight() + 4, L"BACK TO MENU", hoverCol);
		drawPicture(getWidth() / 2 - 25, title.getHeight() + 8, temp, defaultCol);
		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawMenuPlay()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, 20, 11, bgColor | FG_BLUE);
	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;
	while (state == playMenu)
	{
		if (counter == 1)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, L"NEW GAME", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, L"NEW GAME", defaultCol);

		if (counter == 2)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"LOAD GAME", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"LOAD GAME", defaultCol);

		if (counter == 3)
			drawButton(getWidth() / 2 - 25, title.getHeight() + 10, L"BACK TO MENU", hoverCol);
		else
			drawButton(getWidth() / 2 - 25, title.getHeight() + 10, L"BACK TO MENU", defaultCol);

		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawLoadMenu()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	int max = 0;
	for (auto i : saveFile)
		if (i.length() > max)
			max = i.length();
	if (max < 12) // the back to menu size
		max = 13;
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, max * 2 - max / 3, saveFile.size() * 3 + 5, bgColor | FG_BLUE);
	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;

	drawButton(getWidth() / 2 - 52, title.getHeight() + 4, L"CHOOSE A SAVE GAME >", bgColor | FG_DARK_CYAN);
	while (state == loadGame)
	{
		if (counter == 1)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, L"BACK TO MENU", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 4, L"BACK TO MENU", defaultCol);

		for (int i = 0; i < saveFile.size(); i++)
		{
			wstring temp(saveFile[i].length(), L' ');
			copy(saveFile[i].begin(), saveFile[i].end(), temp.begin());
			if (counter == i + 2)
			{
				drawButton(getWidth() / 2 - 24, title.getHeight() + 7 + i * 3, temp, hoverCol);
				index = i;
			}
			else
				drawButton(getWidth() / 2 - 24, title.getHeight() + 7 + i * 3, temp, defaultCol);
		}

		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawEditSave()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;

	int max = 0;
	for (auto i : saveFile)
		if (i.length() > max)
			max = i.length();
	if (max < 12)
		max = 13;
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, max * 2 - max / 3, saveFile.size() * 3 + 5, bgColor | FG_BLUE);
	drawButton(getWidth() / 2 - 52, title.getHeight() + 4, L"CHOOSE A SAVE GAME >", bgColor | FG_DARK_CYAN);
	while (state == editSave)
	{
		drawButton(getWidth() / 2 - 24, title.getHeight() + 4, L"BACK TO MENU", defaultCol);
		for (int i = 0; i < saveFile.size(); i++)
		{
			wstring temp(saveFile[i].length(), L' ');
			copy(saveFile[i].begin(), saveFile[i].end(), temp.begin());
			if (i == index)
				drawButton(getWidth() / 2 - 24, title.getHeight() + 7 + i * 3, temp, hoverCol);
			else
				drawButton(getWidth() / 2 - 24, title.getHeight() + 7 + i * 3, temp, defaultCol);
		}
		if (counter == 1)
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 7 + index * 3, L"BACK", hoverCol);
		else
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 7 + index * 3, L"BACK", defaultCol);
		if (counter == 2)
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 10 + index * 3, L"DELETE", hoverCol);
		else
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 10 + index * 3, L"DELETE", defaultCol);
		if (counter == 3)
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 13 + index * 3, L"PLAY", hoverCol);
		else
			drawButton(getWidth() / 2 + max - 10, title.getHeight() + 13 + index * 3, L"PLAY", defaultCol);
		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawNewGame()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	drawPicture(50, 20, car1, bgColor | FG_RED); // bruh

	short defaultCol = bgColor | FG_DARK_BLUE;
	while (state == newGame)
	{
		drawString(getWidth() / 2 - 23, title.getHeight() + 4, L"ENTER NEW NAME ", defaultCol);
		drawBox(getWidth() / 2 - 25, title.getHeight() + 5, 24, 3, defaultCol);
		FillArea(Position(getWidth() / 2 - 24, title.getHeight() + 6), L' ', 21, 1, defaultCol);
		drawStringC(getWidth() / 2 - 23, title.getHeight() + 6, name, defaultCol);
		drawConsole(); // remember this
	}
	clearConsole();
}
void Game::drawPauseMenu()
{
	drawBorder();
	drawPicture(getWidth() / 2 - title.getWidth() / 1.5, 1, title, bgColor | FG_DARK_MAGENTA);
	// drawPicture(50, 20, car1, bgColor | FG_RED); // bruh
	drawBox(getWidth() / 2 - 27, title.getHeight() + 3, 20, 11, bgColor | FG_BLUE);
	short hoverCol = bgColor | FG_RED, defaultCol = bgColor | FG_CYAN;
	while (state == pauseMenu)
	{
		if (counter == 1)
			drawButton(getWidth() / 2 - 23, title.getHeight() + 4, L"RESUME", hoverCol);
		else
			drawButton(getWidth() / 2 - 23, title.getHeight() + 4, L"RESUME", defaultCol);

		if (counter == 2)
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"SAVE GAME", hoverCol);
		else
			drawButton(getWidth() / 2 - 24, title.getHeight() + 7, L"SAVE GAME", defaultCol);

		if (counter == 3)
			drawButton(getWidth() / 2 - 25, title.getHeight() + 10, L"BACK TO MENU", hoverCol);
		else
			drawButton(getWidth() / 2 - 25, title.getHeight() + 10, L"BACK TO MENU", defaultCol);

		drawConsole();
	}
	clearConsole();
}

bool Game::validMove(picture img, Position pos, int vel, MovingDir dir)
{
	if (pos.x - vel < 0 && dir == left_ || pos.x + vel + img.getWidth() >= getWidth() - offset_box && dir == right_)
		return false;
	if (pos.y - vel < 0 && dir == up_ || pos.y + img.getHeight() + vel >= getHeight() && dir == down_)
		return false;
	return true;
}

//int press_count = 1, press_timer = 1;

void Game::updatePosPlayer(char key)
{
	/*if (press_count != 0)
	{
		press_count--;
		return;
	}
	press_count = press_timer;*/

	if (key == 'D' || key == 'A' || key == 'S' || key == 'W')
	{
		if (OnMusic)
			playKeyBoardSound();
	}
	if (key == 'D' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), right_))
	{
		player->move(right_);
		FillArea(Position(player->x - player->velocity(), player->y), L' ', player->preImg().getWidth(), player->preImg().getHeight(), bgColor);
	}
	if (key == 'A' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), left_))
	{
		player->move(left_);
		FillArea(Position(player->x + player->velocity(), player->y), L' ', player->preImg().getWidth(), player->preImg().getHeight(), bgColor);
	}
	if (key == 'W' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), up_))
	{
		player->move(up_);
		FillArea(Position(player->x, player->y + player->velocity()), L' ', player->preImg().getWidth(), player->preImg().getHeight(), bgColor);
	}
	if (key == 'S' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), down_))
	{
		player->move(down_);
		FillArea(Position(player->x, player->y - player->velocity()), L' ', player->preImg().getWidth(), player->preImg().getHeight(), bgColor);
	}
	/*if (key == ' ')
		player->move(idle_);*/
}
//void OnEdge(int& x, int& y)
//{
//	
//}
void Game::updatePosObject()
{
	for (auto i : animalList)
	{
		MovingDir direction = i->getDir();
		i->move(direction);
		if (direction == right_)
			FillArea(Position(i->x - i->velocity(), i->y), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == left_)
			FillArea(Position(i->x + i->velocity(), i->y), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == up_)
			FillArea(Position(i->x, i->y + i->velocity()), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == down_)
			FillArea(Position(i->x, i->y - i->velocity()), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);

		if (i->x < 0)
			i->x = getWidth() - 1 - offset_box - i->curImg().getWidth();

		if (i->x + i->curImg().getWidth() > getWidth() - offset_box)
			i->x = 0;
	}
	for (auto i : vehicleList)
	{
		MovingDir direction = i->getDir();
		i->move(direction);
		if (direction == right_)
			FillArea(Position(i->x - i->velocity(), i->y), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == left_)
			FillArea(Position(i->x + i->velocity(), i->y), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == up_)
			FillArea(Position(i->x, i->y + i->velocity()), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);
		if (direction == down_)
			FillArea(Position(i->x, i->y - i->velocity()), L' ', i->preImg().getWidth(), i->preImg().getHeight(), bgColor);

		if (i->x < 0)
			i->x = getWidth() - 1 - offset_box - i->curImg().getWidth();

		if (i->x + i->curImg().getWidth() > getWidth() - offset_box) 
			i->x = 0;
	} 
}
void Game::drawObject()
{
	for (auto i : animalList)
	{
		i->playAnim();
		drawPicture(i->x, i->y, i->curImg(), bgColor | i->getColor());
	}
	for (auto i : vehicleList)
	{
		i->playAnim();
		drawPicture(i->x, i->y, i->curImg(), bgColor | i->getColor());
	}

	for (auto i : lights)
	{
		i->run();
		drawPicture(i->x, i->y, trafficLight, bgColor | FG_DARK_GREY);
		if (i->isStop())
		{
			setPosColor(i->x + 1, i->y + 1, COLOUR::BG_RED | FG_RED);
			for (auto j : vehicleList)
				if (i->sameLine(j)) 
					j->is_moving = false;
			for (auto j : animalList)
				if (i->sameLine(j))
					j->is_moving = false;
		}
		else
		{
			setPosColor(i->x + 1, i->y + 2, COLOUR::BG_GREEN | FG_GREEN);
			for (auto j : vehicleList)
				if (i->sameLine(j))
					j->is_moving = true;
			for (auto j : animalList)
				if (i->sameLine(j))
					j->is_moving = true;
		}
	}
}
void Game::drawPlayer()
{
	player->playAnim();
	drawPicture(player->x, player->y, player->curImg(), bgColor | player->getColor()); // we can change this later
}
void Game::Level1()
{
	// line 1
	vehicleList.push_back(new Truck(10, 5, left_, 1));
	vehicleList.push_back(new Car(50, 6, left_, 1));
	animalList.push_back(new Deer(90, 5, left_, 1));

	// line 2
	vehicleList.push_back(new Car(10, 16, right_, 2));
	animalList.push_back(new Deer(50, 15, right_, 2));
	animalList.push_back(new Deer(90, 15, right_, 2));

	// u can random if u want
	lights.push_back(new TrafficLight(170, 15, 1000, 5000)); // (x, y, time for red light, time for green light ) count as millisecond

	// line 3 maybe 
	vehicleList.push_back(new Truck(10, 25, left_, 5));
	vehicleList.push_back(new Car(50, 26, left_, 2));
	vehicleList.push_back(new Car(90, 26, left_, 2));

	lights.push_back(new TrafficLight(5, 25, 2000, 3000)); // dont put 2 lights on 1 line with different timer :>, not funny

	// line 4
	vehicleList.push_back(new Truck(10, 35, right_, 4));
	vehicleList.push_back(new Car(50, 36, right_, 4));
	vehicleList.push_back(new Truck(90, 35, right_, 4));
	lights.push_back(new TrafficLight(170, 35, 3000, 4000));
}
void Game::Level2()
{

}
void Game::Level3()
{

}
void Game::Level4()
{

}
void Game::drawGame()
{
	// what about using the damn stack and queue to pop and delete stuff
	int countCar = 0, countTruck = 0, countBird = 0, countLeopard = 0;
	for (auto i : vehicleList)
	{
		if (i->getName() == "Car") countCar++;
		else countTruck++;
	}
	for (auto i : animalList)
	{
		if (i->getName() == "Bird") countBird++;
		else countLeopard++;
	}

	while (state == playGame)
	{
		drawBorder();

		for (auto i : vehicleList)
			updatePosObject();

		updatePosPlayer(MOVING);
		MOVING = ' '; // avoid further moving

		drawString(getWidth() - offset_box + 5, 10, L"MÁU: " + to_wstring(player->getHealth()) + L" ", bgColor | FG_GREEN);
		drawString(getWidth() - offset_box + 5, 14, L"MÀN CHƠI: " + to_wstring(player->getLevel()) + L" ", bgColor | FG_GREEN);

		drawString(getWidth() - offset_box + 5, 16, L"SỐ XE HƠI: " + to_wstring(countCar) + L" ", bgColor | FG_GREEN);
		drawString(getWidth() - offset_box + 5, 17, L"SỐ XE TẢI: " + to_wstring(countTruck) + L" ", bgColor | FG_GREEN);
		drawString(getWidth() - offset_box + 5, 18, L"SỐ CHIM: " + to_wstring(countBird) + L" ", bgColor | FG_GREEN);
		drawString(getWidth() - offset_box + 5, 19, L"SỐ NAI: " + to_wstring(countLeopard) + L" ", bgColor | FG_GREEN);
		// drawing Line or something
		FillArea(Position(1, 9), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);
		//
		FillArea(Position(1, 19), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);
		//
		FillArea(Position(1, 29), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);
		// line 2
		FillArea(Position(1, 39), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);
		// line 3
		FillArea(Position(1, 49), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);

		// draw box and bla bla
		drawBox(getWidth() - 30, 0, 30, getHeight(), bgColor | FG_MAGENTA);
		FillArea(Position(getWidth() - 29, 20), L'━', 28, 1, bgColor | FG_MAGENTA); // draw a line
		addChar(Position(getWidth() - 30, 20), L'┣', bgColor | FG_MAGENTA);
		addChar(Position(getWidth() - 1, 20), L'┫', bgColor | FG_MAGENTA);

		drawObject();
		drawPlayer();

		for (auto i : animalList)
		{
			if (player->isImpact(i))
			{
				playHitSound();
				FillArea(Position(player->x, player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
				drawPicture(player->x, player->y, explosion, bgColor | FG_DARK_RED);

				drawConsole(); // the main thing we need
				Sleep(1000); // hehe lol :33
				FillArea(Position(player->x, player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
				FillArea(Position(player->x, player->y), L' ', explosion.getWidth(), explosion.getHeight(), bgColor);

				if (player->isDead()) goto game_over;

				player->y = getHeight() - player->curImg().getHeight() - 1;
				player->x = (getWidth() - offset_box) / 2 - player->curImg().getWidth();
				break;
			}
		}
		for (auto i : vehicleList)
		{
			if (player->isImpact(i))
			{
				playHitSound();
				FillArea(Position(player->x, player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
				drawPicture(player->x, player->y, explosion, bgColor | FG_DARK_RED);

				drawConsole(); // the main thing we need
				Sleep(1000); // hehe lol :33
				FillArea(Position(player->x, player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
				FillArea(Position(player->x, player->y), L' ', explosion.getWidth(), explosion.getHeight(), bgColor);

				if (player->isDead()) goto game_over;

				player->y = getHeight() - player->curImg().getHeight() - 1;
				player->x = (getWidth() - offset_box) / 2 - player->curImg().getWidth();
				break;
			}
		}
		drawConsole(); // the main thing we need
	}
game_over:
	if (player->isDead())
	{
		FillArea(Position(1, 1), L' ', getWidth() - offset_box - 1, getHeight() - 2, bgColor);
		drawPicture(player->x + explosion.getWidth() / 3, player->y + explosion.getHeight() / 3, tomb, bgColor | FG_DARK_GREY);
		drawString(player->x + tomb.getWidth() / 2 - 2, player->y + tomb.getHeight() + 4, L"PRESS ANY KEY TO RETURN TO MENU ... ", bgColor | FG_BLUE);
		drawPicture(player->x + explosion.getWidth() / 3, player->y + explosion.getHeight() + 3, ded_man, bgColor | FG_DARK_GREY);
		drawConsole();
		Sleep(1000);
	}
	clearConsole();
}