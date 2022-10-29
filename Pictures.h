#pragma once
#ifndef PICTURE
#define PICTURE
#include <iostream>
using namespace std;
class picture // the system to enhance image and make it work
{
public:
    char image[100][100];
    int height, width;
    picture() {}                                    // for easier use as field in OOP
    picture(const char img[100][100], int h, int w) // creating prototype picture for future use
    {
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                image[i][j] = img[i][j];
        height = h;
        width = w;
    }
};
// this is how we made model or initialized things
// special type of animation
const char car_model1[2][100][100] = {
    {{"   ____"},
     {"__/_|__\\----__"},
     {"|(+)-------(+)|"}},

    {{"   ____"},
     {"__/_|__\\----__"},
     {"|(x)-------(x)|"}}};
// const picture car1(car_model1[0], 3, 16); // the true picture one
const picture car1(car_model1[0], 3, 16);
const picture car2(car_model1[1], 3, 16);

const char nguoi[100][100] =
    {
        {"(x) "},
        {"/|\\"},
        {"/ \\"}};
const picture nguoi1(nguoi, 3, 4);

const char human_anim[2][100][100] = {
    {{" 0"},
     {"/|\\"},
     {"|||"},
     {" ||"},
     {"/ |"}},

    {{" 0"},
     {"/|\\/"},
     {"\\|"},
     {" |\\"},
     {"/ /"}}};
// const picture human(human_anim[0], 5, 4);
const picture walk1(human_anim[0], 5, 4);
const picture walk2(human_anim[1], 5, 5);

const char ship_model1[100][100] =
    {
        {"     //     ______[_]======   ______"},
        {"____[T]____|__|     |________/     /"},
        {"\\_________________________________/ "}};
const picture ship1(ship_model1, 3, 36);

const char intro[100][100] =
    {
        {" __        __"},
        {"/  \\______/  \\"},
        {"| (.)     (.)|"},
        {"|  ________  |"},
        {"|             |"},
        {"\\____________/"}};
const picture intro1(intro, 6, 19);

const char intro_model1[100][100] =
    {
        {" __        __"},
        {"/  \\______/  \\"},
        {"| (.)     (.)|"},
        {"|  ________  |"},
        {"|  \\______/  |"},
        {"\\____________/"}};
const picture intro2(intro_model1, 6, 19);

const char carAttack2_model[100][100] =
    {
        {"  ==={MG]___"},
        {" ____/__|__\\__"},
        {"|(x)=======(x)|"}};
const picture carAttack2(carAttack2_model, 3, 16);
const char bullet_model[100][100] = {
    {">=>"}};
const picture bullet1(bullet_model, 1, 3);

const char ghost2[100][100] =
{
    {"  ________ "},
    {" /        \\"},
    {" | __  __ |"},
    {" |        |"},
    {" |        |"},
    {"/_/\\_/\\_/\\/"}};
const picture ghost_2(ghost2, 6, 11);
const char ghost1[100][100] =
{
    {"  ________ "},
    {" /        \\"},
    {" | ()  () |"},
    {" |        |"},
    {" |        |"},
    {"/_/\\_/\\_/\\/"} };
const picture ghost_1(ghost1, 6, 11);

const char pum[100][100] =
{
    {"  ___________ || ___________ "},
    {" /           \\||/           \\"},
    {"/             \\/             \\"},
    {"|       /\\          /\\       |"},
    {"|      /__\\        /__\\      |"},
    {"|             /\\             |"},
    {"|            /__\\            |"},
    {"|    _________    _______    |"},
    {"\\    \\   __   \\__/ __   /    /"},
    {" \\    \\_/  \\______/  \\_/    /"},
    {"   ------------------------"}};
const picture pum_1(pum, 11, 33);
const char pum1[100][100] =
{
    {"  ___________ || ___________ "},
    {" /           \\||/           \\"},
    {"/             \\/             \\"},
    {"|       /\\          /\\       |"},
    {"|      /__\\        /__\\      |"},
    {"|             /\\             |"},
    {"|            /__\\            |"},
    {"|    \\                 /     |"},
    {"\\     -----------------      /"},
    {" \\      \\/        \\/        /"},
    {"   ------------------------"} };
const picture pum_2(pum1, 11, 33);

#endif
