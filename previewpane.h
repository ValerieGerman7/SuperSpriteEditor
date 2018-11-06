#ifndef PREVIEWPANE_H
#define PREVIEWPANE_H

#include <QLabel>
#include "spriteframe.h"
#include <vector>
#include <iterator>

class PreviewPane : public QLabel
{

//Members

private:
    /**
     * @brief height
     * Height of the preview pane, used for scaling the current sprite frame.
     */
    int height;

    /**
     * @brief width
     * Width of the preview pane, used for scaling the current sprite frame.
     */
    int width;

    /**
     * @brief fps
     * Rate at which frames should remain displayed in the preview pane.
     */
    int fps;

    /**
     * @brief spriteFrames
     * The list of all sprite frames which the preview pane must cycle through.
     */
    std::vector<SpriteFrame> spriteFrames;

    /**
     * @brief frameIndex
     * index of currently displayed sprite frame.
     */
    int frameIndex;
public:

    PreviewPane(int,int, std::vector<SpriteFrame>);
    void play();
    int getHeight();
    int getWidth();
    int getFPS();
    void setWidth(int);
    void setHeight(int);
    void setFPS(int);
private:

    void update();
    void updateFrameList(SpriteFrame);
};

#endif // PREVIEWPANE_H
