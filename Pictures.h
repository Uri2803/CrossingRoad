#pragma once
#ifndef PICTURE
#define PICTURE
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;
// for game state
enum GameState { mainMenu, playMenu, optionMenu, loadGame, playGame, pauseMenu, credits, exitGame };
enum MovingDir { left_, right_, up_, down_, idle_ };

class picture
{
    vector<string> image;
    int width, height;
public:
    picture() {}
    picture(vector<string> temp);
    int getWidth() { return width; }
    int getHeight() { return height; }
    vector<string> getImage() { return image; }
};
void color(int backgroundColor, int text_color);
void moveToXY(int x, int y);
void display_img(picture k, int x, int y, int backgroundColor, int TextColor);
void updatePos(picture image, int x, int y);
class Position // to be inherited by manything
{
public:
    int x, y;
    Position() {}
    Position(int xPos, int yPos)
    {
        x = xPos;
        y = yPos;
    }
};
class Animation : public Position
{
    int track, cooldown, max_cooldown;
    int backColor, textColor;
    MovingDir direction;

public:
    vector<picture> frames;
    Animation();
    Animation(vector<picture> list, int duration, Position posXY, int backgroundColor, int TextColor, MovingDir dir = idle_)
    {
        frames = list;
        max_cooldown = duration;
        cooldown = 0;
        x = posXY.x;
        y = posXY.y;
        backColor = backgroundColor;
        textColor = TextColor;
        track = 0;
        direction = dir;
    }
    void play();
    picture getCurFrame() { return frames[track]; }
};
// this is how we made model or initialized things
// special type of animation
const picture car1(vector<string>{
    "   ____",
        "__/_|__\\----__",
        "|(+)-------(+)|"});

const picture car2(vector<string>{
    "   ____",
        "__/_|__\\----__",
        "|(x)-------(x)|"});

const vector<picture> carAnim{ car1, car2 };

const picture nguoi1(vector<string>{
    "(x) ",
        "/|\\",
        "/ \\"});

const picture ship1(vector<string>{
    "     //     ______[_]======   ______",
        "____[T]____|__|     |________/     /",
        "\\_________________________________/ "});

const picture intro1(vector<string>{
    " __        __",
        "/  \\______/  \\",
        "| (.)     (.)|",
        "|  ________  |",
        "|            |",
        "\\____________/"});

const picture intro2(vector<string>{
    " __        __",
        "/  \\______/  \\",
        "| (.)     (.)|",
        "|  ________  |",
        "|    \\__/    |",
        "\\____________/"});

const vector<picture> introAnim{ intro1, intro2 };

const picture carAttack2(vector<string>{
    "  ==={MG]___",
        " ____/__|__\\__",
        "|(x)=======(x)|"});

const picture bullet1(vector<string>{">=>"});

const picture octo2(vector<string>{
    "  ________ ",
        " /        \\",
        " | __  __ |",
        " \\        /",
        " /        \\",
        "/~/\\~/\\~/\\~\\"});

const picture octo1(vector<string>{
    "  ________ ",
        " /        \\",
        " | ()  () |",
        " \\        /",
        " /        \\",
        "/_/\\_/\\_/\\_\\"});

const vector<picture> octoAnim{ octo1, octo2 };

const picture pum1(vector<string>{
    "  ___________ || ___________ ",
        " /           \\||/           \\",
        "/             \\/             \\",
        "|       /\\          /\\       |",
        "|      /__\\        /__\\      |",
        "|             /\\             |",
        "|            /__\\            |",
        "|    _________    _______    |",
        "\\    \\   __   \\__/ __   /    /",
        " \\    \\_/  \\______/  \\_/    /",
        "   ------------------------"});

const picture pum2(vector<string>{
    "  ___________ || ___________ ",
        " /           \\||/           \\",
        "/             \\/             \\",
        "|       /\\          /\\       |",
        "|      /__\\        /__\\      |",
        "|             /\\             |",
        "|            /__\\            |",
        "|    \\                 /     |",
        "\\     -----------------      /",
        " \\      \\/        \\/        /",
        "   ------------------------"});

const vector<picture> pumAnim{ pum1, pum2 };

const picture m1(vector<string>{
    " o ",
        "/|\\",
        "/ \\"
});
const picture m2(vector<string>{
    "\\ o /",
        "  |  ",
        " / \\ "
});
const picture m3(vector<string>{
    "_ o ",
        " /\\",
        "| \\"
});
const picture m4(vector<string>{
    "",
        " ___\\o",
        "/)  | "
});
const picture m5(vector<string>{
    "__|  ",
        "  \\o ",
        "  ( \\"
});
const picture m6(vector<string>{
    "\\ /",
        " | ",
        "/o\\"
});
const picture m7(vector<string>{
    "   |__",
        " o/   ",
        "/ )   "
});
const picture m8(vector<string>{
    "",
        "o/__ ",
        "|  (\\"
});
const picture m9(vector<string>{
    "o _",
        "/\\ ",
        "/ |"
});
const vector<picture> man{ m1,m2,m3,m4,m5,m6,m7,m8,m9,m2,m1 };

const picture title(vector<string>{
    " _____                   _                                    _ ",
        "/  __ \\                 (_)                                  | |",
        "| /  \\/_ __ ___  ___ ___ _ _ __   __ _    _ __ ___   __ _  __| |",
        "| |   | '__/ _ \\/ __/ __| | '_ \\ / _` |  | '__/ _ \\ / _` |/ _` |",
        "| \\__/\\ | | (_) \\__ \\__ \\ | | | | (_| |  | | | (_) | (_| | (_| |",
        " \\____/_|  \\___/|___/___/_|_| |_|\\__, |  |_|  \\___/ \\__,_|\\__,_|",
        "                                  __/ |                        ",
        "                                 |___/                         "
});
#endif
