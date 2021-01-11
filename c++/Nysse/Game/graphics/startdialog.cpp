#include "startdialog.hh"
#include "ui_startdialog.h"

namespace Student {

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    connect(ui->beginButton, &QPushButton::clicked, this, &StartDialog::accept);
    connect(ui->endButton, &QPushButton::clicked, this, &StartDialog::reject);
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::on_userInput_valueChanged(int value)
{
    userInput_ = value;
    if (userInput_ < 1){
        userInput_ = 1;
    }
    else if (userInput_ > 5){
        userInput_ = 5;
    }
}

void StartDialog::accept()
{
    this->done(1);
    emit beginClicked(userInput_);
}

void StartDialog::reject()
{
    this->done(0);
    emit endClicked();
}

}
