#include "animationtimeline.h"

/**
 * @brief AnimationTimeline::AnimationTimeline Sets up buttons corresponding to the frames
 * of this sprite.
 * @param layout The layout that holds the frame buttons
 * @param model The model of this sprite editor
 * @param parent
 */
AnimationTimeline::AnimationTimeline(QVBoxLayout* layout, SpriteModel& model, QObject *parent)
    : timelineLayout(layout), model(&model)
{
    buttonIconSize = QSize(buttonSize, buttonSize);

    //Set up the first frame
    QPushButton *frameButton = new QPushButton;

    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);
    //Add new frame
    frameButtons.push_back(frameButton);
    //Set icon
    setButtonIcon(0);

    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);
    timelineLayout->addWidget(frameButton, Qt::AlignTop);
    frameButton->show();

    selectedButton = frameButton;
    selectedButtonIndex = 0;
    //Select new frame
    selectFrameButton(frameButton);

    //Plus button tool
    QPushButton *plusButton = new QPushButton;
    plusButton->setText("+");
    plusButton->setFixedHeight(buttonSize);
    plusButton->setFixedWidth(buttonSize);
    QObject::connect(plusButton, &QPushButton::pressed, this,
                     &AnimationTimeline::addNewBlankFrame);

    timelineLayout->addWidget(plusButton, 0, Qt::AlignTop);

    frameButtons.push_back(plusButton);



    //Remove button tool
    QPushButton *removeButton = new QPushButton;
    removeButton->setText("-");
    removeButton->setFixedHeight(buttonSize);
    removeButton->setFixedWidth(buttonSize);
    QObject::connect(removeButton, &QPushButton::pressed,
                     this, &AnimationTimeline::removeSelectedFrame);
    removeButton->show();
    timelineLayout->addWidget(removeButton, 0, Qt::AlignTop);
    frameButtons.push_back(removeButton);

}

/**
 * @brief AnimationTimeline::~AnimationTimeline deletes buttons
 */
AnimationTimeline::~AnimationTimeline(){
    size_t numFrames = frameButtons.size();
    for(size_t i = 0; i < numFrames; i++){
        delete frameButtons[i];
    }
}

/**
 * @brief Adds a duplicate of the frame at the given index after that index.
 * @param frameIndex
 */
void AnimationTimeline::duplicateFrame(int frameIndex){
    SpriteFrame duplicate = model->getAnimation().getFrame(frameIndex);
    //TODO: Copy contents of frame
    insertNewFrame(duplicate, frameIndex+1);
}

/**
 * @brief Add a new frame to the bottom of the list. This new
 * frame is selected.
 * @param newFrame
 */
void AnimationTimeline::addNewFrame(SpriteFrame newFrame){
    size_t newFrameIndex = model->getAnimation().length();
    insertNewFrame(newFrame, newFrameIndex);
}

/**
 * @brief Inserts a new frame at the given position. This inserts the frame into the
 * animation object held by the model, creates a new button and adds it to the timeline
 * layout.
 * @param newFrame: SpriteFrame to add
 * @param newFrameIndex: index to add the frame to
 */
void AnimationTimeline::insertNewFrame(SpriteFrame newFrame, int newFrameIndex){
    QPushButton *frameButton = new QPushButton;

    //Add frame to animation
    model->getAnimation().insertFrame(newFrameIndex, newFrame);

    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);

    //Add new frame
    frameButtons.insert(frameButtons.begin()+(newFrameIndex-1), frameButton);

    //Set in animation object
    setButtonIcon(newFrameIndex-1);

    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);

    timelineLayout->insertWidget(newFrameIndex-1, frameButton, Qt::AlignTop);
    frameButton->show();

    //Select new frame
    selectFrameButton(frameButton);
}

/**
 * @brief Remove the button corresponding to the index, and the frame from the animation.
 * If there is only one frame left, that frame is not removed.
 * Selects the frame prior to this frame after deletion, if the first frame is deleted,
 * the new first frame is selected.
 * @param frameIndex
 */
