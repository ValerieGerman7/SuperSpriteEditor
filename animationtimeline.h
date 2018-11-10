#ifndef ANIMATIONTIMELINE_H
#define ANIMATIONTIMELINE_H
#include "spriteframe.h"
#include "QPushButton"
#include "animation.h"
#include "spritemodel.h"
#include <QObject>
#include <QVBoxLayout>
#include <iostream>

class AnimationTimeline : public QObject {
    Q_OBJECT

public:

    AnimationTimeline(QVBoxLayout*, SpriteModel*, QObject *parent = nullptr);

    ~AnimationTimeline();

    void duplicateFrame(int frameIndex);

    void addNewFrame(SpriteFrame newFrame);

    void deleteFrame(int frameIndex);

    void moveFrame(SpriteFrame frameToMove, int index);

private:
    QVBoxLayout* timelineLayout;

    SpriteModel* model;

    int buttonSize = 40;

    std::vector<QPushButton*> frameButtons;

    QPushButton* selectedButton;
    int tempAddingCounter = 0;

    void setButtonIcon(size_t index);


public slots:
    void addNewBlankFrame();

    void selectFrame();

signals:
    void setSelectedFrame(int index);

};

#endif // ANIMATIONTIMELINE_H
