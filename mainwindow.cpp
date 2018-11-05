#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->redSlider, SIGNAL(sliderMoved(int)), this, SLOT(redSliderChanged(int)));
    connect(ui->greenSlider, SIGNAL(sliderMoved(int)), this, SLOT(greenSliderChanged(int)));
    connect(ui->blueSlider, SIGNAL(sliderMoved(int)), this, SLOT(blueSliderChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_previewAnimationButton_clicked()
{
    animationPreviewWindow.show();
}

std::string MainWindow::getCurrentHexColor() {
    std::stringstream stream;
    stream << "#" << std::hex << std::setfill('0')
           << std::setw(2) << rgb[0] << std::setw(2) << rgb[1] << std::setw(2) << rgb[2];
    return stream.str();
}

void MainWindow::redSliderChanged(int val) {
    ui->redSliderText->setText(QString::number(val));
    rgb[0] = val;
    updateToolColor(rgb[0], rgb[1], rgb[2]);
}

void MainWindow::greenSliderChanged(int val) {
    ui->greenSlidertext->setText(QString::number(val));
    rgb[1] = val;
    updateToolColor(rgb[0], rgb[1], rgb[2]);
}

void MainWindow::blueSliderChanged(int val) {
    //int val = ui->blueSlider->value();
    ui->blueSliderText->setText(QString::number(val));
    rgb[2] = val;
    updateToolColor(rgb[0], rgb[1], rgb[2]);
}

void MainWindow::updateToolColor(int r, int g, int b) {
    QColor color(r, g, b, 255);
    ui->renderAreaPlaceHolder->toolColor = (color);
    ui->currentColorText->setText(getCurrentHexColor().c_str());
    QString style = QString("background-color: %1; border: 2px solid #000000;").arg(getCurrentHexColor().c_str());
    ui->colorPreview->setStyleSheet(style);
}


void MainWindow::on_quitButton_clicked()
{
    close();
}
