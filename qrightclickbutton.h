#ifndef QRIGHTCLICKBUTTON_H
#define QRIGHTCLICKBUTTON_H

#include <QMouseEvent>
#include <QPushButton>

class QRightClickButton : public QPushButton
{
	Q_OBJECT
public:
	explicit QRightClickButton(QWidget *parent = nullptr);

protected:
	void mouseReleaseEvent(QMouseEvent *);

signals:
	void rightClicked(QPoint);

public slots:

};

#endif // QRIGHTCLICKBUTTON_H
