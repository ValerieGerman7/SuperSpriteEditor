#ifndef ANIMATION_H
#define ANIMATION_H
#include "spriteframe.h"
#include <vector>


class Animation
{
public:
    std::vector<SpriteFrame> frames;
    int framesPerSecond = 12;

// Methods
public:
	Animation();
	Animation(int,int);

    int length();
    int width;
    int height;

    void insertFrame(int index, SpriteFrame frame = SpriteFrame());
    void removeFrame(int index);
    void moveFrame(int oldIndex, int newIndex);
    SpriteFrame& getFrame(int index);
};

#endif // ANIMATION_H
