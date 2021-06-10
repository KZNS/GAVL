# GAVL - A C++ Graph Algorithm Visualization Library base on Graphviz

## 使用方式

1. 实现C++图论算法
2. 将`gavl`文件夹放入算法同级目录下
3. 在代码调用相关接口保存图信息
4. 运行代码
5. 代码运行结束，生成图论算法可视化文件`.html`
6. 直接在浏览器中打开生成的文件即可

一些例子见example*

### 通常可视化接口调用方式

``` c++
//包含头文件
#include "gavl/gavl.h"

//定义一个可视化图
VGraph G;

//设置可视化图的一些信息
G.setFileName("example1.html");
G.setDirected(false);

//添加节点
G.addNode(1)
G.addNodes(10);
G.addNodes(1, N);

//添加边
G.addEdge(u, v);
G.addEdge(u, v, c);

//初始化图
G.init();

//根据图论算法，对图进行修改
G.addNode(3)
G.addEdge(u, v);
G.delNode(3)
G.delEdge(3, 4);

G.setWeightEdge(3, 4, 10);

//或者高亮某个节点或者边，来展示对节点或者边的访问
G.pointOutNode(x, 1);
G.pointOutEdge(u, v, 1);

//代码结束时自动输出.html文件

//随时保存一个当前图状态到.dot文件中
G.saveDot()
```

## 引用

本项目使用了[d3-graphviz](https://github.com/magjac/d3-graphviz)实现图的显示和动画效果
