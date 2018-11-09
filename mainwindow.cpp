#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawingtools.h"
#include "sseio.h"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <string>
#include <QObject>

MainWindow::MainWindow(SpriteModel& model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateToolColor(rgb);

    this->model = &model;

    // tool related signals and slots
    connect(ui->drawButton, SIGNAL(pressed()), this, SLOT(setUsePen()));
    connect(ui->fillButton, SIGNAL(pressed()), this, SLOT(setUseFill()));

    // palette related signals and slots
    connect(ui->redSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->greenSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->blueSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->redSliderText, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));
    connect(ui->greenSlidertext, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));
    connect(ui->blueSliderText, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));

    connect(ui->addToPaletteBtn, SIGNAL(released()), this, SLOT(addCurrentColorToPalette()));
    connect(ui->paletteTable, SIGNAL(cellClicked(int,int)), this, SLOT(setColorFromPalette(int, int)));
    connect(ui->clearPaletteBtn, SIGNAL(released()), this, SLOT(clearPalette()));

    // Render canvas
    ui->renderAreaPlaceHolder->setModel(model); // give the canvas a ref to the model
    connect(&model, &SpriteModel::currentFrameChanged, ui->renderAreaPlaceHolder,
            static_cast<void (QWidget::*)()>(&QWidget::repaint));
            // there's a few QWidget.repaint() functions, so cast/force it to the zero parameters one

    // Flip and rotate buttons
    connect(ui->flipHorizontalButton, &QPushButton::pressed,
            &model, &SpriteModel::flipCurrentFrameHorizontally );
    connect(ui->flipVerticalButton, &QPushButton::pressed,
            &model, &SpriteModel::flipCurrentFrameVertically );
    connect(ui->rotateLeftButton, &QPushButton::pressed,
            &model, &SpriteModel::rotateCurrentFrameAntiClockWise );
    connect(ui->rotateRightButton, &QPushButton::pressed,
            &model, &SpriteModel::rotateCurrentFrameClockWise );

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveToFile()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadFromFile()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

    timeline = new AnimationTimeline(ui->verticalLayout);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete timeline;
}

void MainWindow::on_previewAnimationButton_clicked()
{
    animationPreviewWindow.show();
}

/**
 * Returns the current color in hex format. Useful for stylesheets.
 */
std::string MainWindow::getCurrentHexColor() {
    std::stringstream stream;
    stream << "#" << std::hex << std::setfill('0')
           << std::setw(2) << rgb[0] << std::setw(2) << rgb[1] << std::setw(2) << rgb[2];
    return stream.str();
}

/**
 * Updates tool color and ui elements that represent the current color when
 * the value of a palette slider has been changed.
 */
void MainWindow::colorSliderChanged() {
    rgb[0] = ui->redSlider->value();
    rgb[1] = ui->greenSlider->value();
    rgb[2] = ui->blueSlider->value();
    ui->redSliderText->setText(QString::number(rgb[0]));
    ui->greenSlidertext->setText(QString::number(rgb[1]));
    ui->blueSliderText->setText(QString::number(rgb[2]));
    updateToolColor(rgb);
}

/**
 * Updates tool color and ui elements that represent the current color when
 * the text of a palette slider has been changed.
 */
void MainWindow::colorSliderTextChanged() {
    rgb[0] = ui->redSliderText->text().toInt();
    rgb[1] = ui->greenSlidertext->text().toInt();
    rgb[2] = ui->blueSliderText->text().toInt();
    ui->redSlider->setValue(rgb[0]);
    ui->greenSlider->setValue(rgb[1]);
    ui->blueSlider->setValue(rgb[2]);
    updateToolColor(rgb);
}

/**
 * Updates the tool color and the box that represents the tool color.
 */
void MainWindow::updateToolColor(int _rgb[]) {
    QColor color(_rgb[0], _rgb[1], _rgb[2], 255);
    DrawingTools::toolColor = color;
    //ui->renderAreaPlaceHolder->toolColor = (color);
    ui->currentColorText->setText(getCurrentHexColor().c_str());
    QString style = QString("background-color: %1; border: 2px solid #000000;").arg(getCurrentHexColor().c_str());
    ui->colorPreview->setStyleSheet(style);
}

/**
 * Adds the currently selected tool color to the palette.
 */
void MainWindow::addCurrentColorToPalette() {
    int row = paletteCount / ui->paletteTable->columnCount();
    int col = paletteCount % ui->paletteTable->columnCount();

    ui->paletteTable->setItem(row, col, new QTableWidgetItem);
    ui->paletteTable->item(row, col)->setBackgroundColor(QColor(rgb[0], rgb[1], rgb[2], 255));
    paletteCount++;

    // what would be the best way to handle a full palette?
    if (row >= ui->paletteTable->rowCount() - 1 && col >= ui->paletteTable->columnCount() - 1) {
        qDebug("too many colors for palette");
        ui->addToPaletteBtn->setEnabled(false);
    }
}

/**
 * Sets the tool color to the color selected from the palette.
 */
void MainWindow::setColorFromPalette(int row, int col) {

    // if selecting a position with no value set, do nothing
    if (!ui->paletteTable->item(row, col)) {
        return;
    }

    QColor paletteColor = ui->paletteTable->item(row, col)->backgroundColor();
    paletteColor.getRgb(&rgb[0], &rgb[1], &rgb[2]);
    updateToolColor(rgb);
}

void MainWindow::clearPalette() {
    ui->paletteTable->clear();
    ui->addToPaletteBtn->setEnabled(true);
    paletteCount = 0;
}

void MainWindow::setUsePen() {
    DrawingTools::currentTool = DrawingTools::PEN;
    ui->fillButton->setChecked(false);
}

void MainWindow::setUseFill() {
    DrawingTools::currentTool = DrawingTools::FILL;
    ui->drawButton->setChecked(false);
}

void MainWindow::saveToFile() {
    SSEIO io; //placeholder default io object
//    Animation anim; //placeholder default Animation object
    QString saveFileName = fileDialog.getSaveFileName(this,
        tr("Save As .ssp"), "",
        tr("Sprite Sheet Project (*.ssp)"));
    io.save(model->getAnimation(),saveFileName);
}

void MainWindow::loadFromFile() {
    SSEIO io; //paceholder default io object
    QString loadFileName = fileDialog.getOpenFileName(this,
        tr("Open .ssp File"), "",
        tr("Sprite Sheet Project (*.ssp)"));
    auto animation = io.load(loadFileName);
    model->setAnimation(animation);
}

void MainWindow::quit() {
    close();
}

void MainWindow::on_quitButton_clicked()
{
    quit();
}
