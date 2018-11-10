#include "sseio.h"
#include "spriteframe.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <QImage>

#include "gif-h/gif.h"

SSEIO::SSEIO() {

}

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
    QColor pixel;
    std::ifstream infile;
    std::string token;
    int SPRITE_HEIGHT;
    int SPRITE_WIDTH;
    int NUM_FRAMES;

    infile.open(path.toStdString());
    if(infile.is_open()) {
        //special case - first line: "[height] [width]\n"
        infile >> token;
        SPRITE_HEIGHT = std::stoi(token);
        infile >> token;
        SPRITE_WIDTH = std::stoi(token);
        //special case - second line: "[num_frames]\n"
        infile >> token;
        NUM_FRAMES = std::stoi(token);
        QSize spriteSize(SPRITE_WIDTH,SPRITE_HEIGHT);
        QImage image(spriteSize,QImage::Format_ARGB32);
        SpriteFrame currentFrame(SPRITE_WIDTH,SPRITE_HEIGHT);

        for(int frameNum = 0; frameNum < NUM_FRAMES; frameNum++) {
            for(int yPos = 0; yPos < SPRITE_HEIGHT; yPos++) {
                for(int xPos = 0; xPos < SPRITE_WIDTH; xPos++) {
                    //get red
                    infile >> token;
                    pixel.setRed(stoi(token));
                    //get green
                    infile >> token;
                    pixel.setGreen(stoi(token));
                    //get blue
                    infile >> token;
                    pixel.setBlue(stoi(token));
                    //get alpha
                    infile >> token;
                    pixel.setAlpha(stoi(token));
                    image.setPixelColor(xPos,yPos,pixel);
                }
            }
            currentFrame.setImage(image);
            anim.insertFrame(frameNum,currentFrame);
        }
        infile.close();
    }

    return anim;
}

/**
 * @brief SSEIO::exportGif
 * Uses the gif.h library to export to gif.
 * @param anim
 * @param path
 */
void SSEIO::exportGif(Animation &anim, QString path) {

    GifWriter *writer;
    int WIDTH = anim.getFrame(0).getPixMap().width();
    int HEIGHT = anim.getFrame(0).getPixMap().height();
    GifBegin(writer, "../test.gif", WIDTH, HEIGHT, 0);


    for (int i = 0; i < anim.length(); i++){
        uchar *bits = anim.getFrame(i).getImage().bits();
        GifWriteFrame(writer, bits, WIDTH, HEIGHT, 0);
    }

    GifEnd(writer);

}


