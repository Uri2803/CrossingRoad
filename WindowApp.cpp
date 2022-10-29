#include <iostream>
#include <string.h>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <chrono>
#include <thread>
#include <mmsystem.h>
#include "Pictures.h"
using namespace std;
void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void color(int backgroundColor, int text_color)
{
    int wColor = backgroundColor * 16 + text_color;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void display_img(picture k, int x, int y)
{
    color(15, 6);
    for (int i = 0; i < k.height; i++)
    {
        GotoXY(x, y + i);
        cout << k.image[i];
    }
}
void updatePos(picture image, int x, int y)
{
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width; j++)
        {
            GotoXY(x + j, y + i);
            cout << " ";
        }
    }
}
bool isCollided(picture t1, int x1, int y1, picture t2, int x2, int y2)
{
    // faster and better collision detection
    if (x1 + t1.width - 1 >= x2 && x1 <= x2 + t2.width - 1 &&
        y1 + t1.height - 1 >= y2 && y1 <= y2 + t2.height - 1)
        return true;
    return false;
}
class Objects // this is the base one that would be inherited by the other one, the general things of things
{
private:
    picture image;
    int x, y;

public:
    Objects(picture img, int PosX, int PosY)
    {
        image = img;
        x = PosX;
        y = PosY;
    }
    void display() { display_img(image, x, y); }
    bool isCollided(Objects target)
    {
    }
};
class MapObjects;
class movingObstacle;
class projectile;
class Map // the most important thing in this game that portrait and control almost everything
{
private:
    int row, col;

public:
    // list of friend classes that can access the private members of Map
    friend class player;
    friend class movingObstacle;
    friend class projectile;
    friend void LoadEnemy(Map *tilemap);
    // friend bool isCollided(picture t1, int x1, int y1, picture t2, int x2, int y2, Map *tilemap);
    vector<MapObjects *> things;           // static things that we can add on the map to load
    vector<movingObstacle *> movingThings; // dynamic things on map

    Map() {}
    Map(int x, int y)
    {
        row = y;
        col = x;
    }
    void addToStaticThings(MapObjects *temp) { things.push_back(temp); }
    void addToMovingThings(movingObstacle *temp) { movingThings.push_back(temp); }
    void Update(); // update on every frame of the game
};
class projectile
{
private:
    int accel;
    int x, y;
    Map *world;

public:
    friend class player;
    picture image;
    bool HitTarget;
    projectile(Map *tilemap, picture img, int x_pos, int y_pos)
    {
        x = x_pos;
        y = y_pos;
        image = img;
        accel = 5;
        HitTarget = false;
        world = tilemap;
    }
    void behavior();
};
class movingObstacle
{
    Map *world;
    string direction;

public:
    // friend class Map;
    picture image;
    bool goRight, goDown;
    int x, y;
    int accel;

    movingObstacle(Map *tilemap, picture img, int x_pos, int y_pos, int speed = 2, string dir = "left")
    {
        x = x_pos;
        y = y_pos;
        image = img;
        accel = speed;
        direction = dir;
        world = tilemap;

        // if (direction == "left")
        //     goRight = false;
        // if (direction == "right")
        //     goRight = true;
        // if (direction == "up")
        //     goDown = false;
        // if (direction == "down")
        //     goDown = true;

        // goRight = (rand() % 2 == 1) ? true : false;
        //  goDown = (rand() % 2 == 1) ? true : false;
    }
    void move_right()
    {
        x += accel;
        updatePos(image, x - accel, y);
    }
    void move_left()
    {
        x -= accel;
        updatePos(image, x + accel, y);
    }
    void move_down()
    {
        y += accel;
        updatePos(image, x, y - accel);
    }
    void move_up()
    {
        y -= accel;
        updatePos(image, x, y + accel);
    }
    void behavior();
};

struct MapObjects
{
    string name;
    picture image;
    int x, y;
    MapObjects(string ten, picture img, int PosX, int PosY)
    {
        image = img;
        name = ten;
        x = PosX;
        y = PosY;
    }
};
class player
{
private:
    int accel;
    int x, y;
    int health, score;

public:
    int getHealth() { return health; }
    int getScore() { return score; }
    bool isDead() { return (health == 0); }
    picture image; // the image of this one
    bool isFiring; // trigger to see if theres a bullet or rocket exist on the world;
    Map *world;

