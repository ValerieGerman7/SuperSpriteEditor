#include "sseio.h"
#include "spriteframe.h"
#include "gif-h/gif.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <QImage>
#include <iostream>
#include <string>

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
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a]... [r] [g] [b] [a]\n
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a]... [r] [g] [b] [a]\n
...
[r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a] [r] [g] [b] [a]... [r] [g] [b] [a]\n
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
    SpriteFrame currentFrame;
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

        for(int frameNum = 0; frameNum < NUM_FRAMES; frameNum++) {
            std::cout << "Frame: " << frameNum << std::endl;
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
                    std::cout << "Pixel at (" << xPos << "," << yPos << "): ";
                    std::cout << image.pixelColor(xPos,yPos).red() << ' ' << image.pixelColor(xPos,yPos).green() << ' ' << image.pixelColor(xPos,yPos).blue()
                              << ' ' << image.pixelColor(xPos,yPos).alpha() << std::endl;
                }
            }
            currentFrame.setImage(image);
            anim.insertFrame(frameNum,currentFrame);
            std::cout << image.width() << std::endl;
            std::cout << image.height() << std::endl;
        }
        std::cout << anim.frames.size() << std::endl;
        std::cout << anim.frames.at(0).getImage().width() << std::endl;
        std::cout << anim.frames.at(0).getImage().height() << std::endl;

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
void SSEIO::exportToGIF(Animation &anim, QString path) {

    std::string pathAsString = path.toStdString() + "\0";
    GifWriter *writer;
    int WIDTH = anim.getFrame(0).getPixMap().width();
    int HEIGHT = anim.getFrame(0).getPixMap().height();

    GifBegin(writer, &pathAsString[0], WIDTH, HEIGHT, 0);


    for (int i = 0; i < anim.length(); i++){
        uchar *bits = anim.getFrame(i).getImage().bits();
        GifWriteFrame(writer, bits, WIDTH, HEIGHT, 0);
    }

    GifEnd(writer);

}


