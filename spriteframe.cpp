#include "spriteframe.h"
#include <string>

using namespace std;

SpriteFrame::SpriteFrame() {

}

SpriteFrame::SpriteFrame(int width, int height) {
	// making a blank pixmap is easier than a qimage, so we'll do it in reverse this time
	pixmap = QPixmap(width, height );
	pixmapValid = true;
	image = pixmap.toImage();

	_width = image.width();
	_height = image.height();
}

/**
 * @brief SpriteFrame::SpriteFrame
 * Initializes a SpriteFrame object with the given image width and height
 * @param width
 * @param height
 */
SpriteFrame::SpriteFrame(int width, int height) {
    _width = width;
    _height = height;
}

/**
 * @brief SpriteFrame::load
 * @param path
 */
void SpriteFrame::load(QString path ) {
	image.load(path);

	_width = image.width();
	_height = image.height();

	invalidatePixmap();
}

/**
 * @brief SpriteFrame::GetImage
 * Grants access to this frame's QImage for manipulation.
 * @return
 */
QImage& SpriteFrame::getImage() {
	invalidatePixmap();
	return image;
}

void SpriteFrame::setImage(QImage& newImage) {
    image = newImage;
    invalidatePixmap();
}

/**
 * @brief SpriteFrame::GetPixMap
 * @return The current pixmap. It is made if needed.
 */
QPixmap& SpriteFrame::getPixMap() {
	validatePixmapMaybe();
	return pixmap;
}

/**
 * @brief SpriteFrame::InvalidatePixmap
 * Flags the pixmap as requiring an update the next time it is needed.
 */
void SpriteFrame::invalidatePixmap() {
	pixmapValid = false;
}

/**
 * @brief SpriteFrame::ValidatePixmapMaybe
 * Uses the QImage to create the pixmap, but only
 * if it needs to.
 */
void SpriteFrame::validatePixmapMaybe() {
	if ( pixmapValid ) {
		return;
	}

	pixmap = QPixmap::fromImage(image);
	pixmapValid = true;
}

float SpriteFrame::width() {
	return _width;
}

float SpriteFrame::height() {
	return _height;
}
