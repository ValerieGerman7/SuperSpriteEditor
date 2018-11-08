#ifndef ANIMATIONPREVIEWWINDOW_H
#define ANIMATIONPREVIEWWINDOW_H

#include <QWidget>

namespace Ui {
class AnimationPreviewWindow;
}

class AnimationPreviewWindow : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit AnimationPreviewWindow(QWidget *parent = nullptr);
    ~AnimationPreviewWindow();

private slots:
    void on_closeButton_clicked();

private:
    Ui::AnimationPreviewWindow *ui;
    void nextFrame(QPixmap);
};

#endif // ANIMATIONPREVIEWWINDOW_H
