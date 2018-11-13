#include "qrightclickbutton.h"

QRightClickButton::QRightClickButton(QWidget *parent) : QPushButton(parent) {

}

void QRightClickButton::mouseReleaseEvent(QMouseEvent *event) {
	if(event->button()==Qt::RightButton) {
		emit rightClicked(event->pos());
	}
}
