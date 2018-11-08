#ifndef SPRITEMODEL_H
#define SPRITEMODEL_H

#include "animation.h"
#include "spriteframe.h"

#include <QObject>

class SpriteModel : public QObject
{
    Q_OBJECT

private:
    Animation animation;
    int currentIndex = 0;

public:
    explicit SpriteModel(QObject *parent = nullptr);

    void setCurrentFrame(int);

    SpriteFrame& getCurrentFrame();

    SpriteFrame& getFrame(int);

    void notifyOfFrameChange();

    int getAnimationLength();

    void flipCurrentFrameHorizontally();

    void flipCurrentFrameVertically();

    void rotateCurrentFrameClockWise();

    void rotateCurrentFrameAntiClockWise();

signals:
    void currentFrameChanged();

public slots:
};

#endif // SPRITEMODEL_H
