#include "resizespritedialog.h"
#include "ui_resizespritedialog.h"

ResizeSpriteDialog::ResizeSpriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeSpriteDialog)
{
    ui->setupUi(this);
}

ResizeSpriteDialog::~ResizeSpriteDialog()
{
    delete ui;
}

void ResizeSpriteDialog::on_buttonBox_accepted()
{
    int width = ui->widthBox->value();
    int height = ui->heightBox->value();
    emit resizeAnimation(width, height);
}
