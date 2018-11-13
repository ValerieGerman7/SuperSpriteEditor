#include "animationtimeline.h"
#include "qrightclickbutton.h"

#include <QMenu>

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

    //Set up the animation based on the model's animation object
    setupNewAnimation();
}

/**
 * @brief AnimationTimeline::setupNewAnimation Sets up a new animation.
 * Assumptions: The layout shouldn't contain any widgets. The animation should have at least
 * one frame.
 * The frameButtons vector is reset. The frames of the animation are added. The plus
 * and remove buttons are added and connected.
 */
void AnimationTimeline::setupNewAnimation(){
    //New vector
    frameButtons = std::vector<QPushButton*>();

    //Set up the first frame (already existing in animation, so don't call insertFrame)
	QRightClickButton *frameButton = new QRightClickButton;
    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);
	QObject::connect(frameButton, &QRightClickButton::rightClicked,
		this, &AnimationTimeline::showContextMenu);

    //Add new frame
    frameButtons.push_back(frameButton);
    //Set icon
    setButtonIcon(0);

    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);
    timelineLayout->addWidget(frameButton, Qt::AlignTop);
    frameButton->show();
    //Select new frame
    selectFrameDeletedSelection(frameButton);

    //Plus button tool
    QPushButton *plusButton = toolButtonSetup("+");
    QObject::connect(plusButton, &QPushButton::pressed, this,
                     &AnimationTimeline::addNewBlankFrame);

    //Add any other frames this animation may have
    Animation ani = model->getAnimation();
    for(int i = 1; i < ani.length(); i++){
        addNewFrame(ani.getFrame(i));
    }
}

/**
 * @brief AnimationTimeline::toolButtonSetup Sets up a tool button with the buttonSize
 * settings, the given text and adds to the timelineLayout and frameButtons. pressed
 * connections are not done here.
 * @param buttonText
 * @return
 */
QPushButton* AnimationTimeline::toolButtonSetup(QString buttonText){
    QPushButton *toolButton = new QPushButton;
    toolButton->setText(buttonText);
    toolButton->setFixedHeight(buttonSize);
    toolButton->setFixedWidth(buttonSize);
    toolButton->show();

    timelineLayout->addWidget(toolButton, 0, Qt::AlignTop);
    frameButtons.push_back(toolButton);
    return toolButton;
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
 * @brief AnimationTimeline::duplicateSelectedFrame Duplicates the given frame
 * and inserts the duplicated frame after the given frame's position.
 */
void AnimationTimeline::duplicateFrame(QPushButton* button){
    size_t index = find(frameButtons.begin(), frameButtons.end(), button) - frameButtons.begin();
    duplicateFrame(index);
}

/**
 * @brief Add a new frame to the bottom of the list. This new
 * frame is selected.
 * @param newFrame
 */
void AnimationTimeline::addNewFrame(SpriteFrame newFrame){
    size_t endOfFramesIndex = frameButtons.size() - numToolButtons;
    insertNewFrame(newFrame, endOfFramesIndex);
}

/**
 * @brief Inserts a new frame at the given position. This inserts the frame into the
 * animation object held by the model, creates a new button and adds it to the timeline
 * layout.
 * @param newFrame: SpriteFrame to add
 * @param newFrameIndex: index to add the frame to
 */
void AnimationTimeline::insertNewFrame(SpriteFrame newFrame, int newFrameIndex){
	QRightClickButton *frameButton = new QRightClickButton;

    //Add frame to animation
    model->getAnimation().insertFrame(newFrameIndex, newFrame);

    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);

    //Add new frame
    frameButtons.insert(frameButtons.begin()+(newFrameIndex), frameButton);

    //Set in animation object
    setButtonIcon(newFrameIndex);

	QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);
	QObject::connect(frameButton, &QRightClickButton::rightClicked,
		this, &AnimationTimeline::showContextMenu);


    timelineLayout->insertWidget(newFrameIndex, frameButton, Qt::AlignTop);
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

    //Remove button
    QPushButton* remove = frameButtons.at(frameIndex);
    frameButtons.erase(frameButtons.begin() + frameIndex);

    timelineLayout->removeWidget(remove);
    model->getAnimation().removeFrame(frameIndex);

    //Set new selected
    if(selectedButtonIndex == frameIndex){
        //Select the previous frame
        if(selectedButtonIndex == 0){
            selectFrameDeletedSelection(frameButtons[0]);
        } else {
            selectFrameDeletedSelection(frameButtons[selectedButtonIndex-1]);
        }
    }

    delete remove;
}

/**
 * @brief Moves the frame at the given index to the new index.
 * @param frameToMove
 * @param newIndex
 */
void AnimationTimeline::moveFrame(int frameToMove, int newIndex){
    //Return if either index is out of range
    if(frameToMove < 0 || frameToMove >= (frameButtons.size() - numToolButtons) ||
            newIndex < 0 || newIndex >= (frameButtons.size() - numToolButtons)){
        return;
    }
    //Remove
    SpriteFrame frame = model->getFrame(frameToMove);
    deleteFrame(frameToMove);

    //Reset
    insertNewFrame(frame, newIndex);
}

/**
 * @brief AnimationTimeline::moveSelectedFrameUp Moves the selected frame down (later
 * in animation) one position. Doesn't move if the frame would go out of range.
 */
void AnimationTimeline::moveFrameDown(QPushButton* button){
    size_t index = find(frameButtons.begin(), frameButtons.end(), button) - frameButtons.begin();
    moveFrame(index, index+1);
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
    frameButtons[index]->setIconSize(buttonIconSize);
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
//    selectedButton->setEnabled(false);

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
 * This must be called after the animation object has been changed.
 */
void AnimationTimeline::resetAnimationTimeline() {
    //Delete old frames
    int frameNum = frameButtons.size();
    //Remove all except tool buttons
    for(int i = 0; i < frameNum; i++){
        QPushButton* remove = frameButtons.at(0);
        frameButtons.erase(frameButtons.begin());

        timelineLayout->removeWidget(remove);
        delete remove;
    }
    //Set up the animation for the new animation object
    setupNewAnimation();
}

/**
 * @brief AnimationTimeline::removeSelectedFrame Removes the selected Frame. If the
 * selected frame is the last frame, nothing happens.
 */
void AnimationTimeline::removeFrame(QPushButton* button){
    size_t index = find(frameButtons.begin(), frameButtons.end(), button) - frameButtons.begin();
    deleteFrame(index);
}

void AnimationTimeline::showContextMenu( QPoint pos) // this is a slot
{
	QRightClickButton* source = qobject_cast<QRightClickButton*>(sender());
	// for most widgets
	QPoint globalPos = source->mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

	QMenu myMenu;
    myMenu.addAction("Move Down");
    myMenu.addAction("Duplicate");
    myMenu.addAction("Remove Frame");

	QAction* selectedItem = myMenu.exec(globalPos);
	if (selectedItem) {
		// something was chosen
        if ( selectedItem->text() == "Move Down" ) {
            moveFrameDown(source);
        } else if(selectedItem->text() == "Duplicate"){
            duplicateFrame(source);
        } else if(selectedItem->text() == "Remove Frame"){
            removeFrame(source);
        }
	}
	else {
		// nothing was chosen
	}
}
