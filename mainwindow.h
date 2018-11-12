#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <string>
#include "animationpreviewwindow.h"
#include "spritemodel.h"
#include "animationtimeline.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( SpriteModel& model, QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *previewPaneUpdateTimer;
private slots:
    void on_previewAnimationButton_clicked();
    void on_quitButton_clicked();

    // tool slots
    void setUsePen();
    void changePenWidth(int);
    void setUseFill();
    void setUseEraser();

    // palette slots
    void colorSliderChanged();
    void colorSliderTextChanged();
    void addCurrentColorToPalette();
    void setColorFromPalette(int, int);
    void clearPalette();
    void useColorDialog();

    // IO slots
    void saveToFile();
    void loadFromFile();
    void exportToGifFile();

    void quit();

    // Preview Pane slots
    void nextFrame();
    void on_previewFpsSlider_valueChanged(int value);
    void updatePreviewPane();

    void on_actionNew_triggered();

private:
    int rgb[3] = { 0 };
    int paletteCount = 0;
    Ui::MainWindow *ui;
    AnimationPreviewWindow animationPreviewWindow;
    QFileDialog fileDialog;
    QColorDialog colorDialog;
    SpriteModel *model;

    AnimationTimeline* timeline;

    std::string getCurrentHexColor();
    void updateToolColor(int[3]);


};

#endif // MAINWINDOW_H
