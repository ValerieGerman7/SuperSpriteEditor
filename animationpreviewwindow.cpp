#include "animationpreviewwindow.h"
#include "ui_animationpreviewwindow.h"

AnimationPreviewWindow::AnimationPreviewWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimationPreviewWindow)
{
    ui->setupUi(this);
}

AnimationPreviewWindow::~AnimationPreviewWindow()
{
    delete ui;
}

void AnimationPreviewWindow::on_closeButton_clicked()
{
    hide();
}

/**
 * @brief AnimationPreviewWindow::nextFrame
 * @param value
 * Method that updates the preview pane to display the next sprite frame in the animation.
 */
void AnimationPreviewWindow::nextFrame(QPixmap pixmap) {
    ui->previewPane->setPixmap(pixmap);
    ui->previewPane->repaint();
}
