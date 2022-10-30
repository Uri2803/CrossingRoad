#pragma once
#ifndef PICTURE
#define PICTURE
#include <iostream>
#include <vector>
using namespace std;
class picture
{
public:
    vector<string> image;
    int width, height;
    picture() {}
    picture(vector<string> temp)
    {
        image = temp;
        height = temp.size();
        width = 0;
        for (auto i : temp)
            if (i.size() > width)
                width = i.size();
    }
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
    "|             |",
    "\\____________/"});

const picture intro2(vector<string>{
    " __        __",
    "/  \\______/  \\",
    "| (.)     (.)|",
    "|  ________  |",
    "|  \\______/  |",
    "\\____________/"});

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

const picture pum_2(vector<string>{
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
#endif
