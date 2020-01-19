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
    bool isEditMode = false;
    int personID;

public:
    void EditMode();

private:
    void LoadMedia();

private slots:
    void on_pushButton_clicked();
    void on_removeBtn_clicked();
    void on_borrowBtn_clicked();
    void onDoubleClicked(const QModelIndex &index);

private:
    Ui::PersonDialog *ui;
};

#endif // PERSONDIALOG_H
