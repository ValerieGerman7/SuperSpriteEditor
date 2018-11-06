#ifndef ANIMATIONTIMELINE_H
#define ANIMATIONTIMELINE_H
#include "spriteframe.h"
#include "QPushButton"
#include <QVBoxLayout>

class AnimationTimeline
{
public:
    AnimationTimeline(QVBoxLayout*);

    void duplicateFrame(int frameIndex);

    void addNewFrame(SpriteFrame newFrame);

    void deleteFrame(int frameIndex);

    void moveFrame(SpriteFrame frameToMove, int index);

private:
    QVBoxLayout* timelineLayout;

};

#endif // ANIMATIONTIMELINE_H
