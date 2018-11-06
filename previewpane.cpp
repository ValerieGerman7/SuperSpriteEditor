#include "previewpane.h"

/**
 * @brief PreviewPane::PreviewPane
 * @param width
 * @param height
 * @param vector<SpriteFrame>
 *
 * Constructor that takes stores dimensions that frame should be displayed at
 * and a list of frames to be cycled through.
 */
PreviewPane::PreviewPane(int width, int height, std::vector<SpriteFrame> frames){
    this->width = width;
    this->height = height;
    this->spriteFrames = frames;

    if(spriteFrames.size() != 0){

    }
}

/**
 * @brief PreviewPane::play
 * Method for changing the current frame to the next one.
 */
void PreviewPane::play(){

}

/**
 * @brief PreviewPane::update
 * Private method that updates the preview panes current frame as the user makes changes.
 */
void PreviewPane::update(){

}
