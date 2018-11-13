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
    previewPaneUpdateTimer = new QTimer(this);
    previewPaneUpdateTimer->start(1000 / ui->previewFpsSlider->value());

    this->model = &model;

    // tool related signals and slots
    connect(ui->drawButton, SIGNAL(pressed()), this, SLOT(setUsePen()));
    connect(ui->penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
    connect(ui->fillButton, SIGNAL(pressed()), this, SLOT(setUseFill()));
    connect(ui->eraseButton, SIGNAL(pressed()), this, SLOT( setUseEraser()));

    // palette related signals and slots
    connect(ui->colorPreview, SIGNAL(released()), this, SLOT(useColorDialog()));
    connect(ui->redSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->greenSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->blueSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSliderChanged()));
    connect(ui->redSliderText, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));
    connect(ui->greenSliderText, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));
    connect(ui->blueSliderText, SIGNAL(textEdited(const QString&)), this, SLOT(colorSliderTextChanged()));
    connect(ui->addToPaletteBtn, SIGNAL(released()), this, SLOT(addCurrentColorToPalette()));
    connect(ui->paletteTable, SIGNAL(cellClicked(int,int)), this, SLOT(setColorFromPalette(int, int)));
    connect(ui->clearPaletteBtn, SIGNAL(released()), this, SLOT(clearPalette()));

    // Render canvas
    ui->renderArea->setModel(model); // give the canvas a ref to the model
    connect(&model, &SpriteModel::currentFrameChanged, ui->renderArea,
            static_cast<void (QWidget::*)()>(&QWidget::repaint));
            // there's a few QWidget.repaint() functions, so cast/force it to the zero parameters one
    connect(ui->fitSpriteButton, &QPushButton::pressed, ui->renderArea, &RenderCanvas::fitImageToFrame);

    // Flip and rotate buttons
    connect(ui->flipHorizontalButton, &QPushButton::pressed,
            &model, &SpriteModel::flipCurrentFrameHorizontally );
    connect(ui->flipVerticalButton, &QPushButton::pressed,
            &model, &SpriteModel::flipCurrentFrameVertically );
    connect(ui->rotateLeftButton, &QPushButton::pressed,
            &model, &SpriteModel::rotateCurrentFrameAntiClockWise );
    connect(ui->rotateRightButton, &QPushButton::pressed,
            &model, &SpriteModel::rotateCurrentFrameClockWise );
    connect(ui->newSpriteDialog, &NewSpriteDialog::createNewAnimation, &model, &SpriteModel::createNewAnimation );


    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveToFile()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadFromFile()));
    connect(ui->actionExportGIF, SIGNAL(triggered()), this, SLOT(exportToGifFile()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

    //previewpane signals and slots
    connect(previewPaneUpdateTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(&model,&SpriteModel::currentFrameChanged, this, &MainWindow::updatePreviewPane);

    //Timeline
    timeline = new AnimationTimeline(ui->verticalLayout, model);
    connect(timeline, &AnimationTimeline::setSelectedFrame, &model, &SpriteModel::setCurrentFrame);
    connect(&model, &SpriteModel::animationChanged, timeline, &AnimationTimeline::resetAnimationTimeline);


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
 * Opens a flexible color dialog for fine-tuned color selection.
 */
void MainWindow::useColorDialog() {
    QColor c = QColorDialog::getColor(DrawingTools::toolColor);
    ui->redSlider->setValue(c.red());
    ui->greenSlider->setValue(c.green());
    ui->blueSlider->setValue(c.blue());
    colorSliderChanged();
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
    ui->greenSliderText->setText(QString::number(rgb[1]));
    ui->blueSliderText->setText(QString::number(rgb[2]));
    updateToolColor(rgb);
}

/**
 * Updates tool color and ui elements that represent the current color when
 * the text of a palette slider has been changed.
 */
void MainWindow::colorSliderTextChanged() {
    rgb[0] = ui->redSliderText->text().toInt();
    rgb[1] = ui->greenSliderText->text().toInt();
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

    if (row >= ui->paletteTable->rowCount() - 1 && col >= ui->paletteTable->columnCount() - 1) {
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
    ui->eraseButton->setChecked(false);
}

void MainWindow::changePenWidth(int v) {
    DrawingTools::penWidth = v;
}

void MainWindow::setUseFill() {
    DrawingTools::currentTool = DrawingTools::FILL;
    ui->drawButton->setChecked(false);
    ui->eraseButton->setChecked(false);
}

void MainWindow::setUseEraser() {
    DrawingTools::currentTool = DrawingTools::ERASE;
    ui->fillButton->setChecked(false);
    ui->drawButton->setChecked(false);
}

void MainWindow::saveToFile() {
    QString filter = "Sprite Sheet Project (*.ssp)";
    QString saveFileName = fileDialog.getSaveFileName(this,
        tr("Save As .ssp"), "",
        filter, &filter);

    if(saveFileName.isEmpty() || saveFileName.isNull()) {
        return; //user clicked cancel
    }

    size_t saveFileLength = saveFileName.length();
    if(saveFileLength < 5) {
        saveFileName += ".ssp";
    }
    else if((saveFileName.toStdString().substr(saveFileLength - 4, saveFileLength - 1) != ".ssp")) {
        saveFileName += ".ssp";
    }
    saveFileLength = saveFileName.length();
    model->io.save(model->getAnimation(),saveFileName);
}

void MainWindow::loadFromFile() {
    QString loadFileName = fileDialog.getOpenFileName(this,
        tr("Open .ssp File"), "",
        tr("Sprite Sheet Project (*.ssp)"));
    if(loadFileName.isEmpty() || loadFileName.isNull()) {
        return;
    }
    model->setAnimation(model->io.load(loadFileName));
}

void MainWindow::exportToGifFile(){

    QString filter = "GIF Files (*.gif)";
    QString fileName = fileDialog.getSaveFileName(this, tr("Save As .gif"), "",
                                                  filter, &filter);

    if (fileName.isEmpty() || fileName.isNull()){ return; }

    int fileNameLength = fileName.length();
    if (fileNameLength < 5){
        fileName += ".gif";
    }
    else if ((fileName.toStdString().substr(fileNameLength - 4, fileNameLength - 1) != ".gif")){
        fileName += ".gif";
    }

    fileNameLength = fileName.length();


    model->io.exportToGif(model->getAnimation(), fileName);
}

void MainWindow::quit() {
    close();
}

void MainWindow::on_quitButton_clicked()
{
    quit();
}

void MainWindow::nextFrame(){
	int index = model->incrementPreviewIndex();
	SpriteFrame newPreviewPaneSprite = model->getFrame(index);

    ui->previewPane->setPixmap(newPreviewPaneSprite.getPixMap().scaled(ui->previewPane->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->previewPane->repaint();

    if(animationPreviewWindow.isVisible()){
        animationPreviewWindow.nextFrame(newPreviewPaneSprite.getPixMap());
    }
}

void MainWindow::updatePreviewPane(){
	if(model->getCurrentIndex() == model->getPreviewIndex()){
		SpriteFrame updatedPreviewPaneSprite = model->getFrame(model->getPreviewIndex());
        ui->previewPane->setPixmap(updatedPreviewPaneSprite.getPixMap().scaled(ui->previewPane->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->previewPane->repaint();
    }
}
void MainWindow::on_previewFpsSlider_valueChanged(int value)
{
    model->getAnimation().framesPerSecond = value;

    if(value > 0) {
        previewPaneUpdateTimer->start(1000 / value);
    }
    else {
      previewPaneUpdateTimer->stop();
    }
}

void MainWindow::on_actionNew_triggered()
{
    ui->newSpriteDialog->show();
}


