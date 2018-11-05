#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
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
    void colorSliderChanged();
    void colorSliderTextChanged();

private:
    int rgb[3] = { 0 };
    Ui::MainWindow *ui;
    AnimationPreviewWindow animationPreviewWindow;

    std::string getCurrentHexColor();
    void updateToolColor(int[3]);

};

#endif // MAINWINDOW_H
