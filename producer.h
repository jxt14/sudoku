#ifndef PRODUCER_H
#define PRODUCER_H

#include <cstdlib>
#include <ctime>

struct Producer{
    Producer();
    int row[11],col[11],squ[11];
    int a[11][11],b[11][11],bel[11][11],id[11][11],ter[11][11];
    int dfn[601],lowbit[601],num[601],cnt[601];
    bool pp;
    int tot,ans;
    void dfs(int,int);
    void initate();
    void produce();
    void produceAll(int);

};

#endif // PRODUCER_H
