#include "rendercanvas.h"
#include <iostream>
#include <QMouseEvent>
#include <QPointF>
#include <math.h>

using namespace std;

RenderCanvas::RenderCanvas(QWidget *parent) : QWidget(parent) {

	pixmap.load("://drhenrykillinger");
	transparentBackground.load("://background");

}

void RenderCanvas::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	paintBackground();

	paintImage();
}

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

void RenderCanvas::paintImage() {
	QPainter painter(this);

	QSize scaledSize = getScaledImageSize();

	painter.drawPixmap(translation.x(), translation.y(), scaledSize.width(),  scaledSize.height(), pixmap);
}

int RenderCanvas::scaledInt(int value) {
	return floor( (float)value * scale );
}

int RenderCanvas::scaledInt(int value, float scale) {
	return floor( (float)value * scale );
}

QSize RenderCanvas::getScaledImageSize() {
	int newWidth = scaledInt(pixmap.width());
	int newHeight = scaledInt(pixmap.height());
	return QSize(newWidth, newHeight);
}

QRect RenderCanvas::getImageBounds() {
	QSize size = getScaledImageSize();
	return QRect( translation.x(), translation.y(), size.width(), size.height() );
}

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

void RenderCanvas::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button()==Qt::LeftButton){
		drawing = false;
	} else
	if (event->button()==Qt::MiddleButton){
		translating = false;
	}
}

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

void RenderCanvas::wheelEvent(QWheelEvent *event) {
	QSize oldScaledSize = getScaledImageSize();

	scale = scale + event->angleDelta().y() * scaleScrollSpeed;
	if ( scale < .1f ) {
		scale = .1f;
	} else if (scale > 10) {
		scale = 10;
	}

	// translate the image so that it appears to pivot around the center of the image
	QSize newScaledSize = getScaledImageSize();
	translation.setX( translation.x() - ((float)newScaledSize.width() - (float)oldScaledSize.width()) * 0.5f);

	translation.setY( translation.y() - ((float)newScaledSize.height() - (float)oldScaledSize.height()) * 0.5f);


	repaint();
}
