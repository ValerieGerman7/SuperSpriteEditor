#ifndef ANIMATIONPREVIEWWINDOW_H
#define ANIMATIONPREVIEWWINDOW_H

#include <QWidget>

namespace Ui {
class AnimationPreviewWindow;
}

class AnimationPreviewWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationPreviewWindow(QWidget *parent = nullptr);
    ~AnimationPreviewWindow();

private slots:
    void on_closeButton_clicked();

private:
    Ui::AnimationPreviewWindow *ui;
};

#endif // ANIMATIONPREVIEWWINDOW_H
