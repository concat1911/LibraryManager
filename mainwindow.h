#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QString>

//LIBRARY
#include "bookdialog.h"
#include "cdsdialog.h"
#include "persondialog.h"

#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QStandardPaths>

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
    void on_addPersonBtn_clicked();
    void onMediaDoubleClicked(const QModelIndex &index);
    void onPersonDoubleClicked(const QModelIndex &index);
    void on_searchBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    BookDialog *bookDialog;
    CDsDialog *cdsDialog;
    PersonDialog *personDialog;

private:
    void LoadBooks(QString query);
    void LoadPerson(QString q);
};
#endif // MAINWINDOW_H
