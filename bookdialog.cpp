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

    SetMessage("Fill all data below to create new book.");
}

BookDialog::~BookDialog()
{
    delete ui;
}

void BookDialog::on_pushButton_clicked()
{
    //Name Input is empty, not allow create new Book
    if(ui->bookName->text().isEmpty()){
        SetMessage("[FAIL] Book must have a name.");
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

    //Check Book Dialog mode
    //if false, create new book else update book data
    if(!isEditMode){
        Book *newBook = new Book(name, category, author, language, quantity, format, page, hasDigital, ISBN);

        if(newBook->create()){
            QMessageBox::information(this, "MESSAGE", "New book was created!");
            close();
        }else{
            SetMessage("[FAIL] Book name might already exist, please check input.");
        }
    }else{
        Book *newBook = new Book(name, category, author, language, quantity, format, page, hasDigital, ISBN);

        if(newBook->update(bookID)){
            SetMessage("Data updated");
        }else{
            SetMessage("[FAIL] Book name might already exist, please check input.");
        }
    }
}

void BookDialog::SetMessage(QString newMess){
    ui->message->setText(newMess);
}

void BookDialog::EditMode(){
    //qDebug() << bookID << isEditMode;

    SetMessage("You can update this Book data.");

    db = new DatabaseController();
    db->ConnectDB();

    QSqlQuery query;
    query.prepare("select * from media where id=?");
    query.addBindValue(bookID);

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
        }
    }else{
        qDebug() << query.lastError().text();
        SetMessage("[Book not found] Please close this window and try again.");
    }
}

void BookDialog::on_removeBtn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "[CONFIRM PLEASE]", "Do you want to remove this book from library?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        db = new DatabaseController();
        db->ConnectDB();

        QSqlQuery query;
        query.prepare("DELETE FROM media WHERE id=?");
        query.addBindValue(bookID);

        if(query.exec()){
            QMessageBox::information(this, "MESSAGE", "Book was removed!");
            close();
        }
    }
}
