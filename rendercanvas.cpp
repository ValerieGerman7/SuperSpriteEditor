#include "rendercanvas.h"
#include <iostream>
#include <QMouseEvent>
#include <QPointF>
#include <math.h>

using namespace std;

/**
 * @brief RenderCanvas
 * Qt constructor
 */
RenderCanvas::RenderCanvas(QWidget *parent) : QWidget(parent) {

	pixmap.load("://drhenrykillinger");
	transparentBackground.load("://background");

}

/**
 * @brief paintEvent
 * Qt method called whenever a paint event occurs
 * @param event
 */
void RenderCanvas::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	paintBackground();

	paintImage();
}

/**
 * @brief canvasPointToImagePoint
 * Converts a position in the canvas into a pixel position in the current frame.
 * Ensures that the position is actually within the current frame.
 * Takes a canvas point and a reference to a QPoint where the result pixel position
 * should be returned into.
 * @return true if the position is within the bounds of the current frame, false otherwise
 */
bool RenderCanvas::canvasPointToImagePoint(QPoint canvasPoint, QPoint& imagePoint) {
	QRect imageRect = getImageBounds();

	if ( imageRect.contains(canvasPoint) ) {
		int x = ((float)canvasPoint.x() - (float)imageRect.x()) / (float)imageRect.width() * (float)pixmap.width();
		int y = ((float)canvasPoint.y() - (float)imageRect.y()) / (float)imageRect.height() * (float)pixmap.height();
		imagePoint.setX(x);
		imagePoint.setY(y);
		return true;
	} else {
		return false;
	}
}

/**
 * @brief getImageBounds
 * Returns the screen rectangle where the current image will be drawn within the canvas area.
 * Uses the current translation and scale.
 * @return
 */
QRect RenderCanvas::getImageBounds() {
	QSize size = getScaledImageSize();
	return QRect( translation.x(), translation.y(), size.width(), size.height() );
}


/**
 * @brief paintBackground
 * Draws the checkered workspace background across the entire canvas
 */
void RenderCanvas::paintBackground() {
	QPainter painter(this);

	QSize canvasSize = this->size();

	QSize backgroundSize = transparentBackground.size();

	for( int x = 0; x < canvasSize.width(); x += backgroundSize.width() ) {
		for( int y = 0; y < canvasSize.height(); y += backgroundSize.height() ) {
			painter.drawPixmap(x,y,transparentBackground);
		}
	}
}

/**
 * @brief paintImage
 * Draws the current frame being edited using the current scale and translation
 */
void RenderCanvas::paintImage() {
	QPainter painter(this);

	QSize scaledSize = getScaledImageSize();

	painter.drawPixmap(translation.x(), translation.y(), scaledSize.width(),  scaledSize.height(), pixmap);
}

/**
 * @brief getScaledImageSize
 * @return Calculates and returns the current frames size in pixels scaled by the current scale
 */
QSize RenderCanvas::getScaledImageSize() {
	int newWidth = scaledInt(pixmap.width());
	int newHeight = scaledInt(pixmap.height());
	return QSize(newWidth, newHeight);
}

/**
 * @brief scaledInt
 * @return Scales a int by the current scale.
 * Just to ensure that float math is used consistently.
 */
int RenderCanvas::scaledInt(int value) {
	return floor( (float)value * scale );
}

/**
 * @brief scaledInt
 * @return Scales a int by the given scalar.
 * Just to ensure that float math is used consistently.
 */
int RenderCanvas::scaledInt(int value, float scale) {
	return floor( (float)value * scale );
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse pressed event occurs
 * @param event
 */
void RenderCanvas::mousePressEvent(QMouseEvent *event) {
	if (event->button()==Qt::LeftButton){
		drawing = true;
	} else
	if (event->button()==Qt::MiddleButton){
		translating = true;
		mouseStartTranslationPosition = event->pos();
		startTranslationPosition = translation;
	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse released event occurs
 * @param event
 */
void RenderCanvas::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button()==Qt::LeftButton){
		drawing = false;
	} else
	if (event->button()==Qt::MiddleButton){
		translating = false;
	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse move event occurs
 * @param event
 */
void RenderCanvas::mouseMoveEvent(QMouseEvent *event) {
	if ( translating ) {
		translation = startTranslationPosition + event->pos() - mouseStartTranslationPosition;
		repaint();
	} else
	if ( drawing ) {
		QPoint imagePoint;
		if ( canvasPointToImagePoint(event->pos(), imagePoint ) ) {
			cout << imagePoint.x() << " " << imagePoint.y() << endl;
			QImage image = pixmap.toImage();

			image.setPixelColor(imagePoint, QColor(255,0,0 ) );

			pixmap = QPixmap::fromImage(image);

			repaint();
		}

	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a scroll wheel event occurs
 * @param event
 */
void RenderCanvas::wheelEvent(QWheelEvent *event) {
	QSize oldScaledSize = getScaledImageSize();

	scale = scale + event->angleDelta().y() * scaleScrollSpeed;

	// Clamp the scale
	if ( scale < minScale ) {
		scale = minScale;
	} else if (scale > maxScale) {
		scale = maxScale;
	}

	// translate the image so that it appears to pivot around the center of the image
	QSize newScaledSize = getScaledImageSize();
	translation.setX( translation.x() - ((float)newScaledSize.width() - (float)oldScaledSize.width()) * 0.5f);

	translation.setY( translation.y() - ((float)newScaledSize.height() - (float)oldScaledSize.height()) * 0.5f);

	repaint();
}
