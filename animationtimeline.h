#ifndef ANIMATIONTIMELINE_H
#define ANIMATIONTIMELINE_H
#include "spriteframe.h"
#include "QPushButton"
#include "animation.h"
#include "spritemodel.h"
#include "qrightclickbutton.h"
#include <QObject>
#include <QVBoxLayout>
#include <iostream>

class AnimationTimeline : public QObject {
    Q_OBJECT

public:

    AnimationTimeline(QVBoxLayout*, SpriteModel&, QObject *parent = nullptr);

    ~AnimationTimeline();

    void duplicateFrame(int frameIndex);

    void addNewFrame(SpriteFrame newFrame);

    void insertNewFrame(SpriteFrame, int);

    void deleteFrame(int frameIndex);

    void moveFrame(int frameToMove, int newIndex);

	void resetAnimationTimeline();

private:
    //This timeline's layout
    QVBoxLayout* timelineLayout;

    //The model of the sprite editor
    SpriteModel* model;

    //The size of the buttons in this timeline
    int buttonSize = 50;
    //Stores a QSize to set button icons
    QSize buttonIconSize;

    //The number of tool buttons
    int numToolButtons = 1;

    //The button pointers corresponding to the frames and the
    //tool buttons
    std::vector<QPushButton*> frameButtons;

    //The currently selected button
    QPushButton* selectedButton;
    //The index of the currently selected button
    int selectedButtonIndex;

    void setupNewAnimation();

    void setButtonIcon(size_t index);

    void selectFrameButton(QPushButton*);

    void selectFrameDeletedSelection(QPushButton*);

    QPushButton* toolButtonSetup(QString);

    void removeFrame(QPushButton*);

    void duplicateFrame(QPushButton*);

    void moveFrameDown(QPushButton*);

public slots:
    void selectFrame();

	void showContextMenu(QPoint);

    void addNewBlankFrame();

signals:
    void setSelectedFrame(int index);
    Animation getAnimation();

};

#endif // ANIMATIONTIMELINE_H