    vector<projectile *> bullet_mag;
    player(Map *tilemap, int posX, int posY, int speed = 2)
    {
        x = posX;
        y = posY;
        image = car1;
        accel = speed;
        isFiring = false;
        world = tilemap;
        score = 0;
        health = 100;
    }
    void playerControl();
    void display();
};
void player::playerControl()
{
    if (GetKeyState('D') & 0x8000 && x < world->col - image.width)
    {
        x += accel;
        updatePos(image, x - accel, y);
    }
    if (GetKeyState('A') & 0x8000 && x - accel > 0)
    {
        x -= accel;
        updatePos(image, x + accel, y);
    }
    if (GetKeyState('W') & 0x8000 && y - accel >= 0)
    {
        y -= accel;
        updatePos(image, x, y + accel);
    }
    if (GetKeyState('S') & 0x8000 && y + image.height + accel <= world->row) //&& y + accel <= row - image.height)
    {
        y += accel;
        updatePos(image, x, y - accel);
    }
    if (GetKeyState(' ') & 0x8000 && bullet_mag.size() < 3)
    {
        projectile *bullet = new projectile(world, bullet1, x + 12, y);
        bullet_mag.push_back(bullet);
        isFiring = true;
    }
    if (bullet_mag.empty())
        isFiring = false;
}
void player::display()
{
    display_img(image, x, y);
    if (!bullet_mag.empty() && isFiring)
    {
        for (int i = 0; i < bullet_mag.size(); i++)
        {
            projectile *bullet = bullet_mag[i];
            bullet->behavior();
            display_img(bullet->image, bullet->x, bullet->y);
            if (bullet->x + bullet->image.width + bullet->accel > world->col || bullet->HitTarget)
            {
                updatePos(bullet->image, bullet->x, bullet->y);
                bullet_mag.erase(bullet_mag.begin() + i); // last in first out of stack
                // Beep(999999999, 100);  // just a bit of sound effect
                if (bullet->HitTarget)
                    score++;
                break;
            }
        }
    }
    for (auto i = world->movingThings.begin(); i != world->movingThings.end(); i++)
    {
        auto enemy = *i;
        if (isCollided(image, x, y, enemy->image, enemy->x, enemy->y))
        {
            health -= 10;
            updatePos(enemy->image, enemy->x, enemy->y);
            world->movingThings.erase(i);
            break;
        }
    }
}
void movingObstacle::behavior()
{
    if (x + image.width > world->col)
    {
        updatePos(image, x, y);
        x = 0;
        // y = rand() % 20;
    }
    if (x - accel < 0)
    {
        updatePos(image, x, y);
        x = world->col - image.width;
    }
    if (y < 0) // under development
    {
        updatePos(image, x, y);
        y = world->row;
    }
    if (y > world->row + image.height) // under development
    {
        updatePos(image, x, y);
        y = 0;
    }

    // if (!goRight)
    //     move_left();
    // if (goDown)
    //     move_down();
    // if (!goDown)
    //     move_up();
    // if (goRight)
    //     move_right();

    if (direction == "left")
        move_left();
    if (direction == "down")
        move_down();
    if (direction == "up")
        move_up();
    if (direction == "right")
        move_right();
}
// body of collision function

void projectile::behavior()
{
    x += accel;
    updatePos(image, x - accel, y);
    if (world->movingThings.empty())
        return;
    for (int i = 0; i < world->movingThings.size(); i++)
    {
        movingObstacle *target = world->movingThings[i];
        if (isCollided(image, x, y, target->image, target->x, target->y))
        {
            
            // char particle[100][100] = {
            //     {"x   x   x"},
            //     {"  x x x  "},
            //     {"x x x x x"},
            //     {"  x x x  "},
            //     {"x   x   x"}}; // size 5, 9
            // picture bullet_impact(particle, 5, 9);
            char particle[100][100] = {
                {"x   x"},
                {"  x  "},
                {"x   x"}}; // size 3, 5
            picture bullet_impact(particle, 3, 5);
            display_img(bullet_impact, x, y); // - bullet_impact.height
            HitTarget = true;

            updatePos(bullet_impact, x, y);
            updatePos(target->image, target->x, target->y);
            world->movingThings.erase(world->movingThings.begin() + i);
            break;
        }
    }
}
// void clearscreen() // useless thing ever on stackoverflow :3
// {
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     COORD Position;
//     Position.X = 0;
//     Position.Y = 0;
//     SetConsoleCursorPosition(hOut, Position);
// }
void Map::Update() // update on every frame of the game
{
    for (auto movings : movingThings)
    {
        movings->behavior();
        display_img(movings->image, movings->x, movings->y);
    }
    for (auto stuff : things)
        display_img(stuff->image, stuff->x, stuff->y); // no idea why I had to display img of static thing, maybe avoid being erased
}
void clearArea(int height, int width, int x, int y)
{
    for (int i = 0; i < height; i++)
    {
        GotoXY(x, y + i);
        for (int j = 0; j < width; j++)
            cout << " ";
    }
}
void FixConsoleWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void noCursor()
{
    CONSOLE_CURSOR_INFO Info;
    Info.bVisible = FALSE;
    Info.dwSize = 20;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void SetWindowSize(SHORT height, SHORT width)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width;
    WindowSize.Bottom = height;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}
