#include "Pictures.h"
picture::picture(vector<string> temp)
{
    image = temp;
    height = temp.size();
    width = 0;
    for (auto i : temp)
        if (i.size() > width)
            width = i.size();
}
void color(int backgroundColor, int text_color)
{
    int wColor = backgroundColor * 16 + text_color; // 16*16 + 0
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void moveToXY(int x, int y)
{
    COORD cursor;
    cursor.X = x;
    cursor.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}
void display_img(picture k, int x, int y, int backgroundColor, int TextColor)
{
    color(backgroundColor, TextColor);
    for (int i = 0; i < k.getHeight(); i++)
    {
        moveToXY(x, y + i);
        cout << k.getImage()[i];
    }
}
void updatePos(picture image, int x, int y)
{
    for (int i = 0; i < image.getHeight(); i++)
    {
        for (int j = 0; j < image.getWidth(); j++)
        {
            moveToXY(x + j, y + i);
            cout << " ";
        }
    }
}
//Animation::Animation()
//{
//
//}
//Animation::Animation(vector<picture> list, int duration, Position posXY, int backgroundColor, int TextColor, MovingDir dir = idle_)
//{
//	frames = list;
//	max_cooldown = duration;
//	cooldown = 0;
//	x = posXY.x;
//	y = posXY.y;
//	backColor = backgroundColor;
//	textColor = TextColor;
//	track = 0;
//	direction = dir;
//}
void Animation::play()
{
	if (cooldown == 0)
	{
		if (direction == right_)
		{
			x++;
			updatePos(frames[track], x - 1, y);
			if (x + frames[track].getWidth() > 120)
				x = 0;
		}
		if (direction == left_)
		{
			x--;
			updatePos(frames[track], x + 1, y);
			if (x < 0)
				x = 120;
		}
		if (direction == up_)
		{
			y--;
			updatePos(frames[track], x, y + 1);
			if (y < 0)
				y = 20;
		}
		if (direction == down_)
		{
			y++;
			updatePos(frames[track], x, y - 1);
			if (y + getCurFrame().getWidth() > 40)
				y = 0;
		}
		cooldown = max_cooldown;
		if (track < frames.size())
		{
			display_img(frames[track], x, y, backColor, textColor);
			track++;
			if (track == frames.size())
				track = 0;
		}
	}
	else
		cooldown--;
}