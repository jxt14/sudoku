#include "sudo.h"
#include "ui_sudo.h"

sudo::sudo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sudo)
{
    srand(unsigned(time(NULL)));
    ui->setupUi(this);
    int Len;
    Len=45;
    int x,y,num,Lx,Ly;
    QString Sty;
    x=40;y=60;
    px=0;
    py=0;
    pms=0;
    chk=new QTimer(this);
    chk->stop();
    tim=new QLabel(this);
    m=s=0;
    tim->setGeometry(300,28,80,30);
    Sty="";
    Sty+="font: 75 14pt 'Calibri';";
    tim->setStyleSheet(Sty);
    Sty="";
    tim->setText("00:00");
    tim->hide();
    ui->clk->hide();
    QObject::connect(chk,SIGNAL(timeout()),this,SLOT(updatetime()));

//9*9 sudoku StyleSheet
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            Sty="";
            Grid[i][j]=new doku(this);
            Grid[i][j]->setGeometry(x+i*Len,y+j*Len,Len,Len);
            Grid[i][j]->setText("");
            Sty+="font: 75 22pt 'Aharoni';";
            Sty+="background-color: rgb(255,255,200);";
            Sty+="border:1px solid yellow;";
            if(j%3==0)Sty+="border-top:3px solid green;";
            if(j==8)Sty+="border-bottom:3px solid green;";
            if(i%3==0)Sty+="border-left:3px solid green;";
            if(i==8)Sty+="border-right:3px solid green;";
            Grid[i][j]->setStyleSheet(Sty);
            Grid[i][j]->hide();
            num=i*9+j+1;
            Grid[i][j]->id=num;
            for(int k=0;k<9;k++){
                Lit[i][j][k]=new doku(this);
                Lit[i][j][k]->setText("");
                Lx=k%3;
                Ly=k/3;
                Lit[i][j][k]->setGeometry(x+i*Len+Lx*15,y+j*Len+Ly*15,15,15);
                Lit[i][j][k]->hide();
                Lit[i][j][k]->id=num;
                QObject::connect(Lit[i][j][k],SIGNAL(clicked()),Lit[i][j][k],SLOT(Receiveclick()));
                QObject::connect(Lit[i][j][k],SIGNAL(Sendout(int)),this,SLOT(Setposition(int)));
            }
            QObject::connect(Grid[i][j],SIGNAL(clicked()),Grid[i][j],SLOT(Receiveclick()));
            QObject::connect(Grid[i][j],SIGNAL(Sendout(int)),this,SLOT(Setposition(int)));
        }

