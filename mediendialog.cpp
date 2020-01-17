#include "mediendialog.h"
#include "ui_mediendialog.h"

MedienDialog::MedienDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedienDialog)
{
    ui->setupUi(this);
}

MedienDialog::~MedienDialog()
{
    delete ui;
}

void MedienDialog::on_pushButton_clicked()
{
    QString name,type,author,language,format,ISBN;
    int quantity, page;
    bool hasDigital;

    //GET DATA FROM INPUT
    name=ui->bookName->text();
    type=ui->bookType->currentText();
    author=ui->bookAuthor->text();
    language=ui->bookLanguage->currentText();
    format=ui->bookFormat->currentText();
    ISBN=ui->bookISBN->text();

    quantity=ui->bookQuantity->text().toInt();
    page=ui->bookPage->text().toInt();
    hasDigital=ui->hasDigital->isChecked();

    Book *newBook = new Book(name, type, author, language, quantity, format, page, hasDigital, ISBN);

    if(newBook->create(ui->message)){
        SetMessage("New Book created");
    }
}

void MedienDialog::SetMessage(QString newMess){
    ui->message->setText(newMess);
}
