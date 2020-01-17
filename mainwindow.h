#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QString>
#include <databasecontroller.h>

//LIBRARY
#include "bookdialog.h"
#include "cdsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void SetMessage(QString newMess);

private slots:
    void on_addMedianBtn_clicked();
    void on_addCDsBtn_clicked();
    void on_refreshBtn_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseController *db;
    BookDialog *bookDialog;
    CDsDialog *cdsDialog;
private:
    void LoadBooks();
};
#endif // MAINWINDOW_H
