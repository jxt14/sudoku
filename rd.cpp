#include "rd.h"

rdoku::rdoku(QWidget *parent):QRadioButton(parent)
{
    id=0;
}

void rdoku::Receiveclick()
{
    emit this->Sendout(this->id);
}
