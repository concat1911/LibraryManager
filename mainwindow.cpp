#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSortFilterProxyModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetMessage("Welcome back!");

    //UI TABLE SETTINGS
    ui->BookTable->horizontalHeader()->sortIndicatorOrder();
    ui->BookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->BookTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->BookTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->BookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->BookTable, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(onMediaDoubleClicked(QModelIndex)));

    ui->PersonTable->horizontalHeader()->sortIndicatorOrder();
    ui->PersonTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->PersonTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->PersonTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->PersonTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->PersonTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->PersonTable, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(onPersonDoubleClicked(QModelIndex)));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QString("/mediaDB.db"));

    if(!db.open()){
        QMessageBox::information(this, "MESSAGE", "Database connected fail.");
    }

    //LOAD DATA WHEN PROGRAMM START
    LoadBooks("select * from media");
    LoadPerson("select * from person");
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
    //RELOAD TABLE DATA BASE ON CURRENT TAB INDEX
    switch(ui->tabWidget->currentIndex()) {
      case 0:
        SetMessage("Book and CD refreshed!");
        LoadBooks("select * from media");
        break;
      case 1:
        SetMessage("Persons refreshed!");
        LoadPerson("select * from person");
        break;
      default:
        break;
    }
}

void MainWindow::SetMessage(QString newMess){
    ui->message->setText(newMess);
}

void MainWindow::LoadBooks(QString q){
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(q);
    query.exec();
    model->setQuery(query);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->BookTable->setModel(proxyModel);
}

void MainWindow::LoadPerson(QString q){
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(q);

    if(query.exec()){
        model->setQuery(query);
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
        proxyModel->setSourceModel(model);
        ui->PersonTable->setModel(proxyModel);
    }else{
        qDebug() << query.lastError().text();
    }
}

//OPEN DIALOG BASE ON MEDIA TYPE
void MainWindow::onMediaDoubleClicked(const QModelIndex &index){
    QModelIndexList indexList = ui->BookTable->selectionModel()->selectedIndexes();

    if(indexList[2].data() == "Book"){
        bookDialog = new BookDialog();
        bookDialog->isEditMode = true;
        bookDialog->bookID = indexList[0].data().toInt();
        bookDialog->EditMode();
        bookDialog->show();
    }

    if(indexList[2].data() == "CDs"){
        cdsDialog = new CDsDialog();
        cdsDialog->isEditMode = true;
        cdsDialog->cdID = indexList[0].data().toInt();
        cdsDialog->EditMode();
        cdsDialog->show();
    }
//    int row;
//    foreach (QModelIndex index, indexList) {
//        row = index.row();
//        qDebug() << index.data() << index.column();
//    }
}

void MainWindow::onPersonDoubleClicked(const QModelIndex &index){
    QModelIndexList indexList = ui->PersonTable->selectionModel()->selectedIndexes();

    personDialog = new PersonDialog();
    personDialog->isEditMode = true;
    personDialog->personID = indexList[0].data().toInt();
    personDialog->EditMode();
    personDialog->show();
}

void MainWindow::on_searchBtn_clicked()
{
    if(ui->searchInput->text().isEmpty()){
        return;
    }

    QString input = ui->searchInput->text();
    QString q;

    if(ui->tabWidget->currentIndex() == 0){
        SetMessage("Searching media ...!");
        q = "select * from media where name like \'%" + input + "%\' or ISBN like \'%" + input + "%\'";
        LoadBooks(q);
    }else{
        SetMessage("Searching person ...");
        q = "select * from person where Email like \'%" + input + "%\' or [First Name] like \'%" + input + "%\' or [Last Name] like \'%" + input + "%\'";
        LoadPerson(q);
    }

    ui->searchInput->setText("");
}

