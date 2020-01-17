#ifndef CDSDIALOG_H
#define CDSDIALOG_H

#include <QDialog>
#include "median.h"

namespace Ui {
class CDsDialog;
}

class CDsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDsDialog(QWidget *parent = nullptr);
    ~CDsDialog();
public:
    void SetMessage(QString newMess);
private slots:
    void on_pushButton_clicked();

private:
    Ui::CDsDialog *ui;
};

#endif // CDSDIALOG_H