void drawBorder(int &width, int &height)
{
    GotoXY(0, 0);
    for (int i = 0; i < width + 1; i++)
        cout << "=";
    GotoXY(width, 0);
    for (int i = 0; i < height + 1; i++)
    {
        GotoXY(width + 1, i);
        cout << "|||||";
    }
    GotoXY(width + 10, 10);
    cout << "DARE YOU TO DESTROY THE FUCKING WALL";
    GotoXY(width + 10, 12);
    cout << "W, A, S, D TO MOVE";
    GotoXY(width + 10, 13);
    cout << "SPACE TO FIRE";
    GotoXY(width + 10, 15);
    cout << "SCORE: ";
    GotoXY(width + 10, 16);
    cout << "HEALTH: ";
}
void drawGameOver(int &width, int &height)
{
    GotoXY(0, 0);
    for (int i = 0; i < width + 1; i++)
        cout << "=";
    GotoXY(width, 0);
    for (int i = 0; i < height + 1; i++)
    {
        GotoXY(width + 1, i);
        cout << "|||||";
    }
    GotoXY(width / 2, height / 2);
    cout << "GAME OVER YOU MOTHER FUCKER !!!!";
    GotoXY(width / 2, height / 2 + 1);
}
void MouseClick(picture temp, int x, int y)
{
    // INPUT_RECORD InputRecord;
    // DWORD Events;
    // ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &Events);
    POINT *mousePos = new POINT;
    GetCursorPos(mousePos);

    // int x = mousePos->x, y = mousePos->y;
    GotoXY(x, y);
    cout << mousePos->x << " " << mousePos->y << endl;
    display_img(car1, x, y);

    // if (InputRecord.EventType == MOUSE_EVENT)
    // {
    //     if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    //     {
    //         COORD mousePos = InputRecord.Event.MouseEvent.dwMousePosition;
    //         if (mousePos.X >= x && mousePos.X <= x + car1.width &&
    //             mousePos.Y >= y && mousePos.Y <= y + car1.height)
    //         {
    //             system("CLS");
    //             // GotoXY(mousePos->x, mousePos->y);
    //             // cout << " here ";
    //         }
    //     }
    // }

    // Beep(500, 800);
}
void LoadEnemy(Map *tilemap)
{
    while (tilemap->movingThings.size() < 5)
    {
        int speed = rand() % 4, y = rand() % 40;
        while (y == 0)
            y = rand() % 40;
        while (speed == 0)
            speed = rand() % 4;
        movingObstacle *temp = new movingObstacle(tilemap, carAttack2, tilemap->col - carAttack2.width, y, speed, "left");
        tilemap->addToMovingThings(temp);
        Sleep(100);
    }
}
int main()
{
    srand(time(NULL));
    int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 50;
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_MOUSE_INPUT);
    
    SetConsoleTitle(TEXT("My Weird Game"));
    Map *GAME = new Map(SCREEN_WIDTH, SCREEN_HEIGHT);
    drawBorder(SCREEN_WIDTH, SCREEN_HEIGHT);
    // FixConsoleWindow(); // avoid player try to adjust the size of game window
    int x = 1, y = 1, speed = 1;
    int state = 0;
    bool appear = true;

    player *ship = new player(GAME, x, y, 3);
    system("color f0");
    while (true) // 2 ways to get input. getch() of conio.h or GetKeyState() with 0x8000 of Windows.h
    {
        // picture car = nguoi1; //(state == 0) ? car1 : car2;
        // picture car(human_anim[state], 5, 6);
        LoadEnemy(GAME);
        ship->playerControl();
        ship->display();
        GAME->Update();
        if (state == 0)
            state = 1;
        else
            state = 0;
        noCursor();

        // if (GetAsyncKeyState('D'))
        //     x++;
        // if (GetAsyncKeyState('A'))
        //     x--;
        // if (GetAsyncKeyState('W'))
        //     y--;
        // if (GetAsyncKeyState('S'))
        //     y++;

        // GotoXY(x, y);
        // MouseClick(car, x, y);
        //  bool mouseDown = GetAsyncKeyState(VK_LBUTTON) < 0;
        //  if (mouseDown)
        //  {
        //      MouseClick(car, x, y);
        //      mouseDown = false;
        //  }
        if (GetKeyState('E') & 0x8000)
            Beep(500, 800); // sound Beep with frequency 500 in 800 milisec mean 0.8 sec duration
        if (GetKeyState('Q') & 0x8000)
            break;
        // if (appear)
        // {
        //     x += speed;
        //     display(car, x, y);
        // }
        // updatePos(car, x, y, speed);
        // if (x + car.width > SCREEN_WIDTH)
        // {
        //     int k = x + car.width - SCREEN_WIDTH;
        //     clearArea(car.height, car.width, x + car.width - k, y);
        //     if (x > SCREEN_WIDTH)
        //         x = SCREEN_WIDTH;
        //     // GotoXY(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        //     // break;
        // }

        Sleep(60);
        GotoXY(SCREEN_WIDTH + 18, 15);
        cout << ship->getScore();
        GotoXY(SCREEN_WIDTH + 18, 16);
        cout << "   ";
        GotoXY(SCREEN_WIDTH + 18, 16);
        cout << ship->getHealth();
        if (ship->isDead())
        {
            system("cls");
            drawGameOver(SCREEN_WIDTH, SCREEN_HEIGHT);
            break;
        }
    }
    system("pause");
}