//1~9,Delete,Redo,Undo Button
    x=22;y=474;
    for(int i=0;i<10;i++){
        Num[i]=new doku(this);
        Num[i]->id=i+1;
        Num[i]->setGeometry(x+i*45,y,40,40);
        Sty="";
        if(i<9)Sty+="font: 75 22pt 'Aharoni';";
        else Sty+="font: 75 14pt 'Aharoni';";
        Sty+="background-color: rgb(255,255,200);";
        Sty+="border:1px solid yellow;";
        Num[i]->setStyleSheet(Sty);
        Sty="%1";
        if(i<9)Num[i]->setText(Sty.arg(i+1));
        else Num[i]->setText("DEL");
        Num[i]->hide();
        QObject::connect(Num[i],SIGNAL(clicked()),Num[i],SLOT(Receiveclick()));
        QObject::connect(Num[i],SIGNAL(Sendout(int)),this,SLOT(Fill(int)));
    }
    Sty="";
    Sty+="font: 75 14pt '华文新魏';";
    Sty+="background-color: rgb(255,255,200);";
    Sty+="border:1px solid yellow;";
    for(int i=0;i<5;i++){
        Func[i]=new QPushButton(this);
        Func[i]->setGeometry(22+45*i,520,40,40);
        Func[i]->setStyleSheet(Sty);
        Func[i]->hide();
    }
    Func[0]->setText(tr("&撤销"));
    Func[1]->setText(tr("&恢复"));
    Func[2]->setText(tr("&重玩"));
    Func[3]->setText(tr("&暂停"));
    Func[4]->setText(tr("&答案"));
    QObject::connect(Func[0],SIGNAL(clicked()),this,SLOT(Redo()));
    QObject::connect(Func[1],SIGNAL(clicked()),this,SLOT(Undo()));
    QObject::connect(Func[2],SIGNAL(clicked()),this,SLOT(Restartchoose()));
    QObject::connect(Func[3],SIGNAL(clicked()),this,SLOT(pause()));
   // QObject::connect(Func[4],SIGNAL(clicked()),this,SLOT(GetAnswer()));

    Sty="border:1px solid yellow;background-color:rgb(255,255,200);";
    Cong=new QDialog(this);
    Cong->setStyleSheet(Sty);
    Nim=new QLabel(Cong);
    Nim->setGeometry(20,30,190,40);
    Tot=new QLabel(Cong);
    Tot->setGeometry(20,70,190,40);
    Nim->setStyleSheet("font:75 14pt 'Aharoni';");
    Tot->setStyleSheet("font:75 14pt 'Aharoni';");
    Nim->setText("Congratulaions!");
    Cong->hide();
    sw=new QDialog(this);
    sw->setStyleSheet(Sty);
    Wor=new QLabel(sw);
    Wor->setText(tr("确定要重玩吗?"));
    Wor->setStyleSheet("border:none;");
    Wor->setGeometry(40,20,130,30);
    pYes=new QPushButton(sw);
    pNo=new QPushButton(sw);
    pYes->setGeometry(20,60,75,25);
    pNo->setGeometry(110,60,75,25);
    Sty="border:none;background-color:rgb(222,255,150);";
    pYes->setStyleSheet(Sty);
    pNo->setStyleSheet(Sty);
    pYes->setText(tr("&是"));
    pNo->setText(tr("&否"));
    sw->hide();

    QObject::connect(pYes,SIGNAL(clicked()),this,SLOT(Restart()));
    QObject::connect(pNo,SIGNAL(clicked()),this,SLOT(NoRestart()));

//Select the magnitude of difficulty
    duc=new Producer();
    for(int i=0;i<10;i++){
        Dif[i]=new rdoku(this);
        Dif[i]->id=i;
        ui->vout->addWidget(Dif[i]);
        Sty="%1";
        Dif[i]->setText(Sty.arg(i+1));
        QObject::connect(Dif[i],SIGNAL(clicked()),Dif[i],SLOT(Receiveclick()));
        QObject::connect(Dif[i],SIGNAL(Sendout(int)),this,SLOT(ShowAll(int)));
    }
    ui->Select->show();
    Lev=new QLabel(this);
    Lev->setGeometry(80,28,90,30);
    Lev->setStyleSheet("font:75 16pt 'Calibri';");
    Lev->hide();

};

void sudo::Restartchoose()
{
    qDebug("5");
    sw->show();
    sw->raise();
}

void sudo::NoRestart()
{
    sw->hide();
}

void sudo::Restart()
{
    int k;
    QString sty;
    sw->hide();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            Grid[i][j]->show();
            for(k=0;k<9;k++)Lit[i][j][k]->hide();
        }
    }
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            k=duc->a[i][j];
            if(k==0){
                Grid[i][j]->setText("");
                Grid[i][j]->Ena=true;
                Grid[i][j]->num=0;
                Grid[i][j]->tot=0;
                Grid[i][j]->ber=0;
            }else{
                sty="%1";
                Grid[i][j]->setText(sty.arg(k));
                Grid[i][j]->Ena=false;
            }
            if(k!=0){
                Grid[i][j]->num=(1<<(k-1));
                Grid[i][j]->tot=1;
                Grid[i][j]->ber=k;
            }
        }
    clearstyle();
    top=0;
    lim=0;
    now=0;
    m=s=0;
}

//produce a puzzle of sudoku.
void sudo::ShowAll(int limi)
{
    qDebug("1");
    ui->Select->hide();
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            Grid[i][j]->show();
    for(int i=0;i<10;i++)Num[i]->show();
    for(int i=0;i<5;i++)Func[i]->show();
    tim->show();
    ui->clk->show();
    sw->hide();

    chk->start(1000);

    duc->produceAll(25+3*limi);

    QString sty;
    sty="LEVEL %1";
    Lev->setText(sty.arg(limi+1));
    Lev->show();
    int k;
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            k=duc->a[i][j];
            if(k==0){
                Grid[i][j]->setText("");
                Grid[i][j]->Ena=true;
                Grid[i][j]->tot=0;
                Grid[i][j]->num=0;
                Grid[i][j]->ber=0;
            }else{
                sty="%1";
                Grid[i][j]->setText(sty.arg(k));
                Grid[i][j]->Ena=false;
                Grid[i][j]->num=(1<<(k-1));
                Grid[i][j]->tot=1;
                Grid[i][j]->ber=k;
            }
        }
    top=now=0;
    m=s=0;
    lim=0;
    clearstyle();
}

