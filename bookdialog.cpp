#include "bookdialog.h"
#include "ui_bookdialog.h"
#include "QtDebug"
#include "QString"
#include "QMessageBox"
#include <QIntValidator>

BookDialog::BookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{
    ui->setupUi(this);
    ui->bookQuantity->setValidator(new QIntValidator(this));
    ui->bookPage->setValidator(new QIntValidator(this));

    ui->personTable->horizontalHeader()->sortIndicatorOrder();
    ui->personTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->personTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->personTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->personTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->personTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->personTable, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(onDoubleClicked(QModelIndex)));

    ui->removeBtn->setVisible(false);
}

BookDialog::~BookDialog()
{
    delete ui;
}

void BookDialog::on_pushButton_clicked()
{
    //Name Input is empty, not allow create new Book
    if(ui->bookName->text().isEmpty()){
        QMessageBox::information(this, "MESSAGE", "Book must have a name");
        return;
    }

    QString name,type,category,author,language,format,ISBN;
    int quantity, page;
    bool hasDigital;

    //GET DATA FROM INPUT
    name=ui->bookName->text();
    category=ui->bookCategory->currentText();
    author=ui->bookAuthor->text();
    language=ui->bookLanguage->currentText();
    format=ui->bookFormat->currentText();
    ISBN=ui->bookISBN->text();

    quantity=ui->bookQuantity->text().toInt();
    page=ui->bookPage->text().toInt();
    hasDigital=ui->hasDigital->isChecked();

    Book *newBook = new Book(page, hasDigital, ISBN);
    newBook->SetBase(name, category, author, language, quantity, format);
    //Check Book Dialog mode
    //if false, create new book else update book data
    if(!isEditMode){
        if(newBook->create()){
            QMessageBox::information(this, "MESSAGE", "New book was created!");
            close();
        }else{
            QMessageBox::information(this, "MESSAGE", "Book might already exist, please check name input.");
        }
    }else{
        newBook->avaiable = bookAvaiable;
        if(newBook->update(bookID)){
            QMessageBox::information(this, "MESSAGE", "Data Updated");
        }else{
            QMessageBox::information(this, "MESSAGE", "Book might already exist, please check name input.");
        }
    }
}

void BookDialog::EditMode(){
    //qDebug() << bookID << isEditMode;

    QSqlQuery query;
    query.prepare("select * from media where id=?");
    query.addBindValue(bookID);
    LoadPerson();
    ui->removeBtn->setVisible(true);

    if(query.exec()){
        while (query.next()) {
            //Data Model in array
            //[0: id, 1: name, 2: type, 3: category, 4: author, 5: language, 6: ISBN, 7: format, 8: quantity, 9: avaiable, 10: page]
            ui->bookName->setText(query.value(1).toString());
            ui->bookCategory->setCurrentText(query.value(3).toString());
            ui->bookAuthor->setText(query.value(4).toString());
            ui->bookLanguage->setCurrentText(query.value(5).toString());
            ui->bookISBN->setText(query.value(6).toString());
            ui->bookFormat->setCurrentText(query.value(7).toString());
            ui->bookQuantity->setText(query.value(8).toString());
            ui->bookPage->setText(query.value(10).toString());
            bookAvaiable=query.value(9).toInt();
            ui->bookAvaiable->setText(query.value(9).toString() + " books in library.");
        }
    }else{
        qDebug() << query.lastError().text();
    }
}

void BookDialog::on_removeBtn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "[CONFIRM PLEASE]", "Do you want to remove this book from library?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        QSqlQuery query;
        query.prepare("DELETE FROM media WHERE id=?");
        query.addBindValue(bookID);

        if(query.exec()){
            query.prepare("DELETE FROM media_borrow WHERE media_id=?");
            query.addBindValue(bookID);
            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "Book was removed!");
                close();
            }
        }
    }
}

void BookDialog::on_borrowInput_clicked()
{
    if(!isEditMode) return;

    if(bookAvaiable <= 0){
        QMessageBox::information(this, "MESSAGE", "No more book in library");
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
        //Check if this person already borrow the book or not
        query.prepare("SELECT * FROM media_borrow WHERE person_id=? AND media_id=?");
        query.addBindValue(personId);
        query.addBindValue(bookID);

        if(query.exec() && query.next()){
            QMessageBox::information(this, "MESSAGE", "This person already borrow this book.");
            ui->personInput->setText("");
        }else{
            query.prepare("INSERT INTO media_borrow (person_id, media_id) VALUES (?, ?)");
            query.addBindValue(personId);
            query.addBindValue(bookID);

            if(query.exec()){
                QMessageBox::information(this, "MESSAGE", "Updated, this person can take the book.");
                bookAvaiable--;
                query.prepare("UPDATE media SET avaiable=? WHERE id=?");
                query.addBindValue(bookAvaiable);
                query.addBindValue(bookID);
                query.exec();

                ui->personInput->setText("");
                ui->bookAvaiable->setText(QString::number(bookAvaiable) + " books in library");
                LoadPerson();
            }
        }
    }else{
        QMessageBox::information(this, "MESSAGE", "Not found this person with that email, please try again.");
        ui->personInput->setText("");
    }
}

void BookDialog::LoadPerson(){
    if(!isEditMode) return;

    QSqlQueryModel *model = new QSqlQueryModel();
    QString q = "SELECT * FROM person LEFT JOIN media_borrow mb ON mb.person_id = person.id WHERE mb.media_id=";
    q.append(QString::number(bookID));
    model->setQuery(q);
    model->removeColumns(6, 2);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(model);
    proxyModel->setSourceModel(model);
    ui->personTable->setModel(proxyModel);
    ui->bookAvaiable->setText(QString::number(bookAvaiable) + " books in library");
}


void BookDialog::onDoubleClicked(const QModelIndex &index){
    QModelIndexList indexList = ui->personTable->selectionModel()->selectedIndexes();

    QMessageBox::StandardButton reply;
    QString mess = "Did " + indexList[3].data().toString() + " return this book?";
    reply = QMessageBox::question(this, "[CONFIRM REQUIRE]", mess, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM media_borrow WHERE person_id=? and media_id=?");
        query.addBindValue(indexList[0].data());
        query.addBindValue(bookID);

        if(query.exec()){
            QMessageBox::information(this, "MESSAGE", "Book was returned!");
            bookAvaiable++;
            query.prepare("UPDATE media SET avaiable=? WHERE id=?");
            query.addBindValue(bookAvaiable);
            query.addBindValue(bookID);
            query.exec();

            LoadPerson();
        }
    }

//    indexList[2].data() == "Book"
}
