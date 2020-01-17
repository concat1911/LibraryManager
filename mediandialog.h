#ifndef MEDIANDIALOG_H
#define MEDIANDIALOG_H

#include <QDialog>

namespace Ui {
class MedianDialog;
}

class MedianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianDialog(QWidget *parent = nullptr);
    ~MedianDialog();

private:
    Ui::MedianDialog *ui;
};

#endif // MEDIANDIALOG_H
