#include "animationtimeline.h"

AnimationTimeline::AnimationTimeline(QVBoxLayout* layout) : timelineLayout(layout)
{
    QPushButton *test = new QPushButton;
    test->setText("Constructed");
    timelineLayout->addWidget(test, 0, Qt::AlignHCenter);
}

void AnimationTimeline::duplicateFrame(int frameIndex){

}

void AnimationTimeline::addNewFrame(SpriteFrame newFrame){

}

void AnimationTimeline::deleteFrame(int frameIndex){

}

void AnimationTimeline::moveFrame(SpriteFrame frameToMove, int index){

}
