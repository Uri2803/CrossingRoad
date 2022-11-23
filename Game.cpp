#include "Game.h"
Game::Game()
{
	OnMusic = true;
	bgColor = COLOUR::BG_WHITE;
	playing = true;
	BuildConsole(210, 49, 16, COLOUR::BG_WHITE);
	FillArea(Position(0, 0), L' ', getWidth(), getHeight(), bgColor);
	readSaveFile();
	state = mainMenu;
	scene = new thread(&Game::drawMenu, this);
	playTheme();
}
Game::Game(int width_, int height_, int fontSize, short color)
{
	OnMusic = true;
	BuildConsole(width_, height_, fontSize, color);
	playing = true;
	bgColor = color;
	FillArea(Position(0, 0), L' ', getWidth(), getHeight(), bgColor);
	readSaveFile();
	state = mainMenu;
	scene = new thread(&Game::drawMenu, this);
	playTheme();

}
Game::~Game()
{
	fstream file("savegame.txt", ios::out);
	for (auto i : saveFile)
		file << i << endl;
	file.close();

	delete scene;
}
void Game::drawBorder()
{
	// drawing border, first frame to create border
	short borderColor = COLOUR::BG_WHITE | FG_DARK_GREEN;
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
	fstream file("savegame.txt", ios::in);
	string temp;
	while (!file.eof())
	{
		getline(file, temp, '\n');
		if (temp != "")
			saveFile.push_back(temp);
	}
	file.close();
}
void Game::writeSaveFile()
{
	
}
string name = "", curFileName; // for new file name
char MOVING;
int counter = 2, index = counter;
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
					scene->join();                                 // shutdown mainMenu thread
					scene = new thread(&Game::drawMenuPlay, this); // start the playMenu thread
				}
				if (counter == 2)
				{
					counter = 1;
					state = optionMenu;
					scene->join();                                   // shutdown mainMenu thread
					scene = new thread(&Game::drawMenuOption, this); // start the optionMenu thread
				}
				if (counter == 3)
				{
					state = credits;
					scene->join();
					scene = new thread(&Game::drawMenuCredit, this);
				}
				if (counter == 4) // exit the game
				{
					state = exitGame;
					scene->join();      // shutdown mainMenu thread
					// moveToXY(x + 12, y + 2); // non can break except for this
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
					scene->join();
					scene = new thread(&Game::drawNewGame, this);
				}
				if (counter == 2)
				{
					state = loadGame;
					scene->join();
					scene = new thread(&Game::drawLoadMenu, this);
					counter = 1;
				}
				if (counter == 3)
				{
					state = mainMenu; // back to the main menu thread
					scene->join();
					scene = new thread(&Game::drawMenu, this);
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
					scene->join();
					scene = new thread(&Game::drawMenu, this);
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
					scene->join();
					scene = new thread(&Game::drawMenuPlay, this);
				}
				else
				{
					// load data to the player and start the game thread
					// or u can make another Menu list like remove save files, edit save file name
					state = editSave;
					counter = 1;
					scene->join();
					scene = new thread(&Game::drawEditSave, this);
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
					scene->join();
					scene = new thread(&Game::drawLoadMenu, this);
				}
				else if (counter == 2)
				{
					string _name = saveFile[index] + ".txt";
					int k = _name.length();
					char *Name = new char[k+1];
					strcpy(Name, _name.c_str());
					remove(Name);
					saveFile.erase(saveFile.begin() + index);
					state = loadGame;
					counter = index + 1;
					scene->join();
					scene = new thread(&Game::drawLoadMenu, this);
				}
				else if (counter == 3)
				{
					curFileName = saveFile[index];
					string _name = curFileName + ".txt";
					fstream file(_name, ios::in);
					int _x = 0, _y = 0;
					file >> _x >> _y;
					file.close();
					player = new Player(Position(_x, _y), carAnimator);
					state = playGame;
					scene->join();
					scene = new thread(&Game::drawGame, this);
				}
			}
		}
		else if (state == credits)
		{
			if (key == '\r')
			{
				state = mainMenu;
				scene->join();
				scene = new thread(&Game::drawMenu, this);
			}
		}
		else if (state == newGame)
		{
			if (key == '\r' && !name.empty())
			{
				state = playGame;
				scene->join();
				scene = new thread(&Game::drawGame, this);
				saveFile.push_back(name);
				curFileName = name;
				player = new Player(Position(5, 5), carAnimator);
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
					scene->join();
					scene = new thread(&Game::drawGame, this);
				}
				if (counter == 2)
				{
					// save thing or somth
					string _name = curFileName + ".txt";
					fstream file(_name, ios::out);
					file << player->x<<" "<<player->y;
					file.close();
					//saveFile.push_back(name);
				}
				if (counter == 3)
				{
					state = mainMenu; // back to the main menu thread
					scene->join();
					scene = new thread(&Game::drawMenu, this);
				}
			}
		}
		else if (state == playGame) // the main game back end system
		{
			MOVING = key;
			if (key == 27)
			{
				// do the pause menu
				state = pauseMenu;
				scene->join();
				scene = new thread(&Game::drawPauseMenu, this);
				counter = 1;
			}
			// start the new Game, thread, input new nameFile save for the game
			// if (key == 'w') // get the input file here
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
			L"21127456 - VÕ CAO TRÍ",
			L"21127730 - HOÀNG LÊ CÁT THANH",
			L"21127149 - HUỲNH MINH QUANG"
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

int offset_box = 30;
bool Game::validMove(picture img, Position pos, int vel, MovingDir dir)
{
	if (pos.x - vel < 0 && dir == left_ || pos.x + vel + img.getWidth() >= getWidth() - offset_box && dir == right_)
		return false;
	if (pos.y - vel < 0 && dir == up_ || pos.y + img.getHeight() + vel >= getHeight() && dir == down_)
		return false;
	return true;
}
void Game::updatePosPlayer(char key)
{
	if (key == 'd' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), right_))
	{
		player->move(right_);
		if (OnMusic)
			playKeyBoardSound();
		FillArea(Position(player->x - player->velocity(), player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
	}
	if (key == 'a' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), left_))
	{
		player->move(left_);
		if (OnMusic)
			playKeyBoardSound();
		FillArea(Position(player->x + player->velocity(), player->y), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
	}
	if (key == 'w' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), up_))
	{
		player->move(up_);
		if (OnMusic)
			playKeyBoardSound();
		FillArea(Position(player->x, player->y + player->velocity()), L' ', player->curImg().getWidth(), player->curImg().getHeight(), bgColor);
	}
	if (key == 's' && validMove(player->curImg(), Position(player->x, player->y), player->velocity(), down_))
	{
		player->move(down_);
		if (OnMusic)
			playKeyBoardSound();
		FillArea(Position(player->x, player->y - player->velocity()), L' ', car1.getWidth(), car1.getHeight(), bgColor);
	}
}
void Game::drawPlayer()
{
	drawPicture(player->x, player->y, player->curImg(), bgColor | FG_RED);
}
void Game::drawGame()
{
	drawBox(getWidth() - 30, 0, 30, getHeight(), bgColor | FG_MAGENTA);
	FillArea(Position(getWidth() - 29, 20), L'━', 28, 1, bgColor | FG_MAGENTA); // draw a line
	addChar(Position(getWidth() - 30, 20), L'┣', bgColor | FG_MAGENTA);
	addChar(Position(getWidth() - 1, 20), L'┫', bgColor | FG_MAGENTA);

	while (state == playGame)
	{
		drawBorder();
		updatePosPlayer(MOVING);
		MOVING = ' '; // avoid further moving
		FillArea(Position(1, 20), L'━', getWidth() - offset_box - 1, 1, bgColor | FG_DARK_GREEN);
		drawPlayer();
		drawConsole(); // the main thing we need
	}
	clearConsole();
}