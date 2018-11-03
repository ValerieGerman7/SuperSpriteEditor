#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "animationpreviewwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_previewAnimationButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::MainWindow *ui;
    AnimationPreviewWindow animationPreviewWindow;
};

#endif // MAINWINDOW_H
