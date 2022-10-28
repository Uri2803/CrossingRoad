#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <thread>
#include "Pictures.h"
#pragma comment(lib, "Winmm.lib")
// compile by this code: g++ Menu.cpp -o Menu.exe -lWinmm
using namespace std;
void moveToXY(int x, int y)
{
    COORD cursor;
    cursor.X = x;
    cursor.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}
void color(int backgroundColor, int text_color)
{
    int wColor = backgroundColor * 16 + text_color; // 16*16 + 0
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void display_img(picture k, int x, int y)
{
    color(15, 4);
    for (int i = 0; i < k.height; i++)
    {
        moveToXY(x, y + i);
        cout << k.image[i];
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
class Menu
{
    int x, y;
    int counter, hoverColor, defaultColor, background_color;
    bool OnMusic;
    Music *MP3;

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
    }
    void drawMenu();
    void drawMenuPlay();
    void drawMenuOption();
};
void Menu::drawMenuOption()
{
    if (OnMusic)
        MP3->play();
    int set[] = {defaultColor, defaultColor};
    char key;
    while (true)
    {
        if (counter == 1)
            set[0] = hoverColor;
        else
            set[0] = defaultColor;

        if (counter == 2)
            set[1] = hoverColor;
        else
            set[1] = defaultColor;

        moveToXY(x, y);
        color(background_color, set[0]);
        cout << "1.Music: ";
        if (OnMusic)
            cout << " ON";
        else
            cout << " OFF";

        moveToXY(x, y + 1);
        color(background_color, set[1]);
        cout << "2.Back to Menu";

        moveToXY(x, y + 2);
        cout << "               ";

        key = getch();
        if (key == 'w' && counter > 1)
            counter--;
        if (key == 's' && counter < 2)
            counter++;
        if (key == '\r')
        {
            if (counter == 1)
            {
                moveToXY(x + 12, y);
                if (OnMusic)
                {
                    MP3->close();
                    OnMusic = false;
                    cout << "               ";
                }
                else
                {
                    MP3->newPlay();
                    OnMusic = true;
                    moveToXY(x, y);
                    cout << "               ";
                }
            }
            if (counter == 2)
                break;
        }
    }
    drawMenu();
}
void Menu::drawMenuPlay()
{
    if (OnMusic)
        MP3->play();
    int set[] = {defaultColor, defaultColor, defaultColor};
    char key;
    while (true)
    {
        if (counter == 1)
            set[0] = hoverColor;
        else
            set[0] = defaultColor;

        if (counter == 2)
            set[1] = hoverColor;
        else
            set[1] = defaultColor;

        if (counter == 3)
            set[2] = hoverColor;
        else
            set[2] = defaultColor;

        moveToXY(x, y);
        color(background_color, set[0]);
        cout << "1.New Game ";

        moveToXY(x, y + 1);
        color(background_color, set[1]);
        cout << "2.Load Game";

        moveToXY(x, y + 2);
        color(background_color, set[2]);
        cout << "3.Back to Menu";

        key = getch();
        if (key == 'w' && counter > 1)
            counter--;
        if (key == 's' && counter < 3)
            counter++;

        if (key == '\r')
        {
            if (counter == 1)
                moveToXY(x + 12, y);
            if (counter == 2)
                moveToXY(x + 12, y + 1);
            if (counter == 3)
            {
                goto choice;
            }
            cout << " Event " << counter << " occur ";
        }
    }
choice:
    if (counter == 1)
        cout << "PLAY GAME";
    if (counter == 2)
        cout << "LOAD GAME";
    else
    {
        counter = 2;
        drawMenu();
    }
}

void Menu::drawMenu()
{
    if (OnMusic)
        MP3->play();
    int set[] = {defaultColor, defaultColor, defaultColor};
    char key;
    while (true)
    {
        if (counter == 1)
            set[0] = hoverColor;
        else
            set[0] = defaultColor;

        if (counter == 2)
            set[1] = hoverColor;
        else
            set[1] = defaultColor;

        if (counter == 3)
            set[2] = hoverColor;
        else
            set[2] = defaultColor;

        moveToXY(x, y);
        color(background_color, set[0]);
        cout << "1.Play Game       ";

        moveToXY(x, y + 1);
        color(background_color, set[1]);
        cout << "2.Option      ";

        moveToXY(x, y + 2);
        color(background_color, set[2]);
        cout << "3.Quit        ";

        key = getch();
        if (key == 'w' && counter > 1)
            counter--;
        if (key == 's' && counter < 3)
            counter++;

        if (key == '\r')
        {
            if (counter == 1)
            {
                drawMenuPlay();
            }
            if (counter == 2)
            {
                drawMenuOption();
            }
            if (counter == 3)
            {
                moveToXY(x + 12, y + 2);
                break;
            }
        }
    }
choice:
    if (counter == 1)
        drawMenuPlay();
    if (counter == 2)
    {
        moveToXY(x, y + 2);
        cout << "           ";
        drawMenuOption();
    }
    moveToXY(x / 2, y / 2);
    if (OnMusic)
    {
        OnMusic = false;
        MP3->close();
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

void playAnimation(int x, int y) // under development
{
    int index = 1;
    while (true)
    {
        index = (index == 1) ? 2 : 1;
        if (index == 1)
            display_img(intro1, x, y);
        else
            display_img(intro2, x, y);
        Sleep(100);
    }
}
int main()
{
    int SCREEN_WIDTH = 110, SCREEN_HEIGHT = 60;
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetConsoleTitle(TEXT("My Game Menu"));
    SetFontAttribute();
    int x = 30, y = 10;
    Menu *GAME = new Menu(x, y, 0, 8, 15);
    system("color f0"); // remember to change this color too, its the whole background color
    noCursor();

    // music.join();
    GAME->drawMenu();
    // thread UI(&Menu::drawMenu, *GAME); // require a pointer to member function, an object, parameters (maybe)
    // thread anim(playAnimation, x + 30, y - 5);

    // anim.join();
    //  UI.join();
}