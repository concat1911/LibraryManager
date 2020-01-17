#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetMessage("Welcome back!");

    ui->BookTable->horizontalHeader()->sortIndicatorOrder();
    ui->BookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->BookTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->BookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BookTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->PersonTable->horizontalHeader()->sortIndicatorOrder();
    ui->PersonTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->PersonTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->PersonTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->PersonTable->setSelectionMode(QAbstractItemView::SingleSelection);

    LoadBooks();
    LoadPerson();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addMedianBtn_clicked()
{
    bookDialog = new BookDialog();
    bookDialog->show();
}

void MainWindow::on_addCDsBtn_clicked()
{
    cdsDialog = new CDsDialog();
    cdsDialog->show();
}

void MainWindow::on_addPersonBtn_clicked()
{
    personDialog = new PersonDialog();
    personDialog->show();
}

void MainWindow::on_refreshBtn_clicked()
{
    switch(ui->tabWidget->currentIndex()) {
      case 0:
        SetMessage("Books refreshed!");
        LoadBooks();
        break;
      case 1:
        SetMessage("Persons refreshed!");
        LoadPerson();
        break;
      default:
        break;
    }
}

void MainWindow::SetMessage(QString newMess){
    ui->message->setText(newMess);
}

void MainWindow::LoadBooks(){
    db = new DatabaseController();
    db->ConnectDB();
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("select * from media");
    query.exec();
    model->setQuery(query);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->BookTable->setModel(proxyModel);

    db->CloseDB();
}

void MainWindow::LoadPerson(){
    db = new DatabaseController();
    db->ConnectDB();
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("select * from person");
    query.exec();
    model->setQuery(query);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->PersonTable->setModel(proxyModel);

    db->CloseDB();
}
