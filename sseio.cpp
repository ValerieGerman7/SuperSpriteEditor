#include "sseio.h"
#include <fstream>
#include <QImage>

SSEIO::SSEIO() {

}

/*
 * ---Overall Format----
[height] [width]\n
[num_frames]\n
[frame_0]\n
[frame_1]\n
...
[frame_n]\n


----Single Frame----
(Separate rows with a newline)
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] ...\n
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] ...\n
...
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] ...\n
 *
 * */

/**
 * @brief SSEIO::Save
 * Saves a sprite as an .ssp-formatted file
 * @param sprite the Animation object to save
 * @param path the filename used to save the file
 */
void SSEIO::save(Animation& sprite, std::string path) {
    //---Comments are pseudocode and will be removed upon implementation---
    if(sprite.frames.empty()) {
        return;
    }
    //append .ssp to the path if it does not already exist

    std::ofstream outfile;
    QImage image = sprite.frames.at(0).getImage();
    const int SPRITE_HEIGHT = image.height();
    const int SPRITE_WIDTH = image.width();

    //get the sprite's dimensions and use them for "[height] [width]\n"
    //use sprite.frames.size() for "[num_frames]\n"

    //loop over each frame to write its data to the save file
    for(SpriteFrame frame : sprite.frames) {
        //obtain the QImage as QPixmap does not allow pixel access
        image = frame.getImage();
        //iterate over all the pixels in the frame and write them to the .ssp file
        for(int i = 0; i < SPRITE_HEIGHT; i++) {
            for(int j = 0; j < SPRITE_WIDTH; j++) {
                //write "[r] [g] [b] [a] [r] [g] [b] [a] ..." to file pixel by pixel
                //add a \n newline after the RGBA tuple instead of a space for the last pixel in a row

                if(j == SPRITE_WIDTH) {
                    //add \n newline
                }
                else {
                    //add " " space
                }
            }
        }
    }
}

/**
 * @brief SSEIO::Load
 * Load a sprite from an .ssp-formatted file
 * @param path the filename to load the sprite from
 */
Animation& SSEIO::load(std::string path) {
    Animation anim;

    return anim;
}

void SSEIO::exportGif() {

}
