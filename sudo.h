#ifndef SUDO_H
#define SUDO_H

#include <QRadioButton>
#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include "doku.h"
#include "action.h"
#include "producer.h"
#include "rd.h"

namespace Ui {
class sudo;
}

class sudo : public QMainWindow
{
    Q_OBJECT

public:
    explicit sudo(QWidget *parent = 0);
    ~sudo();

private slots:
    void Setposition(int);
    void Fill(int);
    void ShowAll(int);
    void Redo();
    void Undo();
    void updatetime();
    void pause();
    void Restartchoose();
    void Restart();
    void NoRestart();
private:
    Ui::sudo *ui;
    doku *Grid[9][9];
    rdoku *Dif[11];
    doku *Lit[9][9][9];
    doku *Num[10];
    QPushButton *Func[4];
    QDialog *sw;
    QPushButton *pYes;
    QPushButton *pNo;
    QLabel *Wor;
    QLabel *Lev;
    QTimer *chk;
    QLabel *tim;
    QDialog *Cong;
    QLabel *Nim;
    QLabel *Tot;
    Producer *duc;
    int px,py,m,s,pms;
    void clearstyle();
    void setclickstyle(int,int,int);
    void deal(int);
    void restyle();
    void Terminal();
    action his[1001];
    int top,lim,now;
};

#endif // SUDO_H
