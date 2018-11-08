#include "animationtimeline.h"

AnimationTimeline::AnimationTimeline(QVBoxLayout* layout, QObject *parent) : timelineLayout(layout)
{
    frameCount = 0;
    //Add the first frame
    addNewBlankFrame();
    //TODO: Set as selected

    QPushButton *plusButton = new QPushButton;
    plusButton->setText("+");
    plusButton->setFixedHeight(30);
    plusButton->setFixedWidth(30);
    QObject::connect(plusButton, &QPushButton::pressed, this,
                     &AnimationTimeline::addNewBlankFrame);
    timelineLayout->addWidget(plusButton, 0, Qt::AlignTop);
    frameCount++;

    frameButtons.push_back(plusButton);
}

AnimationTimeline::~AnimationTimeline(){
    for(size_t i = 0; i < frameCount; i++){
        delete frameButtons[i];
    }
}

void AnimationTimeline::duplicateFrame(int frameIndex){

}

void AnimationTimeline::addNewFrame(SpriteFrame newFrame){
    QPushButton *frameButton = new QPushButton;
    frameButton->setText(QString::number(tempAddingCounter));
    tempAddingCounter++;
    frameButton->setFixedWidth(30);
    frameButton->setFixedHeight(30);

    //Remove plus (if frames on list)
    size_t index = frameCount - 1;
    QPushButton* plusButton;
    bool removePlus = frameCount > 1;
    if(removePlus){
        plusButton = frameButtons.at(index);
        plusButton->setText("++");
        frameButtons.pop_back();
        timelineLayout->removeWidget(plusButton);
    }

    //Add new frame
    frameButtons.push_back(frameButton);
    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);
    frameCount++;

    timelineLayout->addWidget(frameButton, 0, Qt::AlignTop);
    frameButton->show();

    if(removePlus){
        //Add back plus
        frameButtons.push_back(plusButton);
        timelineLayout->addWidget(plusButton);
    }

    std::cout<< "adding frame"<<std::endl;

}

void AnimationTimeline::deleteFrame(int frameIndex){
    QPushButton* remove = frameButtons.at(frameIndex);
    frameButtons.erase(frameButtons.begin() + frameIndex);
    timelineLayout->removeWidget(remove);
    frameCount--;
    delete remove;
}

void AnimationTimeline::moveFrame(SpriteFrame frameToMove, int index){

}

void AnimationTimeline::addNewBlankFrame(){
    SpriteFrame newFrame;
    std::cout << "blank frame" << std::endl;
    addNewFrame(newFrame);
}

/* Select the current frame (to change drawing area)
 * The button's pointer is passed in to identify the frame.
*/
void AnimationTimeline::selectFrame(){
    QPushButton* send = qobject_cast<QPushButton*>(sender());

    //Find index
    //size_t index = find(frameButtons.begin(), frameButtons.end(), send) - frameButtons.begin();
    //verify index?
    //new selected button

    //selectedButton = frameButtons[index];
    //TODO: change frame on screen, indication on button that it is selected
    //QString content = selectedButton->text();
    //std::cout << content << std::endl;
    std::cout << "test" << std::endl;
}
