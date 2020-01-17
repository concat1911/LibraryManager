#ifndef MEDIENDIALOG_H
#define MEDIENDIALOG_H

#include <QDialog>

namespace Ui {
class MedienDialog;
}

class MedienDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedienDialog(QWidget *parent = nullptr);
    ~MedienDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MedienDialog *ui;
};

#endif // MEDIENDIALOG_H
