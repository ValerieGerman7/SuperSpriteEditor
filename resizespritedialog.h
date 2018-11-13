#ifndef RESIZESPRITEDIALOG_H
#define RESIZESPRITEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeSpriteDialog;
}

class ResizeSpriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeSpriteDialog(QWidget *parent = nullptr);
    ~ResizeSpriteDialog();

signals:
    void resizeAnimation(int, int);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ResizeSpriteDialog *ui;
};

#endif // RESIZESPRITEDIALOG_H
