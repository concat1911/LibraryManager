#include "cdsdialog.h"
#include "ui_cdsdialog.h"
#include "QDoubleValidator"
#include "QMessageBox"

CDsDialog::CDsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDsDialog)
{
    ui->setupUi(this);
    ui->cdQuantity->setValidator(new QIntValidator(this));
    ui->cdLength->setValidator(new QDoubleValidator(this));
    ui->cdSize->setValidator(new QDoubleValidator(this));


    ui->personTable->horizontalHeader()->sortIndicatorOrder();
    ui->personTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->personTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->personTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->personTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->personTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->personTable, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(onDoubleClicked(QModelIndex)));

    ui->removeBtn->setVisible(false);
}

CDsDialog::~CDsDialog()
{
    delete ui;
}

void CDsDialog::on_pushButton_clicked()
{  
    //Name Input is empty, not allow create new CDs
    if(ui->cdName->text().isEmpty()){
        QMessageBox::information(this, "MESSAGE", "CDs must have a name");
        return;
    }

    QString name,type,category,author,language,format;
    int quantity;
    float length, size;

    //GET DATA FROM INPUT
    name=ui->cdName->text();
    category=ui->cdCategory->currentText();
    author=ui->cdAuthor->text();
    language=ui->cdLanguage->currentText();
    format=ui->cdFormat->currentText();
    quantity=ui->cdQuantity->text().toInt();

    length=ui->cdLength->text().toFloat();
    size=ui->cdSize->text().toFloat();

    CDs *newCD = new CDs(name, category, author, language, quantity, format, length, size);
    //CHECK MODE
    if(!isEditMode){
        if(newCD->create()){
            QMessageBox::information(this, "MESSAGE", "New CDs was added into library.");
            close();
        }else{
            QMessageBox::information(this, "MESSAGE", "CDs might already exist, please check input name.");
        }
    }else{
        if(newCD->update(cdID)){
            QMessageBox::information(this, "MESSAGE", "CDs data updated.");
        }else{
            QMessageBox::information(this, "MESSAGE", "CDs might already exist, please check input name.");
        }
    }
}

void CDsDialog::EditMode(){

    QSqlQuery query;
    query.prepare("select * from media where id=?");
    query.addBindValue(cdID);
    LoadPerson();
    ui->removeBtn->setVisible(true);

    if(query.exec()){
        while (query.next()) {
            //Data Model in array
            //[0: id, 1: name, 2: type, 3: category, 4: author, 5: language, 6: ISBN, 7: format, 8: quantity, 9: avaiable, 11: length, 12: size]
            ui->cdName->setText(query.value(1).toString());
            ui->cdCategory->setCurrentText(query.value(3).toString());
            ui->cdAuthor->setText(query.value(4).toString());
            ui->cdLanguage->setCurrentText(query.value(5).toString());
            ui->cdFormat->setCurrentText(query.value(7).toString());
            ui->cdQuantity->setText(query.value(8).toString());
            ui->cdLength->setText(query.value(11).toString());
            ui->cdSize->setText(query.value(12).toString());
            cdAvaiable=query.value(9).toInt();
            ui->cdAvaiable->setText(query.value(9).toString() + " cd(s) in library.");
        }
    }else{
        qDebug() << query.lastError().text();
    }
}

void CDsDialog::LoadPerson(){
    if(!isEditMode) return;

    QSqlQueryModel *model = new QSqlQueryModel();
    QString q = "SELECT * FROM person LEFT JOIN media_borrow mb ON mb.person_id = person.id WHERE mb.media_id=";
    q.append(QString::number(cdID));
    model->setQuery(q);
    model->removeColumns(6, 2);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->personTable->setModel(proxyModel);
    ui->cdAvaiable->setText(QString::number(cdAvaiable) + " CDs in library");
}

void CDsDialog::onDoubleClicked(const QModelIndex &index){
    QModelIndexList indexList = ui->personTable->selectionModel()->selectedIndexes();

    QMessageBox::StandardButton reply;
    QString mess = "Does " + indexList[3].data().toString() + " return this CDs?";
    reply = QMessageBox::question(this, "[CONFIRM REQUIRE]", mess, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM media_borrow WHERE person_id=? and media_id=?");
        query.addBindValue(indexList[0].data());
        query.addBindValue(cdID);

        if(query.exec()){
            QMessageBox::information(this, "MESSAGE", "CDs was returned!");
            cdAvaiable++;
            query.prepare("UPDATE media SET avaiable=? WHERE id=?");
            query.addBindValue(cdAvaiable);
            query.addBindValue(cdID);
            query.exec();

            LoadPerson();
        }
    }
}

void CDsDialog::on_borrowInput_clicked()
{
    if(!isEditMode) return;

    if(cdAvaiable <= 0){
        QMessageBox::information(this, "MESSAGE", "No more CDs in library.");
        return;
    }

    if(ui->personInput->text().isEmpty()){
        QMessageBox::information(this, "MESSAGE", "Please enter valid person email.");
        return;
    }

    QString personEmail = ui->personInput->text();

    QSqlQuery query;
    query.prepare("SELECT (id) FROM person WHERE Email=?");
    query.addBindValue(personEmail);

    if(query.exec() && query.next()){
        int personId = query.value(0).toInt();
        //Check if this person already borrow the CD or not
        query.prepare("SELECT * FROM media_borrow WHERE person_id=? AND media_id=?");
        query.addBindValue(personId);
        query.addBindValue(cdID);

        if(query.exec() && query.next()){
            QMessageBox::information(this, "MESSAGE", "This person already borrow this CD.");
            ui->personInput->setText("");
        }else{
            query.prepare("INSERT INTO media_borrow (person_id, media_id) VALUES (?, ?)");
            query.addBindValue(personId);
            query.addBindValue(cdID);

            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "Updated, this person can take the CD.");
                cdAvaiable--;
                query.prepare("UPDATE media SET avaiable=? WHERE id=?");
                query.addBindValue(cdAvaiable);
                query.addBindValue(cdID);
                query.exec();

                ui->personInput->setText("");
                ui->cdAvaiable->setText(QString::number(cdAvaiable) + " CDs in library");
                LoadPerson();
            }
        }
    }else{
        QMessageBox::information(this, "MESSAGE", "Not found this person with that email, please try again.");
        ui->personInput->setText("");
    }
}

void CDsDialog::on_removeBtn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "[CONFIRM PLEASE]", "Do you want to remove this cd from library?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM media WHERE id=?");
        query.addBindValue(cdID);

        if(query.exec()){
            query.prepare("DELETE FROM media_borrow WHERE media_id=?");
            query.addBindValue(cdID);
            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "CDs was removed!");
                close();
            }
        }
    }
}
