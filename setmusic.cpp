#include "setmusic.h"
#include "ui_setmusic.h"

setmusic::setmusic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setmusic)
{
    ui->setupUi(this);
}

setmusic::~setmusic()
{
    delete ui;
}
