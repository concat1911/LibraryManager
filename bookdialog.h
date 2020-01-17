#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>
#include "median.h"

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(QWidget *parent = nullptr);
    ~BookDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::BookDialog *ui;
    void SetMessage(QString newMess);
};

#endif // BOOKDIALOG_H
