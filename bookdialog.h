#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>
#include "median.h"
#include <QtSql>

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(QWidget *parent = nullptr);
    ~BookDialog();

public:
    bool isEditMode = false;
    int bookID;

public:
    void EditMode();

private:
    void LoadPerson();

private:
    int bookAvaiable;

private slots:
    void on_pushButton_clicked();                       //Confirm button, if edit mode => update data else create new book
    void on_removeBtn_clicked();                        //Ask permission for remove data
    void on_borrowInput_clicked();                      //Check valid data then allow person take the media
    void onDoubleClicked(const QModelIndex &index);     //Ask permission that this person return media or not

private:
    Ui::BookDialog *ui;
};

#endif // BOOKDIALOG_H
