#include "bookdialog.h"
#include "ui_bookdialog.h"
#include "QtDebug"
#include "QString"
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

    Book *newBook = new Book(name, category, author, language, quantity, format, page, hasDigital, ISBN);

    if(newBook->create()){
        SetMessage("New Book created");
    }else{
        SetMessage("[FAIL] Please check input, name and ISBN must be unique.");
    }
}

void BookDialog::SetMessage(QString newMess){
    ui->message->setText(newMess);
}
