/*
 * Floyd算法
 */
#include <cstdio>
#include <algorithm>
#include "gavl/gavl.h"
using namespace std;
#define Nmax 30
#define INF 100000
int N, M;
int mp[Nmax][Nmax];
int main()
{
    freopen("example2.in", "r", stdin);
    freopen("example2.out", "w", stdout);
    VGraph G;
    G.setFileName("example2.html");
    scanf("%d %d", &N, &M);
    G.addNodes(1, N);

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            if (i != j)
            {
                mp[i][j] = INF;
                G.addEdge(i, j, INF);
            }
            else
            {
                G.addEdge(i, j, 0);
            }
        }
    }
    int u, v, c;
    for (int i = 0; i < M; i++)
    {
        scanf("%d %d %d", &u, &v, &c);
        mp[u][v] = c;
        mp[v][u] = c;
        G.setWeightEdge(u, v, c);
    }
    G.init();
    for (int k = 1; k <= N; k++)
    {
        G.pointOutNode(k, 1);
        for (int i = 1; i <= N; i++)
        {
            if (i == k)
            {
                continue;
            }
            G.setCheckBlock(1);
            G.pointOutNode(i, 2);
            G.pointOutEdge(k, i, 1);
            for (int j = 1; j <= N; j++)
            {
                if (j == k || j == i)
                {
                    continue;
                }
                G.setCheckBlock(1);
                G.pointOutNode(j, 3);
                G.pointOutEdge(j, k, 2);
                G.pointOutEdge(i, j, 4);
                mp[i][j] = min(mp[i][j], mp[i][k] + mp[k][j]);
                G.setWeightEdge(i, j, mp[i][j]);
            }
        }
    }
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            printf("%d ", mp[i][j]);
        }
        printf("\n");
    }
    return 0;
}