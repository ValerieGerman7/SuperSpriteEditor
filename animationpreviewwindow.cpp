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
