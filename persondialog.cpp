#include "persondialog.h"
#include "ui_persondialog.h"

PersonDialog::PersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonDialog)
{
    ui->setupUi(this);
}

PersonDialog::~PersonDialog()
{
    delete ui;
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

    if(newPerson->create()){
        SetMessage("New CD was added");
    }else{
        SetMessage("[FAIL] Please check input, CD name must be unique.");
    }
}

void PersonDialog::SetMessage(QString newMess){
    ui->message->setText(newMess);
}
