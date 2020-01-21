#ifndef CDSDIALOG_H
#define CDSDIALOG_H

#include <QDialog>
#include "median.h"
#include <QtSql>

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
    void on_pushButton_clicked();                       //Confirm button, if edit mode => update data else create new book
    void on_borrowInput_clicked();                      //Ask permission for remove data
    void on_removeBtn_clicked();                        //Check valid data then allow person take the media
    void onDoubleClicked(const QModelIndex &index);     //Ask permission that this person return media or not

private:
    int cdAvaiable;

private:
    Ui::CDsDialog *ui;
};

#endif // CDSDIALOG_H
