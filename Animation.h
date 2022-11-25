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
    Animation(vector<picture> list, int duration, short col = COLOUR::BG_WHITE | FG_DARK_YELLOW);
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
const Animation carAnimator(carAnim, 100, COLOUR::BG_WHITE | FG_DARK_MAGENTA);
const Animation manAnimator(manAnim, 300, COLOUR::BG_WHITE | FG_DARK_GREY);
const Animation octoAnimator(octoAnim, 300, COLOUR::BG_WHITE | FG_CYAN);
const Animation pumAnimator(pumAnim, 300, COLOUR::BG_WHITE | FG_DARK_RED);
const Animation introAnimator(introAnim, 300, COLOUR::BG_WHITE | FG_DARK_YELLOW);
#endif // !_ANIMATION_