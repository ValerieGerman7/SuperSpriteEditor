#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#include <QPixmap>
#include <string>


/**
 * @brief The SpriteFrame class
 * Represents a single frame within an animation.
 * Maintains a QImage that can be reused and edited, and
 * provides a pixmap for drawing to the screen.
 */
class SpriteFrame {

	public:
		SpriteFrame();

	// Private Members
	private:
		/**
		 * @brief pixmap
		 * The pixmap version of this frame.
		 * If you plan to draw the same image more than once on the screen, convert it to a QPixmap.
		 */
		QPixmap pixmap;

		/**
		 * @brief image
		 * The image version of this frame.
		 * If you plan to manipulate an image, modify it, change pixels on it, etc., use a QImage.
		 */
		QImage image;

		/**
		 * @brief pixmapValid
		 * Whether the current data in the pixmap member is up to date or not.
		 * Used to allow making changes to the image at one time and then offloading the computation
		 * of the pixmap until the next time the pixmap is needed.
		 */
		bool pixmapValid = false;

		float _width, _height;

	// Useful Public Functions
	public:
		void load(QString);

		float width();

		float height();

		QPixmap& getPixMap();

		QImage& getImage();

        void setImage(QImage&);

	// Private Helper Functions
	private:
		void invalidatePixmap();

		void validatePixmapMaybe();
};

#endif // SPRITEFRAME_H
