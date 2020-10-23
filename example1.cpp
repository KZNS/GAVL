/*
 *  堆优化的Dijkstra算法
 */
#include <cstdio>
#include <vector>
#include <queue>
#include "gavl/gavl.h"
using namespace std;
#define Nmax 100
int N, M;
vector<pair<int, int>> gp[Nmax];
bool used[Nmax];
int dis[Nmax];
int main()
{
    freopen("example1.in", "r", stdin);
    freopen("example1.out", "w", stdout);
    VGraph G;                       /* 实例化一个可视化图  */
    G.setFileName("example1.html"); /* 设置可视化文件名 */

    scanf("%d %d", &N, &M);
    G.addNodes(1, N); /* 添加结点 1~N */
    int u, v, c;
    for (int i = 0; i < M; i++)
    {
        scanf("%d %d %d", &u, &v, &c);
        gp[u].push_back(make_pair(v, c));
        gp[v].push_back(make_pair(u, c));
        G.addEdge(u, v, c); /* 添加边 u -- v, 边权为 c */
    }
    G.init(); /* 初始化 */
    priority_queue<pair<int, int>> pls;
    pls.push(make_pair(0, 1));
    pair<int, int> pru;
    int x;
    while (!pls.empty())
    {
        pru = pls.top();
        pls.pop();
        c = -pru.first;
        x = pru.second;
        if (used[x])
            continue;
        G.pointOutNode(x, 1); /* 高亮结点 x，层级为 1 */
        used[x] = true;
        dis[x] = c;
        G.setInfoNode(x, "dis", c); /* 设置结点 x 的 dis 属性值为 c */
        for (int i = 0; i < gp[x].size(); i++)
        {
            G.pointOutNode(gp[x][i].first, 2); /* 高亮结点 gp[x][i].first，层级为 2 */
            pls.push(make_pair(-(c + gp[x][i].second), gp[x][i].first));
        }
    }
    for (int i = 1; i <= N; i++)
    {
        printf("%d %d\n", i, dis[i]);
    }
    G.saveDot("example1.dot");
    return 0;
}