void sudo::setclickstyle(int x,int y,int col)
{
    QString sty,st2;
    sty="";
    if(Grid[x][y]->tot<=1){
        sty+="font:75 22pt 'Aharoni';";
        sty+="border:1px solid yellow;";
        if(y%3==0)sty+="border-top:3px solid green;";
        if(y==8)sty+="border-bottom:3px solid green;";
        if(x%3==0)sty+="border-left:3px solid green;";
        if(x==8)sty+="border-right:3px solid green;";
        if(Grid[x][y]->Ena==true)sty+="color:blue;";
        else sty+="color:black;";
        if(col==0)sty+="background-color:rgb(255,255,200);";
        if(col==1)sty+="background-color:rgb(222,255,200);";
        if(col==2)sty+="background-color:rgb(255,255,150);";
        Grid[x][y]->setStyleSheet(sty);
    }
    else{
        sty+="font:75 13pt 'Aharoni';";
        sty+="color:blue;";
        if(col==0){
            sty+="background-color:rgb(255,255,200);";
            sty+="border:1px none rgb(255,255,200);";
        }
        if(col==1){
            sty+="background-color:rgb(222,255,200);";
            sty+="border:1px none rgb(222,255,200);";
        }
        if(col==2){
            sty+="background-color:rgb(255,255,150);";
            sty+="border:1px none rgb(255,255,150);";
        }
        for(int z=0;z<9;z++){
            st2=sty;
            if(z%3==0){
                if(x%3==0)st2+="border-left:3px solid green;";
                else st2+="border-left:1px solid yellow;";
            }
            if(z%3==2){
                if(x==8)st2+="border-right:3px solid green;";
                else st2+="border-right:1px solid yellow;";
            }
            if(z/3==0){
                if(y%3==0)st2+="border-top:3px solid green;";
                else st2+="border-top:1px solid yellow;";
            }
            if(z/3==2){
                if(y==8)st2+="border-bottom:3px solid green;";
                else st2+="border-bottom:1px solid yellow;";
            }
            Lit[x][y][z]->setStyleSheet(st2);
        }
    }
}

void sudo::clearstyle()
{
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)setclickstyle(i,j,0);
}

void sudo::restyle()
{
    clearstyle();
    for(int i=0;i<9;i++){
        setclickstyle(px-1,i,1);
        setclickstyle(i,py-1,1);
    }
    if(Grid[px-1][py-1]->tot==1){
        for(int i=0;i<9;i++)
            for(int j=0;j<9;j++){
                if(Grid[i][j]->ber==Grid[px-1][py-1]->ber)setclickstyle(i,j,2);
            }
    }
}

//Click the position and highlight the row and column it's in
void sudo::Setposition(int num)
{
    int x,y;
    if(num%9==0){
        x=num/9;
        y=9;
    }
    else{
        x=(num/9)+1;
        y=num%9;
    }
    px=x;
    py=y;
    restyle();
}

void sudo::Terminal()
{
    QString sty;
    sty="Total time: %1 min %2 s";
    Tot->setText(sty.arg(m).arg(s));
    ui->Select->show();
    Cong->show();
    Cong->raise();
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            Grid[i][j]->hide();
            for(int k=0;k<9;k++)Lit[i][j][k]->hide();
        }
    for(int i=0;i<10;i++)Num[i]->hide();
    for(int i=0;i<4;i++)Func[i]->hide();
    chk->stop();
    ui->clk->hide();
    tim->hide();
    Lev->hide();
    for(int i=0;i<10;i++)Dif[i]->setDown(false);
}

