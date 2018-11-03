#ifndef RENDERCANVAS_H
#define RENDERCANVAS_H

#include <QWidget>
#include <QPainter>

class RenderCanvas : public QWidget
{
    Q_OBJECT

private:

	QPixmap pixmap;
	QPixmap transparentBackground;

	float scale = 1;
	float scaleScrollSpeed = 0.005f;

	QPoint translation;

	bool translating = false;
	QPoint mouseStartTranslationPosition;
	QPoint startTranslationPosition;

	bool drawing = false;

public:
    explicit RenderCanvas(QWidget *parent = nullptr);

	bool canvasPointToImagePoint(QPoint, QPoint&);

	QRect getImageBounds();
protected:
	void paintEvent(QPaintEvent *event) override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	void paintBackground();
	void paintImage();

	int scaledInt(int);
	int scaledInt(int, float);

	QSize getScaledImageSize();

signals:

public slots:
};

#endif // RENDERCANVAS_H
