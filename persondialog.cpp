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
    if(ui->firstName->text().isEmpty() || ui->email->text().isEmpty()){
        QMessageBox::information(this, "MESSAGE", "First name and Email required, please check inputs.");
        return;
    }
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
    QString mess = "Does " + ui->email->text() + " return " + indexList[1].data().toString() + " ?";
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

void PersonDialog::on_removeBtn_clicked()
{
    QAbstractItemModel* tableModel= ui->mediaTable->model();
    if(tableModel->rowCount() != 0){
        QMessageBox::information(this, "MESSAGE", "All media should be returned before remove this Person Data.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "[CONFIRM PLEASE]", "Do you want to remove person from library?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM person WHERE id=?");
        query.addBindValue(personID);

        if(query.exec()){
            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "Person was removed!");
                close();
            }
        }
    }
}

void PersonDialog::on_borrowBtn_clicked()
{
    if(!isEditMode) return;

    if(ui->mediaInput->text().isEmpty()){
        QMessageBox::information(this, "MESSAGE", "Please enter valid media name.");
        return;
    }

    QString mediaName = ui->mediaInput->text();

    QSqlQuery query;
    query.prepare("SELECT id, avaiable FROM media WHERE name=?");
    query.addBindValue(mediaName);

    if(query.exec() && query.next()){
        int mediaId = query.value(0).toInt();
        int mediaAvaiable = query.value(1).toInt();

        //Check media is avaiable to borrow
        if(mediaAvaiable <= 0){
            QMessageBox::information(this, "MESSAGE", "No more in library, can't borrow right now.");
            ui->mediaInput->setText("");
            return;
        }

        //Check if this person already borrow the media or not
        query.prepare("SELECT * FROM media_borrow WHERE person_id=? AND media_id=?");
        query.addBindValue(personID);
        query.addBindValue(mediaId);

        if(query.exec() && query.next()){
            QMessageBox::information(this, "MESSAGE", "This person already borrow this media.");
            ui->mediaInput->setText("");
        }else{
            query.prepare("INSERT INTO media_borrow (person_id, media_id) VALUES (?, ?)");
            query.addBindValue(personID);
            query.addBindValue(mediaId);

            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "Updated, this person can take the book.");
                query.prepare("UPDATE media SET avaiable=avaiable-1 WHERE id=?");
                query.addBindValue(mediaId);
                query.exec();

                ui->mediaInput->setText("");
                LoadMedia();
            }
        }
    }else{
        QMessageBox::information(this, "MESSAGE", "Media not found, please check name input.");
        ui->mediaInput->setText("");
    }
}
