#pragma once
#ifndef PICTURE
#define PICTURE
#include <iostream>
#include <vector>
using namespace std;
class picture
{
    vector<wstring> image;
    int width, height;

public:
    picture() {}
    picture(vector<wstring> temp)
    {
        image = temp;
        height = temp.size();
        width = 0;
        for (auto i : temp)
            if (i.size() > width)
                width = i.size();
    }
    int getWidth() const { return width; } // make sure that they are const
    int getHeight() const { return height; }
    vector<wstring> getImg() const { return image; }
};

// this is how we made model or initialized things
// special type of animation
const picture car1(vector<wstring>{
    L"   ____",
    L"__/_|__\\----__",
    L"|(+)-------(+)|"});

const picture car2(vector<wstring>{
    L"   ____",
    L"__/_|__\\----__",
    L"|(x)-------(x)|"});

const picture ship1(vector<wstring>{
    L"     //     ______[_]======   ______",
    L"____[T]____|__|     |________/     /",
    L"\\_________________________________/ "});

const picture intro1(vector<wstring>{
    L" __        __",
    L"/  \\______/  \\",
    L"| (.)     (.)|",
    L"|  ________  |",
    L"|             |",
    L"\\____________/"});

const picture intro2(vector<wstring>{
    L" __        __",
    L"/  \\______/  \\",
    L"| (.)     (.)|",
    L"|  ________  |",
    L"|  \\______/  |",
    L"\\____________/"});

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

const picture pum1(vector<wstring>{
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
    L"   ------------------------"});

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
        L"   ------------------------"});
const picture dog1(vector<wstring>{
    L"      __",
    L" (___()ˊˋ",
    L"/¸    /`",
    L"//'-- \\\\"
});
const picture dog2(vector<wstring>{
    L"      __",
    L" (___()ˊˋ",
    L"/¸    /`",
    L"\\\\'--//"
});
const picture m1(vector<wstring>{
    L" o ",
    L"/|\\",
    L"/ \\"
});
const picture m2(vector<wstring>{
    L"\\ o /",
        L"  |  ",
        L" / \\ "
});
const picture m3(vector<wstring>{
    L"_ o ",
        L" /\\",
        L"| \\"
});
const picture m4(vector<wstring>{
    L"",
        L" ___\\o",
        L"/)  | "
});
const picture m5(vector<wstring>{
    L"__|  ",
        L"  \\o ",
        L"  ( \\"
});
const picture m6(vector<wstring>{
    L"\\ /",
        L" | ",
        L"/o\\"
});
const picture m7(vector<wstring>{
    L"   |__",
        L" o/   ",
        L"/ )   "
});
const picture m8(vector<wstring>{
    L"",
        L"o/__ ",
        L"|  (\\"
});
const picture m9(vector<wstring>{
    L"o _",
        L"/\\ ",
        L"/ |"
});
const picture title(vector<wstring>{
    L" _____                   _                                    _ ",
        L"/  __ \\                 (_)                                  | |",
        L"| /  \\/_ __ ___  ___ ___ _ _ __   __ _    _ __ ___   __ _  __| |",
        L"| |   | '__/ _ \\/ __/ __| | '_ \\ / _` |  | '__/ _ \\ / _` |/ _` |",
        L"| \\__/\\ | | (_) \\__ \\__ \\ | | | | (_| |  | | | (_) | (_| | (_| |",
        L" \\____/_|  \\___/|___/___/_|_| |_|\\__, |  |_|  \\___/ \\__,_|\\__,_|",
        L"                                  __/ |                        ",
        L"                                 |___/                         "
});
#endif
