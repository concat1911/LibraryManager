#include "cdsdialog.h"
#include "ui_cdsdialog.h"
#include "QDoubleValidator"

CDsDialog::CDsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDsDialog)
{
    ui->setupUi(this);
    ui->cdQuantity->setValidator(new QIntValidator(this));
    ui->cdLength->setValidator(new QDoubleValidator(this));
    ui->cdSize->setValidator(new QDoubleValidator(this));
}

CDsDialog::~CDsDialog()
{
    delete ui;
}

void CDsDialog::on_pushButton_clicked()
{
    //Name Input is empty, not allow create new Book
    if(ui->cdName->text().isEmpty()){
        SetMessage("[FAIL] CDs must have a name.");
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

    if(newCD->create()){
        SetMessage("New CDs was added.");
    }else{
        SetMessage("[FAIL] Please check input, CD name must be unique.");
    }
}

void CDsDialog::SetMessage(QString newMess){
    ui->message->setText(newMess);
}
