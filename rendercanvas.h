#ifndef RENDERCANVAS_H
#define RENDERCANVAS_H

#include "spriteframe.h"
#include "spritemodel.h"

#include <QWidget>
#include <QPainter>

/**
 * @brief The RenderCanvas class
 * Qt ui widget that displays the current frame being edited.
 *
 * Handles events:
 * -mouse clicks for tools
 * -zooming in using the scroll wheel
 * -panning the image using the middle mouse button
 *
 * Uses a current position and scale transform:
 * -An x,y translation, that it uses to offset where the frame is drawn
 * -A scale scalar, that it uses to shrink or expand how the frame is drawn
 *
 * Brian Landes, November 3, 2018
 */


class RenderCanvas : public QWidget
{
	Q_OBJECT

// Startup settings
private:

	/**
	 * @brief scaleScrollSpeed
	 * The modifier for how the delta from the scroll wheel effects the scale.
	 */
	const float scaleScrollSpeed = 0.001f;

	/**
	 * @brief minScale
	 */
	const float minScale = 0.1f;

	/**
	 * @brief maxScale
	 */
	const float maxScale = 20;

	const QColor backgroundColor = QColor(50,50,50);

    const QColor checkeredWorkspaceWhiteColor = QColor(250,250,250);

    const QColor checkeredWorkspaceGreyColor = QColor(200,200,200);

    const float checkeredWorkspaceSize = 4;

// Private members
private:

    SpriteModel *model;

//	/**
//	 * @brief transparentBackground
//	 * A resource image that it uses to draw a checkered workspace background
//	 */
//	QPixmap transparentBackground;

	/**
	 * @brief scale
	 * The current scale for the frame
	 */
	float scale = 1;

	/**
	 * @brief translation
	 * The current x,y position translation
	 */
	QPoint translation;

	/**
	 * @brief translating
	 * Used between mouse events to keep track of whether we're changing the
	 *  current translation using middle mouse button and drag
	 */
	bool translating = false;

	/**
	 * @brief mouseStartTranslationPosition
	 * Used between mouse events to keep track of where the mouse started for this translation click and drag
	 */
	QPoint mouseStartTranslationPosition;

	/**
	 * @brief startTranslationPosition
	 * Used between mouse events to keep track of what the translation was before we changed it for this click and drag
	 */
	QPoint startTranslationPosition;

	/**
	 * @brief drawing
	 * Used between mouse events to keep track of whether we're drawing
	 */
	bool drawing = false;


// Useful public methods
public:

	bool canvasPointToImagePoint(QPoint, QPoint&);

	QRect getImageBounds();

    void setModel(SpriteModel&);

	void fitImageToFrame();

// Private Methods
private:

	SpriteFrame& currentFrame();

	void paintGreyWorkspace(QPainter&);

	void paintBackground(QPainter&);

	void paintImage(QPainter&);

	QSize getScaledImageSize();

	int scaledInt(int);

	int scaledInt(int, float);


//Qt override methods
public:
    explicit RenderCanvas( QWidget *parent = nullptr);

protected:

	void paintEvent(QPaintEvent *event) override;

	void mousePressEvent(QMouseEvent *event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;


signals:

public slots:
};

#endif // RENDERCANVAS_H