void AnimationTimeline::deleteFrame(int frameIndex){
    if(frameButtons.size() <= 1 + numToolButtons){
        return; //Never removes last frame
    }
    if(selectedButtonIndex == frameIndex){
        //Select the previous frame
        if(selectedButtonIndex == 0){
            selectFrameDeletedSelection(frameButtons[0]);
        } else {
            selectFrameDeletedSelection(frameButtons[selectedButtonIndex-1]);
        }
    }
    QPushButton* remove = frameButtons.at(frameIndex);
    frameButtons.erase(frameButtons.begin() + frameIndex);

    timelineLayout->removeWidget(remove);
    model->getAnimation().removeFrame(frameIndex);

    delete remove;
}

/**
 * @brief Moves the frame at the given index to the new index.
 * @param frameToMove
 * @param newIndex
 */
void AnimationTimeline::moveFrame(int frameToMove, int newIndex){
    //Remove
    SpriteFrame frame = model->getFrame(frameToMove);
    model->getAnimation().removeFrame(frameToMove);
    deleteFrame(frameToMove);

    //Reset
    insertNewFrame(frame, newIndex);
}

/**
 * @brief Set the button at the given index 's icon to the corresponding
 * image in the animation object.
 * @param index
 */
void AnimationTimeline::setButtonIcon(size_t index){
    SpriteFrame frame = model->getFrame(index);

    QPixmap pix = frame.getPixMap();
    QIcon ButtonIcon(pix);
    frameButtons[index]->setIcon(ButtonIcon);
}

/**
 * @brief Add a new blank frame to this animation.
 */
void AnimationTimeline::addNewBlankFrame(){
    SpriteFrame newFrame(model->getAnimation().width, model->getAnimation().height);
    addNewFrame(newFrame);
}

/**
 * @brief Select the current frame (to change drawing area)
 * The button's pointer is passed in to identify the frame.
 * Emits setSelectedFrame to change which frame is selected
 *  in the drawing.
 * A selected button is disabled.
 * area.
 */
void AnimationTimeline::selectFrame(){
    selectFrameButton(qobject_cast<QPushButton*>(sender()));
}

/**
 * @brief Sets the given button to the selected button. The
 * selected button is disabled and the currently editable. The previously
 * selected button must be valid.
 * frame.
 * @param send
 */
void AnimationTimeline::selectFrameButton(QPushButton* send){
    //Refresh old selected button's icon
    setButtonIcon(selectedButtonIndex);
    selectedButton->setEnabled(true);

    selectedButton = send;

    size_t index = find(frameButtons.begin(), frameButtons.end(), send) - frameButtons.begin();
    selectedButtonIndex = index;
    selectedButton->setEnabled(false);

    emit setSelectedFrame(index);
}

/**
 * @brief Selects the given button, assumes the current selectedButton has been
 * deleted.
 */
void AnimationTimeline::selectFrameDeletedSelection(QPushButton* send){
    selectedButton = send;

    size_t index = find(frameButtons.begin(), frameButtons.end(), send) - frameButtons.begin();
    selectedButtonIndex = index;
    selectedButton->setEnabled(false);

    emit setSelectedFrame(index);
}

/**
 * @brief Deletes old frames and corresponding buttons and sets new ones based
 * on the animation object in the model. Selects the first frame in the animation.
 * (Assumes there is always at least one frame in the animation).
 */
void AnimationTimeline::resetAnimationTimeline() {
    //Delete old frames
    int frameNum = frameButtons.size();
    //Remove all except tool buttons
    for(int i = 0; i < frameNum - numToolButtons; i++){
        QPushButton* remove = frameButtons.at(0);
        frameButtons.erase(frameButtons.begin());

        timelineLayout->removeWidget(remove);
    }
    //Add all new frames
    Animation ani = model->getAnimation();
    for(int i = 0; i < ani.length(); i++){
        addNewFrame(ani.getFrame(i));
    }
    selectedButton = frameButtons[0];
    selectedButtonIndex = 0;
    selectedButton->setEnabled(false);
}

/**
 * @brief AnimationTimeline::removeSelectedFrame Removes the selected Frame. If the
 * selected frame is the last frame, nothing happens.
 */
void AnimationTimeline::removeSelectedFrame(){
    deleteFrame(selectedButtonIndex);
}
