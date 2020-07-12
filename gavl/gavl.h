#ifndef GAVL_H
#define GAVL_H

#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <sstream> 

/* 可视化网页模板位置 */
#define TEMPLATE_H "gavl/templateH.html"
#define TEMPLATE_T "gavl/templateT.html"
/* 高亮颜色种类数 */
#define NUM_COLOR 7

/* 结点/边的属性存储结构 及其迭代器 */
typedef std::map<std::string, std::string> VProperty;
typedef VProperty::iterator VPropertyIt;
/* 边的编号类型 */
typedef std::pair<int, int> VEdgeId;
/* 结点集合类型 及其迭代器 */
typedef std::map<int, VProperty> VNodeMap;
typedef VNodeMap::iterator VNodeIt;
/* 边集合类型 及其迭代器*/
typedef std::map<VEdgeId, VProperty> VEdgeMap; 
typedef VEdgeMap::iterator VEdgeIt;

/* 可视化库主体类 */
class VGraph
{
public:
    /* 构造函数，初始化默认值 */
    VGraph();
    /* 析构函数，输出可视化网页的末尾部分并保存 */
    ~VGraph();
    
    /* 
     * 设置可视化网页文件名，需要在init()前执行
     * 如不调用，默认值 “gavl.html”
     */
    int setFileName(std::string _fileName);
    /* 
     * 设置图是否为有向图，需要在init()前执行
     * true 有向图
     * false 无向图
     * 如不调用，默认值 false
     */
    int setDirected(bool _directed);
    /* 初始化图的其余信息，创建可视化网页文件，输出文件头部 */
    void init();
    /* 结束初始化，init至doneInit之间的图操作不产生动画，结果作为图的初始状态*/
    void doneInit();
    /* 设置检查点，输出当前图的状态作为动画的一帧 */
    void setCheckPoint();

    /* 以下操作如果在doneInit之后调用，自动调用setCheckPoint输出修改后的状态*/

    /* 添加一个编号为 nodeId 的结点*/
    void addNode(int nodeId);
    /* 添加 total 个结点，编号从 0 到 total-1 */
    void addNodes(int total); 
    /* 添加一些结点，编号从 firstId 到 lastId（含） */
    void addNodes(int firstId, int lastId);
    /* 添加从 u 指向 v 的有向边或无向边，根据图是否为有向图的设定 */
    void addEdge(int u, int v);
    /* 添加从 u 指向 v 的有向边或无向边, 边权为 c */
    template<typename T>
    void addEdge(int u, int v, T c);
    /* 删除编号为nodeId的结点*/
    void delNode(int nodeId);
    /* 在有向图中删除从u到v的边，在无向图中删除u和v之间的边 */
    void delEdge(int u, int v);
  
    /* 显示效果部分*/

    /*
     * 直接设置nodeId节点的颜色为color
     * color格式为16进制颜色码，如 #aec7e8
     */
    int setColorNode(int nodeId, std::string color);
    /*
     * 高亮 nodeId 结点，高亮层级为 level [1, 6]
     * 每一层级同时刻只能有一个结点
     * 在相同层级设置结点高亮，会取消前一结点以及层级数更大的结点的高亮效果
     */
    int pointOutNode(int nodeId, int level);
    /*
     * 取消nodeId结点的高亮
     * 同时会取消层级数比nodeId结点更大的结点的高亮效果
     */
    int unPointOutNode(int nodeId);
    /*
     * 直接设置从u到v的边的颜色为color
     * color格式为16进制颜色码，如 #aec7e8
     */
    int setColorEdge(int u, int v, std::string color);
    /*
     * 高亮从u到v的边，高亮层级为 level [1, 6]
     * 每一层级同时刻只能有一条边
     * 在相同层级设置边高亮，会取消前一条边以及层级数更大的边的高亮效果
     */
    int pointOutEdge(int u, int v, int level);
    /*
     * 取消从u到v的边的高亮
     * 同时会取消层级数比从u到v的边更大的边的高亮效果
     */
    int unPointOutEdge(int u, int v);

    /* 设置从u到v的边的权值，值为info */
    template<typename T>
    int setWeightEdge(int u, int v, T info);

    /* 其他结点/边上的信息信息通过以下接口设置 */

    /* 设置 nodeId 结点信息，标签为tag，内容为info*/
    template<typename T>
    int setInfoNode(int nodeId, std::string tag, T info);
    /* 删除 nodeId 结点标签为 tag 的信息*/
    int delInfoNode(int nodeId, std::string tag);
    /* 设置从 u 到 v 边的信息，标签为tag，内容为info*/
    template<typename T>
    int setInfoEdge(int u, int v, std::string tag, T info);
    /* 删除从 u 到 v 边的标签为 tag 的信息*/
    int delInfoEdge(int u, int v, std::string tag);
    
    /* 其他功能 */
    
    /* 
     * 接下来times次修改不输出状态到动画
     * 注意被屏蔽输出的修改将在有效输出中作为同一帧动画一起发生
     */
    void setCheckBlock(int times);

private:
    /* true 有向图，false 无向图 */
    bool directed;
    /* 可视化网页文件名 */
    std::string fileName;
    /* 结点集合 */
    VNodeMap nodes;
    /* 边集合 */
    VEdgeMap edges;

    /* ture：在每一次修改后输出图状态 */
    bool checkEveryStap;

    /* 可视化网页的文件流 */
    std::ofstream dotFile;

    /* 内置的NUM_COLOR种颜色 */
    std::string colors[NUM_COLOR];
    /* 黑色 */
    std::string colorBlack;
    
    /* 结点颜色 */
    std::map<int, std::string> colorNode;
    /* 结点形状 */
    std::map<int, std::string> shapeNode;
    /* 边颜色 */
    std::map<VEdgeId, std::string> colorEdge;

    /* 边的权值 */
    std::map<std::pair<int, int> , std::string> weightEdge;

    /* 高亮结点列表 */
    int pointOutNodeLs[NUM_COLOR];
    /* 高亮边列表 */
    VEdgeId pointOutEdgeLs[NUM_COLOR];

    /* 屏蔽setCheckPoint的次数 */
    int checkBlock;
    
    /* 通过以下方法，方便添加多种对外接口 */

    /* 添加编号为nodeId结点 */
    void addNode_(int nodeId);
    /* 添加编号为edgeId的边，权值为c */
    void addEdge_(VEdgeId edgeId, std::string c);
    /* 删除编号为nodeId结点 */
    void delNode_(int nodeId);
    /* 删除编号为edgeId的边 */
    void delEdge_(VEdgeId edgeId);

    /* 设置nodeId结点颜色为color*/
    int setColorNode_(int nodeId, std::string color);
    /* 取消层级为level的结点的高亮*/
    int unPointOutNode_(int level);
    /* 设置edgeId边颜色为color*/
    int setColorEdge_(VEdgeId edgeId, std::string color);
    /* 取消层级为level的边的高亮*/
    int unPointOutEdge_(int level);

    /* 以下方法输出图的状态到可视化网页 */

    /* 输出结点状态 */
    void printNode_(VNodeIt nodeIt);
    /* 输出边状态 */
    void printEdge_(VEdgeIt edgeIt);

    /* 以下方法方便内部处理 */
    
    /* 获得从 u 到 v 的边的编号 */
    VEdgeId getEdgeId_(int u, int v) const;
    /* 转换任意类型数据到字符串 */
    template<typename T>
    std::string toString_(T info);
};

#include "gavl.cpp"

#endif
