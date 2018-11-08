#include "sseio.h"
#include "spriteframe.h"
#include <fstream>
#include <iterator>
#include <vector>
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
void SSEIO::save(Animation& sprite, QString path) {
    //---Comments are pseudocode and will be removed upon implementation---

    //append .ssp to the path if it does not already exist
    //open outfile
    std::ofstream outfile;

    std::vector<SpriteFrame>::iterator frameIterator;
    frameIterator = sprite.frames.begin();

    if(frameIterator == sprite.frames.end()) {
        return;
    }

    const size_t NUM_FRAMES = sprite.frames.size();
    const int SPRITE_HEIGHT = (*frameIterator).getPixMap().height();
    const int SPRITE_WIDTH = (*frameIterator).getPixMap().width();

    //send "[height] [width]\n[num_frames]\n" to outfile

    while(frameIterator < sprite.frames.end()) {
        QImage image = (*frameIterator).getImage();
        //send frame image's pixel data, row by row, to outfile
        for(int i = 0; i < SPRITE_HEIGHT; i++) {
            for(int j = 0; j < SPRITE_WIDTH; j++) {
                //write "[r] [g] [b] [a] [r] [g] [b] [a] ..." to file pixel by pixel
                //add a \n newline after the RGBA tuple instead of a space for the last pixel in a row

                if(j == SPRITE_WIDTH - 1) {
                    //add \n newline
                }
                else {
                    //add " " space
                }
            }
        }
        frameIterator++;
    }
}

/**
 * @brief SSEIO::Load
 * Load a sprite from an .ssp-formatted file
 * @param path the filename to load the sprite from
 */
Animation& SSEIO::load(QString path) {
    Animation anim;

    return anim;
}

void SSEIO::exportGif() {

}
