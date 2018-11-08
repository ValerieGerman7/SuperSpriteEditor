#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "animationpreviewwindow.h"
#include "spritemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( SpriteModel& model, QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    int rgb[3] = { 0 };
    int paletteCount = 0;
    Ui::MainWindow *ui;
    AnimationPreviewWindow animationPreviewWindow;

    std::string getCurrentHexColor();
    void updateToolColor(int[3]);

};

#endif // MAINWINDOW_H
