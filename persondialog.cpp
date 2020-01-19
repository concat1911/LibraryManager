#include "persondialog.h"
#include "ui_persondialog.h"
#include "QMessageBox"

PersonDialog::PersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonDialog)
{
    ui->setupUi(this);
    ui->mediaTable->horizontalHeader()->sortIndicatorOrder();
    ui->mediaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->mediaTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->mediaTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->mediaTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mediaTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->mediaTable, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(onDoubleClicked(QModelIndex)));

    ui->removeBtn->setVisible(false);
}

PersonDialog::~PersonDialog()
{
    delete ui;
}

void PersonDialog::EditMode(){
    db = new DatabaseController();
    db->ConnectDB();

    QSqlQuery query;
    query.prepare("select * from person where id=?");
    query.addBindValue(personID);
    LoadMedia();
    ui->removeBtn->setVisible(true);

    if(query.exec()){
        while (query.next()) {
            //Data Model in array
            //[0: id, 1: First Name, 2: Last Name, 3: Email, 4: Contact, 5: Type]
            ui->firstName->setText(query.value(1).toString());
            ui->lastName->setText(query.value(2).toString());
            ui->email->setText(query.value(3).toString());
            ui->contact->setText(query.value(4).toString());
            ui->type->setCurrentText(query.value(5).toString());
        }
    }else{
        qDebug() << query.lastError().text();
    }
}

void PersonDialog::on_pushButton_clicked()
{
    QString firstName, lastName, email, contact, type;

    //GET DATA FROM INPUT
    firstName=ui->firstName->text();
    lastName=ui->lastName->text();
    email=ui->email->text();
    contact=ui->contact->text();
    type=ui->type->currentText();

    Person *newPerson = new Person(firstName, lastName, email, contact, type);

    if(!isEditMode){
        if(newPerson->create()){
            QMessageBox::information(this, "MESSAGE", "New person was added.");
            close();
        }else{
            QMessageBox::information(this, "MESSAGE", "Email must be unique, please check input.");
        }
    }else{
        if(newPerson->update(personID)){
            QMessageBox::information(this, "MESSAGE", "Person data was saved.");
        }else{
            QMessageBox::information(this, "MESSAGE", "Email must be unique, please check input.");
        }
    }
}

void PersonDialog::onDoubleClicked(const QModelIndex &index){
    QModelIndexList indexList = ui->mediaTable->selectionModel()->selectedIndexes();

    QMessageBox::StandardButton reply;
    QString mess = "Does " + ui->email->text() + " return this " + indexList[1].data().toString() + " ?";
    reply = QMessageBox::question(this, "[CONFIRM REQUIRE]", mess, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM media_borrow WHERE person_id=? and media_id=?");
        query.addBindValue(personID);
        query.addBindValue(indexList[0].data());

        if(query.exec()){
            QMessageBox::information(this, "MESSAGE", "Media was returned!");
            LoadMedia();
            query.prepare("UPDATE media SET avaiable=avaiable+1 WHERE id=?");
            query.addBindValue(indexList[0].data());
            query.exec();
        }
    }
}

void PersonDialog::LoadMedia(){
    if(!isEditMode) return;

    QSqlQueryModel *model = new QSqlQueryModel();
    QString q = "SELECT id, name, type, category, ISBN, quantity, avaiable FROM media LEFT JOIN media_borrow mb ON mb.media_id = media.id WHERE mb.person_id=";
    q.append(QString::number(personID));
    model->setQuery(q);
    model->removeColumns(7, 2);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->mediaTable->setModel(proxyModel);
}
