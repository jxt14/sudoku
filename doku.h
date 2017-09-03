#ifndef DOKU_H
#define DOKU_H
#include <QPushButton>
#include <QWidget>

class doku : public QPushButton
{
    Q_OBJECT
public:
    doku(QWidget *parent=0);
    int num,tot,ber,id;
    bool Ena;
signals:
    void Sendout(int);
public slots:
    void Receiveclick();
};

#endif // DOKU_H
