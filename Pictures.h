#pragma once
#ifndef _PICTURE_
#define _PICTURE_
#include <iostream>
#include <vector>
using namespace std;
class Position
{
public:
    int x, y;
    Position()
    {
        x = 0;
        y = 0;
    }
    Position(int PosX, int PosY)
    {
        x = PosX;
        y = PosY;
    }
};
enum COLOUR // color initialization
{
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007,
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};
class picture
{
    vector<wstring> image;
    int width, height;

public:
    picture() 
    {
        width = 0;
        height = 0;
    }
    picture(vector<wstring> temp);
    int getWidth() const { return width; } // make sure that they are const
    int getHeight() const { return height; }
    vector<wstring> getImg() const { return image; }
};

// this is how we made model or initialized things
// special type of animation
const picture car1(vector<wstring>{
        L"    _____",
        L" __╱_|___╲____",
        L"╰(+)───────(+)╯"});

const picture car2(vector<wstring>{
        L"    _____",
        L" __╱_|___╲____",
        L"╰(x)───────(x)╯"});
const vector<picture> carAnim{ car1, car2 };

const picture car3(vector<wstring>{
        L"      _____",
        L" ____/___|_╲__",
        L"╰(x)───────(x)╯"
});
const picture car4(vector<wstring>{
        L"      _____",
        L" ____/___|_╲__",
        L"╰(+)───────(+)╯"
});
const vector<picture> carAnim1{ car3, car4 };

const picture truck1(vector<wstring>{
        L"  ___┑",
        L" ╱/_||________",
        L"‖ᴐ___╠─────────┡",
        L"┖(x)───────(x)┚"
});
const picture truck2(vector<wstring>{
        L"  ___┑",
        L" ╱/_||________,",
        L"‖ᴐ___╠─────────┡",
        L"┖(+)───────(+)┚"
});
const vector<picture> truckAnimLeft{ truck1, truck2 };

const picture truck3(vector<wstring>{
        L"         ┍___",
        L",________||_╲╲",
        L"┦─────────╢__с║",
        L"┖(x)───────(x)┚"
});
const picture truck4(vector<wstring>{
        L"         ┍___",
        L" ________||_╲╲",
        L"┦─────────╢__с║",
        L"┖(+)───────(+)┚"
});
const vector<picture> truckAnimRight{ truck3, truck4 };

const picture intro1(vector<wstring>{
    L"T là người chơi",
    L" __        __",
    L"/  \\______/  \\",
    L"| (.)     (.)|",
    L"|  ________  |",
    L"|            |",
    L"\\____________/"});

const picture intro2(vector<wstring>{
    L"T là người chơi",
    L" __        __",
    L"/  \\______/  \\",
    L"| (.)     (.)|",
    L"|  ________  |",
    L"|  \\______/  |",
    L"\\____________/"});

const vector<picture> introAnim{ intro1, intro2 };

const picture carAttack2(vector<wstring>{
    L"  ==={MG]___",
    L" ____/__|__\\__",
    L"|(x)=======(x)|"});

const picture bullet1(vector<wstring>{L">=>"});

const picture octo2(vector<wstring>{
    L"  ________ ",
    L" /        \\",
    L" | __  __ |",
    L" \\        /",
    L" /        \\",
    L"/~/\\~/\\~/\\~\\"});

const picture octo1(vector<wstring>{
    L"  ________ ",
    L" /        \\",
    L" | ()  () |",
    L" \\        /",
    L" /        \\",
    L"/_/\\_/\\_/\\_\\"});

const vector<picture> octoAnim{ octo1, octo2 };

const picture pum_1(vector<wstring>{
    L"  ___________ || ___________ ",
    L" /           \\||/           \\",
    L"/             \\/             \\",
    L"|       /\\          /\\       |",
    L"|      /__\\        /__\\      |",
    L"|             /\\             |",
    L"|            /__\\            |",
    L"|    _________    _______    |",
    L"\\    \\   __   \\__/ __   /    /",
    L" \\    \\_/  \\______/  \\_/    /",
    L"  \\________________________/"});

const picture pum_2(vector<wstring>{
    L"  ___________ || ___________ ",
    L" /           \\||/           \\",
    L"/             \\/             \\",
    L"|       /\\          /\\       |",
    L"|      /__\\        /__\\      |",
    L"|             /\\             |",
    L"|            /__\\            |",
    L"|    \\                 /     |",
    L"\\     -----------------      /",
    L" \\      \\/        \\/        /",
    L"  \\________________________/"});