//Fill in the blanks with the numbers
void sudo::Fill(int num)
{
    bool pcheck;
    int chn,cn,nth;
    nth=num;
    cn=0;
    if(px==0||py==0)return;
    if(Grid[px-1][py-1]->Ena==false)return;
    if(nth<=9)chn=(Grid[px-1][py-1]->num)^(1<<(nth-1));
    else{
        cn=Grid[px-1][py-1]->num;
        chn=0;
    }
    deal(chn);
    pcheck=true;
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)if(Grid[i][j]->ber!=duc->ter[i][j])pcheck=false;
    if(pcheck==true)Terminal();
    lim=0;
    now++;
    his[now].x=px;
    his[now].y=py;
    his[now].btn=nth;
    his[now].num=cn;
    top=now;
}

void sudo::deal(int num)
{
    QString sty;
    int x,y,l,k;
    x=px-1;y=py-1;
    Grid[x][y]->num=num;
    Grid[x][y]->ber=0;
    l=0;
    Grid[x][y]->tot=0;
    for(int i=0;i<9;i++){
        if(((Grid[x][y]->num)&(1<<i))!=0){
            Grid[x][y]->tot++;
            l=i;
        }
    }
    l++;
    if(Grid[x][y]->tot==1)Grid[x][y]->ber=l;
    if(Grid[x][y]->tot<=1){
        for(int i=0;i<9;i++)Lit[x][y][i]->hide();
        Grid[x][y]->show();
        if(Grid[x][y]->tot==0)Grid[x][y]->setText("");
        else{
            sty="%1";
            Grid[x][y]->setText(sty.arg(l));
        }
    }
    else{
        for(int i=0;i<9;i++){
            k=(Grid[x][y]->num)&(1<<i);
            sty="%1";
            if(k!=0)Lit[x][y][i]->setText(sty.arg(i+1));
            else Lit[x][y][i]->setText("");
        }
        Grid[x][y]->hide();
        for(int i=0;i<9;i++)Lit[x][y][i]->show();
    }
    restyle();
}

void sudo::Redo()
{
    int chn;
    if(lim==10||now==0)return;
    lim++;
    px=his[now].x;
    py=his[now].y;
    if(his[now].btn<=9)chn=(Grid[px-1][py-1]->num)^(1<<(his[now].btn-1));
    else chn=his[now].num;
    now--;
    deal(chn);
}

void sudo::Undo()
{
    int chn;
    if(now==top)return;
    now++;
    px=his[now].x;
    py=his[now].y;
    lim--;
    if(his[now].btn<=9)chn=(Grid[px-1][py-1]->num)^(1<<(his[now].btn-1));
    else chn=0;
    deal(chn);
}

void sudo::updatetime()
{
    QString sty;
    s++;
    if(s==60){
        s=0;
        m++;
    }
    if(m==0&&s<10){
        sty="00:0%1";
        tim->setText(sty.arg(s));
    }
    else if(m==0&&s>=10){
        sty="00:%1";
        tim->setText(sty.arg(s));
    }
    else if(m<10&&s<10){
        sty="0%1:0%2";
        tim->setText(sty.arg(m).arg(s));
    }
    else if(m<10&&s>=10){
        sty="0%1:%2";
        tim->setText(sty.arg(m).arg(s));
    }
    else if(s<10){
        sty="%1:0%2";
        tim->setText(sty.arg(m).arg(s));
    }
    else{
        sty="%1:%2";
        tim->setText(sty.arg(m).arg(s));
    }
}

void sudo::pause()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(Grid[i][j]->tot<=1){
                if(pms==0)Grid[i][j]->hide();
                else Grid[i][j]->show();
            }
            else{
                for(int k=0;k<9;k++){
                    if(pms==0)Lit[i][j][k]->hide();
                    else Lit[i][j][k]->show();
                }
            }
        }
    }
    for(int i=0;i<10;i++){
        if(pms==0)Num[i]->hide();
        else Num[i]->show();
        if(i<5&&i!=3){
            if(pms==0)Func[i]->hide();
            else Func[i]->show();
        }
    }
    if(pms==0){
        Func[3]->setText(tr("&开始"));
        chk->stop();
    }
    else{
        Func[3]->setText(tr("暂停"));
        chk->start(1000);
        sw->hide();
    }
    pms=pms^1;
}

sudo::~sudo()
{
    delete ui;
}
