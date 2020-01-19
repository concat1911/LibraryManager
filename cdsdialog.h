#ifndef CDSDIALOG_H
#define CDSDIALOG_H

#include <QDialog>
#include "median.h"
#include "databasecontroller.h"

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
    bool isEditMode = false;
    int cdID;

public:
    void EditMode();

private:
    void LoadPerson();

private slots:
    void on_pushButton_clicked();
    void onDoubleClicked(const QModelIndex &index);

    void on_borrowInput_clicked();

private:
    DatabaseController *db;
    int cdAvaiable;

private:
    Ui::CDsDialog *ui;
};

#endif // CDSDIALOG_H
