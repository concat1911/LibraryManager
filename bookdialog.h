#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>
#include "median.h"
#include "databasecontroller.h"

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
    DatabaseController *db;

private slots:
    void on_pushButton_clicked();

    void on_removeBtn_clicked();

private:
    Ui::BookDialog *ui;
    void SetMessage(QString newMess);
};

#endif // BOOKDIALOG_H
