#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <thread>
#include "Pictures.h"
#include <string>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
// compile by this code: g++ Menu.cpp -o Menu.exe -lWinmm
using namespace std;
void clearArea(int height, int width, int x, int y)
{
    for (int i = 0; i < height; i++)
    {
        moveToXY(x, y + i);
        for (int j = 0; j < width; j++)
            cout << " ";
    }
}
class Music
{
public:
    Music()
    {
        mciSendString(LPTSTR("open \"Music/BlueDream.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    }
    void play() { mciSendString(LPTSTR("play mp3 repeat"), NULL, 0, NULL); }
    void pause() { mciSendString(LPTSTR("pause mp3"), NULL, 0, NULL); }
    void close() { mciSendString(LPTSTR("close mp3"), NULL, 0, NULL); }
    void newPlay()
    {
        close();
        mciSendString(LPTSTR("open \"Music/BlueDream.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
        play();
    }
};
class Menu : public Position
{
    int counter, hoverColor, defaultColor, background_color;
    bool OnMusic;
    Music* MP3;

    vector<Animation*> MenuFrames;

    bool stillGet;
    thread* gameThread;

    vector<string> saveFile; // there would be a class of save things then we can load them all
    GameState state;

public:
    Menu() {}
    ~Menu();          // we will write out new file save in the destructor to avoid user quit the game without saving process
    Menu(int PosX, int PosY, int hoverColor_, int TextColor, int backgroundColor_);
    void startMenu(); // the main loop for input, no drawing or moving cursor here
    // void exitMenu()
    // {
    //     stillGet = false;
    //     mainMenu->join();
    // }
    void drawMenu();
    void drawMenuPlay();
    void drawMenuOption();
    void drawLoadMenu();
    void readSaveFile();
    void playAnim();
    void drawIntroBlabla();
};
Menu::~Menu()
{
    fstream file("savegame.txt", ios::out);
    for (auto i : saveFile)
        file << i << endl;
    file.close();
}
Menu::Menu(int PosX, int PosY, int hoverColor_, int TextColor, int backgroundColor_)
{
    x = PosX;
    y = PosY;
    counter = 2;
    hoverColor = hoverColor_;
    defaultColor = TextColor;
    background_color = backgroundColor_;
    OnMusic = true;
    MP3 = new Music();
    MP3->play();

    stillGet = true;
    gameThread = new thread(&Menu::drawMenu, this);
    state = mainMenu;

    readSaveFile();
    srand(time(NULL));
    MenuFrames.push_back(new Animation(man, 500, Position(x, y - 6), background_color, 12, "right"));
    MenuFrames.push_back(new Animation(octoAnim, 500, Position(x, y + 10), background_color, 3, "left"));
    MenuFrames.push_back(new Animation(pumAnim, 700, Position(x + 30, y + 5), background_color, 2, "idle"));
    MenuFrames.push_back(new Animation(introAnim, 500, Position(x + 80, y - 5), background_color, 1, "down"));
}
void SetFontAttribute(int sizeX, int sizeY);
void Menu::drawIntroBlabla()
{
    // SetFontAttribute(0, 16);
    moveToXY(x + 30, y);
    color(background_color, 2);
    cout << "WELCOME TO GROUP 4 GAME";

    moveToXY(x + 30, y + 1);
    color(background_color, 3);
    cout << "   CROSSING ROAD";
}
void Menu::playAnim()
{
    for (auto i : MenuFrames)
    {
        i->play();
    }
}
void Menu::readSaveFile()
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
void Menu::startMenu()
{
    while (stillGet)
    {
        char key = getch();
        /*if (key == 'u')
            SetFontAttribute(0, 24);
        else
            SetFontAttribute(0, 18);*/
        if (state == mainMenu)
        {
            if (key == 'w' && counter > 1)
                counter--;
            if (key == 's' && counter < 3)
                counter++;

            if (key == '\r')
            {
                if (counter == 1)
                {
                    state = playMenu;
                    gameThread->join();                                 // shutdown mainMenu thread
                    gameThread = new thread(&Menu::drawMenuPlay, this); // start the playMenu thread
                }
                if (counter == 2)
                {
                    counter = 1;
                    state = optionMenu;
                    gameThread->join();                                   // shutdown mainMenu thread
                    gameThread = new thread(&Menu::drawMenuOption, this); // start the optionMenu thread
                }
                if (counter == 3) // exit the game
                {
                    state = exitGame;
                    gameThread->join();      // shutdown mainMenu thread
                    moveToXY(x + 12, y + 2); // non can break except for this
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
                }
                if (counter == 2)
                {
                    state = loadGame;
                    gameThread->join();
                    gameThread = new thread(&Menu::drawLoadMenu, this);
                }
                if (counter == 3)
                {
                    state = mainMenu; // back to the main menu thread
                    gameThread->join();
                    gameThread = new thread(&Menu::drawMenu, this);
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
                        MP3->close();
                        OnMusic = false;
                    }
                    else
                    {
                        MP3->newPlay();
                        OnMusic = true;
                    }
                }
                else
                {
                    state = mainMenu;
                    gameThread->join();
                    gameThread = new thread(&Menu::drawMenu, this);
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
                    gameThread->join();
                    gameThread = new thread(&Menu::drawMenuPlay, this);
                }
                else
                {
                    // load data to the player and start the game thread
                    // or u can make another Menu list like remove save files, edit save file name
                }
            }
        }
        else if (state == playGame)
        {
            // start the new Game, thread, input new nameFile save for the game
            // if (key == 'w') // get the input file here
        }
    }
}
void Menu::drawLoadMenu()
{
    while (state == loadGame)
    {
        drawIntroBlabla();

        moveToXY(x, y);
        if (counter == 1)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "<--Back to Menu-->";

        for (int i = 0; i < saveFile.size(); i++)
        {
            moveToXY(x, y + i + 1);
            if (counter == i + 2)
                color(background_color, hoverColor);
            else
                color(background_color, defaultColor);
            cout << i + 1 << "." << saveFile[i] << "         ";
        }
    }
    clearArea(saveFile.size() + 2, 20, x, y);
}
void Menu::drawMenuOption()
{
    clearArea(1, 10, x, y + 2);
    while (state == optionMenu)
    {
        playAnim();
        drawIntroBlabla();

        moveToXY(x, y);
        if (counter == 1)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "1.Music: ";
        if (OnMusic)
            cout << " ON ";
        else
            cout << " OFF";

        moveToXY(x, y + 1);
        if (counter == 2)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "2.Back to Menu";
    }
}
void Menu::drawMenuPlay()
{
    clearArea(3, 20, x, y);
    while (state == playMenu)
    {
        playAnim();
        drawIntroBlabla();

        moveToXY(x, y);
        if (counter == 1)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "1.New Game ";

        moveToXY(x, y + 1);
        if (counter == 2)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "2.Load Game";

        moveToXY(x, y + 2);
        if (counter == 3)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "3.Back to Menu";
    }
}

void Menu::drawMenu()
{
    clearArea(3, 20, x, y);
    while (state == mainMenu)
    {
        playAnim();
        drawIntroBlabla();

        moveToXY(x, y);
        if (counter == 1)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "1.Play Game";

        moveToXY(x, y + 1);
        if (counter == 2)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "2.Option";

        moveToXY(x, y + 2);
        if (counter == 3)
            color(background_color, hoverColor);
        else
            color(background_color, defaultColor);
        cout << "3.Quit";
    }
}

void SetWindowSize(int height, int width)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width;
    WindowSize.Bottom = height;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    // SetWindowPos(GetConsoleWindow(), 0, 0, 0, width * 100, height * 16, SWP_SHOWWINDOW | SWP_NOMOVE);
}
void noCursor()
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void SetFontAttribute(int sizeX, int sizeY) // used to design the font of the text
{
    CONSOLE_FONT_INFOEX letter;
    letter.cbSize = sizeof(letter);
    letter.nFont = 0;
    letter.dwFontSize.X = sizeX;  // Width of each character in the font
    letter.dwFontSize.Y = sizeY; // Height
    letter.FontFamily = FF_DONTCARE;
    letter.FontWeight = FW_NORMAL;
    wcscpy(letter.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &letter);
}
int main()
{
    int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 50;
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetConsoleTitle(TEXT("My Weird Game"));
	SetFontAttribute(0, 24);
    int x = 30, y = 10;
    system("color f0"); // remember to change this color too, its the whole background color
    noCursor();
    Menu GAME = Menu(30, 10, 0, 8, 15);
    GAME.startMenu();
}
