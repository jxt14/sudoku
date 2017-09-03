#ifndef RD_H
#define RD_H

#include <QRadioButton>
#include <QWidget>

class rdoku : public QRadioButton{
    Q_OBJECT
public:
    rdoku(QWidget *parent = 0);
    int id;
signals:
    void Sendout(int);
private slots:
    void Receiveclick();

};

#endif // RD_H
