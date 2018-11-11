#ifndef SPRITEMODEL_H
#define SPRITEMODEL_H

#include "animation.h"
#include "spriteframe.h"
#include "sseio.h"

#include <QObject>

class SpriteModel : public QObject
{
    Q_OBJECT

private:
    Animation animation;
    int currentIndex = 0;

public:
    explicit SpriteModel(QObject *parent = nullptr);

    Animation& getAnimation();

    void setAnimation(Animation);

    SpriteFrame& getCurrentFrame();

    SpriteFrame& getFrame(int);

    void notifyOfFrameChange();

    int getAnimationLength();

    void flipCurrentFrameHorizontally();

    void flipCurrentFrameVertically();

    void rotateCurrentFrameClockWise();

    void rotateCurrentFrameAntiClockWise();

	void createNewAnimation(int, int);

    SSEIO io;

signals:
    void currentFrameChanged();
	void animationChanged();

public slots:
    void setCurrentFrame(int);
    Animation& getAnimationSlot();

};

#endif // SPRITEMODEL_H