const vector<picture> pumAnim{pum_1, pum_2};

const picture dog1(vector<wstring>{
    L"      __",
    L" (___()ˊˋ",
    L"/¸    /`",
    L"//'-- ╲╲"
});
const picture dog2(vector<wstring>{
    L"      __",
    L" (___()ˊˋ",
    L"/¸    /`",
    L"╲╲'--//"
});
const vector<picture> dogAnim{ dog1, dog2 };

const picture deer1(vector<wstring>{
        L"    (__)",
        L" Ϛ__/''•",
        L" /___ |",
        L"╱╱   ╲╲"
});
const picture deer2(vector<wstring>{
        L"    (__)",
        L" Ϛ__/''•",
        L" /___ |",
        L" ╲╲ ╱╱"
});
const vector<picture> deerAnim_right{ deer1, deer2 };

const picture deer3(vector<wstring>{
        L"(__)",
        L"•''╲__ɂ",
        L" | ___ ╲",
        L" //    ╲╲"
});
const picture deer4(vector<wstring>{
        L"(__)",
        L"•''╲__ɂ",
        L" | ___ ╲",
        L"  ╲╲  //"
});
const vector<picture> deerAnim_left{ deer3, deer4 };

const picture explosion(vector<wstring>{
        L"         BOOM!!!",
        L"     ˷ ₍˷  ˷ ₎ ˷ ₎˷",
        L"   (˷ ₍ ( ₍  )˷  ₎˷˷)",
        L" ( (  (    )   `)  ) ˷)",
        L"(˷˷ (˷   (˷ ₎ ˷) ˷) ₎˷˷)",
        L"    `~~`╲ ' . /`~~`",
        L"         ;   ;",
        L"         /   ╲",
        L"________/_ __ ╲________"
});

const picture tomb(vector<wstring>{
        L"   _",
        L" _|R|_",
        L"│_ I _│",
        L" ╭|P|╮",
        L"_||_||_"
});

const picture dancing1(vector<wstring>{
        L" (•․•)/",
        L"<)   )",
        L" ╱  ╲"
});
const picture dancing2(vector<wstring>{
        L"╲(•․•)",
        L" (   (>",
        L" ╱  ╲"
});
const vector<picture> dancing{ dancing1, dancing2 };

const picture tree(vector<wstring>{
        L"  _",
        L" ( ) ",
        L"(_X_)",
        L"  j"
});

const picture m1(vector<wstring>{
    L" o ",
    L"╱|╲",
    L"╱ ╲"
});
const picture m2(vector<wstring>{
    L"╲ o ╱",
    L"  |  ",
    L" ╱ ╲ "
});
const picture m3(vector<wstring>{
    L"_ o ",
    L" ╱╲",
    L"| ╲"
});
const picture m4(vector<wstring>{
    L"",
    L" ___╲o",
    L"╱)  | "
});
const picture m5(vector<wstring>{
    L"__|  ",
    L"   ╲o ",
    L"  ( ╲"
});
const picture m6(vector<wstring>{
    L"╲ ╱",
    L" | ",
    L"╱o╲"
});
const picture m7(vector<wstring>{
    L"   |__",
    L" o/   ",
    L"╱ )   "
});
const picture m8(vector<wstring>{
    L"",
    L"o╱__ ",
    L"|  (╲"
});
const picture m9(vector<wstring>{
    L"o _",
    L"╱╲ ",
    L"╱ |"
});
const vector<picture>manAnim{ m1, m2, m3, m4, m5, m6, m7, m8, m9, m2, m1 };

const picture trafficLight(vector<wstring>{
    L"┌─┐",
    L"│O│",
    L"│O│",
    L"└╥┘"
});

const picture title(vector<wstring>{
    L" _____                   _                                    _ ",
    L"/  __ \\                 (_)                                  | |",
    L"| /  \\/_ __ ___  ___ ___ _ _ __   __ _    _ __ ___   __ _  __| |",
    L"| |   | '__/ _ \\/ __/ __| | '_ \\ / _` |  | '__/ _ \\ / _` |/ _` |",
    L"| \\__/\\ | | (_) \\__ \\__ \\ | | | | (_| |  | | | (_) | (_| | (_| |",
    L" \\____/_|  \\___/|___/___/_|_| |_|\\__, |  |_|  \\___/ \\__,_|\\__,_|",
    L"                                  __/ |                        ",
    L"                                 |___/                         "});

#endif
