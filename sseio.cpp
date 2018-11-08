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
    outfile.open(path.toStdString());
    if(outfile.is_open()) {

        std::vector<SpriteFrame>::iterator frameIterator;
        frameIterator = sprite.frames.begin();

        if(frameIterator == sprite.frames.end()) {
            return;
        }

        const size_t NUM_FRAMES = sprite.frames.size();
        const int SPRITE_HEIGHT = (*frameIterator).getPixMap().height();
        const int SPRITE_WIDTH = (*frameIterator).getPixMap().width();

        outfile << SPRITE_HEIGHT << ' ' << SPRITE_WIDTH << std::endl;
        outfile << NUM_FRAMES << std::endl;

        QImage image;
        QColor pixel;

        while(frameIterator < sprite.frames.end()) {
            image = (*frameIterator).getImage();
            //send frame image's pixel data, row by row, to outfile
            for(int row = 0; row < SPRITE_HEIGHT; row++) {
                for(int col = 0; col < SPRITE_WIDTH; col++) {
                    pixel = image.pixel(col,row);
                    outfile << pixel.red() << ' ' << pixel.green() << ' ' << pixel.blue() << ' ' << pixel.alpha();
                    if(col == SPRITE_WIDTH - 1) {
                        outfile << std::endl;
                    }
                    else {
                        outfile << ' ';
                    }
                }
            }
            frameIterator++;
        }
        outfile.close();
    }
}

/**
 * @brief SSEIO::Load
 * Load a sprite from an .ssp-formatted file
 * @param path the filename to load the sprite from
 */
Animation SSEIO::load(QString path) {
    Animation anim;
    SpriteFrame frame;
    std::ifstream infile;
    std::string line;
    infile.open(path.toStdString());
    if(infile.is_open()) {
        if(infile >> line) {

        }
        infile.close();
    }

    return anim;
}

void SSEIO::exportGif() {

}


