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
    void on_pushButton_clicked();
    void on_removeBtn_clicked();
    void on_borrowInput_clicked();
    void onDoubleClicked(const QModelIndex &index);

private:
    Ui::BookDialog *ui;
};

#endif // BOOKDIALOG_H
