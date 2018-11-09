#include "newspritedialog.h"
#include "ui_newspritedialog.h"

NewSpriteDialog::NewSpriteDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewSpriteDialog)
{
	ui->setupUi(this);

	setWindowFlag(Qt::Dialog);
	setModal(true);
}

NewSpriteDialog::~NewSpriteDialog()
{
	delete ui;
}

void NewSpriteDialog::on_buttonBox_accepted()
{
	int width = ui->widthSpinBox->value();
	int height = ui->heightSpinBox->value();
	emit createNewAnimation(width, height);
}
