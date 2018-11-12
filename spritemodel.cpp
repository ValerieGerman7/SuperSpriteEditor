#include "spritemodel.h"
#include <iostream>
#include <QDebug>
SpriteModel::SpriteModel(QObject *parent) : QObject(parent)
{
    //Default size
	animation = Animation();
    SpriteFrame newFrame;
    newFrame.load("://drhenrykillinger");
    animation.insertFrame(0, newFrame);
}

/**
 * @brief Returns the animation object
 * @return
 */
Animation& SpriteModel::getAnimation() {
    return animation;
}

void SpriteModel::setAnimation(Animation anim){
    animation = anim;
    currentIndex = 0;
    previewIndex = 0;
    notifyOfFrameChange();
	emit animationChanged();
}

void SpriteModel::setCurrentFrame(int index) {
	if ( index >= animation.length() ) {
		std::cerr << "SpriteModel::setCurrentFrame: Frame index out of  bounds" << std::endl;
	} else {
		currentIndex = index;
		notifyOfFrameChange();
	}
}

SpriteFrame& SpriteModel::getCurrentFrame() {
    return animation.getFrame(currentIndex);
}

SpriteFrame& SpriteModel::getFrame(int index) {
	if ( index >= animation.length() ) {
		std::cerr << "SpriteModel::getFrame: Frame index out of  bounds" << std::endl;
		return animation.getFrame(0);
	}
    return animation.getFrame(index);
}

int SpriteModel::getAnimationLength() {
    return animation.length();
}

void SpriteModel::notifyOfFrameChange() {
    emit currentFrameChanged();
}

void SpriteModel::flipCurrentFrameHorizontally() {
    SpriteFrame& frame = getCurrentFrame();
    QImage image = frame.getImage().mirrored(true, false);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::flipCurrentFrameVertically() {
    SpriteFrame& frame = getCurrentFrame();
    QImage image = frame.getImage().mirrored(false, true);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::rotateCurrentFrameClockWise() {
    SpriteFrame& frame = getCurrentFrame();
    QTransform transform;
    transform.rotate(90);
    QImage image = frame.getImage().transformed(transform);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::rotateCurrentFrameAntiClockWise() {
    SpriteFrame& frame = getCurrentFrame();
    QTransform transform;
    transform.rotate(-90);
    QImage image = frame.getImage().transformed(transform);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::createNewAnimation(int width, int height) {
	auto animation = Animation(width, height);
	setAnimation(animation);
<<<<<<< HEAD
    emit animationChanged();
=======
}

/**
 * @brief SpriteModel::incrementPreviewIndex
 * @return
 *
 * Increments the index, then returns the index of the currently previewed sprite.
 */
int SpriteModel::incrementPreviewIndex(){
//	std::cout << getAnimationLength() << std::endl;
//	if ((getAnimationLength() - 1)==0) {
//		std::cout << "zero" << std::endl;
//	}
	previewIndex += 1;
	if ( previewIndex >= getAnimationLength() ) {
		previewIndex = 0;
	}
//	previewIndex = ++previewIndex % (getAnimationLength() - 1);
	return previewIndex;
>>>>>>> a8cfede1a0e4cc18b491dfc98aabbf24256cd863
}

/**
 * @brief SpriteModel::getPreviewIndex
 * @return
 *
 * Returns the index of the currently previewed sprite.
 */
<<<<<<< HEAD
int SpriteModel::getAndIncrementPreviewIndex(bool shouldIncrement){
    if(shouldIncrement){
        int animationLength = getAnimationLength();
        if(animationLength <= 1){
            previewIndex = 0;
            //Cannot mod zero
            return previewIndex;
        }
        previewIndex = (++previewIndex) % (animationLength - 1);
        return previewIndex;
    }
    else{
        return previewIndex;
    }
=======
int SpriteModel::getPreviewIndex(){
	return previewIndex;
>>>>>>> a8cfede1a0e4cc18b491dfc98aabbf24256cd863
}

int SpriteModel::getCurrentIndex(){
    return currentIndex;
}
