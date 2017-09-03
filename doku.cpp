#include "sudo.h"
#include "doku.h"

doku::doku(QWidget *parent):QPushButton(parent)
{
    Ena=false;
    num=0;
    tot=0;
    ber=0;
    id=0;
}

void doku::Receiveclick()
{
    emit this->Sendout(this->id);
}
