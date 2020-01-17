#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>

#include "person.h"

namespace Ui {
class PersonDialog;
}

class PersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonDialog(QWidget *parent = nullptr);
    ~PersonDialog();

public:
    void SetMessage(QString newMess);

private slots:
    void on_pushButton_clicked();

private:
    Ui::PersonDialog *ui;
};

#endif // PERSONDIALOG_H
