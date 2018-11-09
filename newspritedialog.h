#ifndef NEWSPRITEDIALOG_H
#define NEWSPRITEDIALOG_H

#include <QDialog>

namespace Ui {
class NewSpriteDialog;
}

class NewSpriteDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewSpriteDialog(QWidget *parent = nullptr);
	~NewSpriteDialog();

signals:
	void createNewAnimation(int,int);

private slots:
	void on_buttonBox_accepted();

private:
	Ui::NewSpriteDialog *ui;
};

#endif // NEWSPRITEDIALOG_H
