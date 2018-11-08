#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "animationpreviewwindow.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer *previewPaneUpdateTimer;

private slots:
    void on_previewAnimationButton_clicked();
    void on_quitButton_clicked();

    // tool slots
    void setUsePen();
    void setUseFill();

    // palette slots
    void colorSliderChanged();
    void colorSliderTextChanged();
    void addCurrentColorToPalette();
    void setColorFromPalette(int, int);
    void clearPalette();

    //preview pane slots
    void nextFrame();
    void on_previewFpsSlider_valueChanged(int value);

private:
    int rgb[3] = { 0 };
    int paletteCount = 0;
    Ui::MainWindow *ui;
    AnimationPreviewWindow animationPreviewWindow;

    std::string getCurrentHexColor();
    void updateToolColor(int[3]);

};

#endif // MAINWINDOW_H
