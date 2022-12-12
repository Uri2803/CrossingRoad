#pragma once
#ifndef _ANIMATION_
#define _ANIMATION_
#include "Pictures.h"
class Animation
{
    int track, cooldown, max_cooldown;
    short color;
    vector<picture> frames;

public:
    Animation();
    short getCol() { return color; }
    Animation(vector<picture> list, int duration, short col = COLOUR::FG_DARK_YELLOW);
    void play();
    picture getCurFrame() const { return frames[track]; }
    picture getPreFrame() const 
    {
        if (track == 0)
            return frames[frames.size() - 1];
        else
            return frames[track - 1];
    }
};
const Animation carAnimatorRight(carAnim, 100, COLOUR::FG_DARK_MAGENTA);
const Animation carAnimatorLeft(carAnim1, 100, COLOUR::FG_CYAN);
const Animation manAnimator(manAnim, 300, COLOUR::FG_DARK_GREY);
const Animation truckAnimatorLeft(truckAnimLeft, 300, COLOUR::FG_RED);
const Animation truckAnimatorRight(truckAnimRight, 300, COLOUR::FG_DARK_BLUE);
const Animation deerAnimatorLeft(deerAnim_left, 300, COLOUR::FG_DARK_MAGENTA);
const Animation deerAnimatorRight(deerAnim_right, 300, COLOUR::FG_DARK_MAGENTA);
const Animation duckAnimatorLeft(duckAnimLeft, 300, COLOUR::FG_YELLOW);
const Animation duckAnimatorRight(duckAnimRight, 300, COLOUR::FG_YELLOW);
const Animation dancingAnimator(dancing, 300, COLOUR::FG_BLACK);
const Animation introAnimator(introAnim, 300, COLOUR::FG_DARK_BLUE);
const Animation mainChar(mainAnim, 200, COLOUR::FG_BLACK);
#endif // !_ANIMATION_
