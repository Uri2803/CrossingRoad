#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <thread>
#include "Pictures.h"
#include <string>
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
void updatePos(picture image, int x, int y) // used for animation
{
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width; j++)
        {
            moveToXY(x + j, y + i);
            cout << " ";
        }
    }
}
void drawBorderOrSomething(int x, int y)
{
    moveToXY(x - 5, y - 15);
    cout << "WELCOME TO GROUP 4 GAME";
    moveToXY(x - 5, y - 14);
    cout << "CROSSING ROAD";
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
    Music *MP3;

    vector<Animation *> MenuFrames;

    bool stillGet;
    thread *mainMenu;
    thread *playMenu;
    thread *optionMenu;

    thread *saveGames;       // a thread to display save files
    vector<string> saveFile; // there would be a class of save things then we can load them all

    string state;

public:
    Menu(int PosX, int PosY, int hoverColor_, int TextColor, int backgroundColor_)
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

        mainMenu = new thread(&Menu::drawMenu, this);
        state = "mainMenu";
        readSaveFile();

        MenuFrames.push_back(new Animation(carAnim, 200, Position(x, y - 6), background_color, 12));
        MenuFrames.push_back(new Animation(octoAnim, 400, Position(x, y + 10), background_color, 12));
        MenuFrames.push_back(new Animation(pumAnim, 700, Position(x + 30, y + 5), background_color, 12));
    }
    void getCounter(); // the main loop for input, no drawing or moving cursor here
    void exitMenu()
    {
        stillGet = false;
        mainMenu->join();
    }
    void drawMenu();
    void drawMenuPlay();
    void drawMenuOption();
    void drawLoadMenu();
    void readSaveFile();
    void playAnim();
};
void Menu::playAnim()
{
    for (auto i : MenuFrames)
    {
        i->play();
        // i->x++;
        // updatePos(i->getCurFrame(), i->x - 1, i->y);
        // if (i->x > y + 100)
        //     i->x = 0;
    }
}
void Menu::readSaveFile()
{
    fstream file("savegame.txt", ios::in);
    string temp;
    while (!file.eof())
    {
        getline(file, temp, '\n');
        saveFile.push_back(temp);
    }
    file.close();
}
void Menu::getCounter()
{
    while (stillGet)
    {
        char key = getch();
        if (state == "mainMenu")
        {
            if (key == 'w' && counter > 1)
                counter--;
            if (key == 's' && counter < 3)
                counter++;

            if (key == '\r')
            {
                if (counter == 1)
                {
                    state = "playMenu";
                    mainMenu->join();                                 // shutdown mainMenu thread
                    playMenu = new thread(&Menu::drawMenuPlay, this); // start the playMenu thread
                }
                if (counter == 2)
                {
                    counter = 1;
                    state = "optionMenu";
                    mainMenu->join();                                     // shutdown mainMenu thread
                    optionMenu = new thread(&Menu::drawMenuOption, this); // start the optionMenu thread
                }
                if (counter == 3) // exit the game
                {
                    // exitMenu();
                    state = "exit";
                    mainMenu->join();        // shutdown mainMenu thread
                    moveToXY(x + 12, y + 2); // non can break except for this
                    break;
                }
            }
        }
        else if (state == "playMenu")
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
                    state = "loadGame";
                    playMenu->join();
                    saveGames = new thread(&Menu::drawLoadMenu, this);
                }
                if (counter == 3)
                {
                    state = "mainMenu"; // back to the main menu thread
                    playMenu->join();
                    mainMenu = new thread(&Menu::drawMenu, this);
                }
            }
        }
        else if (state == "optionMenu")
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
                    state = "mainMenu";
                    optionMenu->join();
                    mainMenu = new thread(&Menu::drawMenu, this);
                }
            }
        }
        else if (state == "loadGame")
        {
            if (key == 'w' && counter > 1)
                counter--;
            if (key == 's' && counter < saveFile.size() + 1)
                counter++;
            if (key == '\r')
            {
                if (counter == 1)
                {
                    state = "playMenu";
                    counter = 2;
                    saveGames->join();
                    playMenu = new thread(&Menu::drawMenuPlay, this);
                }
                else
                {
                    // load data to the player and start the game thread
                    // or u can make another Menu list like remove save files, edit save file name
                }
            }
        }
    }
}
void Menu::drawLoadMenu()
{
    while (state == "loadGame")
    {
        playAnim();
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
    while (state == "optionMenu")
    {
        playAnim();
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
    while (state == "playMenu")
    {
        playAnim();
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
    // display_img(octo1, x + 30, y - 10);
    clearArea(3, 20, x, y);
    while (state == "mainMenu")
    {
        playAnim();
        moveToXY(x + 30, y);
        color(background_color, 2);
        cout << "WELCOME TO GROUP 4 GAME";

        moveToXY(x + 30, y + 1);
        color(background_color, 3);
        cout << "   CROSSING ROAD";

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
}
void noCursor()
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void SetFontAttribute() // used to design the font of the text
{
    CONSOLE_FONT_INFOEX letter;
    letter.cbSize = sizeof(letter);
    letter.nFont = 0;
    letter.dwFontSize.X = 0;  // Width of each character in the font
    letter.dwFontSize.Y = 24; // Height
    letter.FontFamily = FF_DONTCARE;
    letter.FontWeight = FW_NORMAL;
    wcscpy(letter.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &letter);
}
int main()
{
    int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 50;
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetConsoleTitle(TEXT("My Game Menu"));
    SetFontAttribute();
    int x = 30, y = 10;
    Menu *GAME = new Menu(x, y, 0, 8, 15);
    system("color f0"); // remember to change this color too, its the whole background color
    noCursor();

    GAME->getCounter();
}
