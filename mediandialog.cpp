#include "mediandialog.h"
#include "ui_mediandialog.h"

MedianDialog::MedianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianDialog)
{
    ui->setupUi(this);
}

MedianDialog::~MedianDialog()
{
    delete ui;
}
