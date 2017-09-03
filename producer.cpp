#include "producer.h"

Producer::Producer()
{
    int Li,Lj,k;
    for(int i=0;i<9;i++)num[1<<i]=i+1;
    for(int i=0;i<=511;i++){
        lowbit[i]=i&(-i);
        cnt[i]=0;
        k=i;
        while(k!=0){
            cnt[i]++;
            k=k-lowbit[k];
        }
    }
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            id[i][j]=9*i+j+1;
            Li=i/3;Lj=j/3;
            bel[i][j]=3*Li+Lj;
        }
}

void Producer::initate()
{
    for(int i=0;i<9;i++){
        col[i]=511;
        row[i]=511;
        squ[i]=511;
    }
    for(int i=0;i<=100;i++)dfn[i]=0;
    tot=0;
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            if(a[i][j]==0){
                tot++;
                dfn[tot]=id[i][j];
            }
            else{
                col[i]=col[i]&(511-(1<<(a[i][j]-1)));
                row[j]=row[j]&(511-(1<<(a[i][j]-1)));
                squ[bel[i][j]]=squ[bel[i][j]]&(511-(1<<(a[i][j]-1)));
            }
        }
}

void Producer::dfs(int dep, int kd)
{
    if(dep==tot+1){
        if(kd==0){
            pp=true;
            return;
        }
        else{
            ans++;
            if(ans>1)return;
        }
    }
    int i,h,k,x,y,mk,nxt;
    x=(dfn[dep]-1)/9;
    y=(dfn[dep]-1)%9;
    k=col[x]&row[y]&squ[bel[x][y]];
    mk=cnt[k];
    if(mk==0)mk=10;
    for(i=dep+1;i<=tot;i++){
        x=(dfn[i]-1)/9;
        y=(dfn[i]-1)%9;
        k=col[x]&row[y]&squ[bel[x][y]];
        if(cnt[k]<mk&&cnt[k]!=0){
            mk=cnt[k];
            h=dfn[dep];
            dfn[dep]=dfn[i];
            dfn[i]=h;
        }
    }
    x=(dfn[dep]-1)/9;
    y=(dfn[dep]-1)%9;
    k=col[x]&row[y]&squ[bel[x][y]];
    while(k!=0){
        nxt=lowbit[k];
        a[x][y]=num[nxt];
        col[x]^=nxt;row[y]^=nxt;squ[bel[x][y]]^=nxt;
        dfs(dep+1,kd);
        if(kd==0){
            if(pp)return;
        }
        else{
            if(ans>1)return;
        }
        col[x]^=nxt;row[y]^=nxt;squ[bel[x][y]]^=nxt;
        a[x][y]=0;
        k-=lowbit[k];
    }
}

void Producer::produce()
{
    int temp[11],k,h;
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)a[i][j]=0;
    for(int i=0;i<9;i++)temp[i]=i+1;
    for(int i=1;i<=40;i++){
        k=rand()%9;
        h=temp[0];
        temp[0]=temp[k];
        temp[k]=h;
    }
    for(int i=0;i<9;i++)a[0][i]=temp[i];

    for(int i=0;i<=100;i++)dfn[i]=0;
    for(int i=0;i<=10;i++){
        col[i]=511;
        row[i]=511;
        squ[i]=511;
    }
    tot=0;
    for(int i=0;i<=2;i++)
        for(int j=0;j<9;j++){
            if(a[i][j]==0){
                tot++;
                dfn[tot]=id[i][j];
            }
            else{
                col[i]=col[i]&(511-(1<<(a[i][j]-1)));
                row[j]=row[j]&(511-(1<<(a[i][j]-1)));
                squ[bel[i][j]]=squ[bel[i][j]]&(511-(1<<(a[i][j]-1)));
            }
        }
    pp=false;
    dfs(1,0);
}

void Producer::produceAll(int limit)
{
    int x,y;
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++){
            a[i][j]=0;
            b[i][j]=0;
        }
    while(true){
        produce();
        initate();
        pp=false;
        dfs(1,0);
        if(pp==true)break;
    }
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            ter[i][j]=a[i][j];
    for(int lo=1;lo<=limit;lo++){
        for(int i=0;i<9;i++)
            for(int j=0;j<9;j++)b[i][j]=a[i][j];
        while(true){
            x=rand()%9;
            y=rand()%9;
            while(a[x][y]==0){
                x=rand()%9;
                y=rand()%9;
            }
            a[x][y]=0;
            initate();
            ans=0;
            dfs(1,1);
            for(int i=0;i<9;i++)
                for(int j=0;j<9;j++)a[i][j]=b[i][j];
            if(ans==1){
                a[x][y]=0;
                break;
            }
        }
    }
}
