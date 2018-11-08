#ifndef ANIMATIONTIMELINE_H
#define ANIMATIONTIMELINE_H
#include "spriteframe.h"
#include "QPushButton"
#include <QObject>
#include <QVBoxLayout>
#include <iostream>

class AnimationTimeline : public QObject {
    Q_OBJECT

public:

    AnimationTimeline(QVBoxLayout*, QObject *parent = nullptr);

    ~AnimationTimeline();

    void duplicateFrame(int frameIndex);

    void addNewFrame(SpriteFrame newFrame);

    void deleteFrame(int frameIndex);

    void moveFrame(SpriteFrame frameToMove, int index);

private:
    QVBoxLayout* timelineLayout;

    std::vector<QPushButton*> frameButtons;

    QPushButton* selectedButton;
    int tempAddingCounter = 0;


public slots:
    void addNewBlankFrame();

    void selectFrame();

    void timelineButtonPressSlot(QPushButton*);

};

#endif // ANIMATIONTIMELINE_H
