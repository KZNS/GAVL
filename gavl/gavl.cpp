#ifndef GAVL_CPP
#define GAVL_CPP

#include "gavl.h"

/* public */

VGraph::VGraph()
{
    fileName = "gavl.html";
    directed = false;
}

VGraph::~VGraph()
{
    htmlFile << "[" << std::endl;
    if (directed)
    {
        htmlFile << "'digraph {'," << std::endl;
    }
    else
    {
        htmlFile << "'graph {'," << std::endl;
    }
    htmlFile << "'   END'," << std::endl;
    htmlFile << "'}'" << std::endl;
    htmlFile << "]," << std::endl;
    std::ifstream tf(TEMPLATE_T);
    std::string tail((std::istreambuf_iterator<char>(tf)),
                      std::istreambuf_iterator<char>());
    tf.close();
    htmlFile << tail;
    htmlFile.close();
}

int VGraph::setFileName(std::string _fileName)
{
    if (htmlFile.is_open())
    {
        return -1;
    }
    else
    {
        fileName = _fileName;
        return 0;
    }
}

int VGraph::setDirected(bool _directed)
{
    if (htmlFile.is_open())
    {
        return -1;
    }
    else
    {
        directed = _directed;
        return 0;
    }
}

void VGraph::init()
{
    checkEveryStap = false;
    colors[0] = "#ffffff";
    colors[1] = "#ff4500";
    colors[2] = "#df7f0e";
    colors[3] = "#FFD700";
    colors[4] = "#ffbb78";
    colors[5] = "#2ca02c";
    colors[6] = "#98df8a";
    colorBlack = "#000000";
    memset(pointOutNodeLs, -1, sizeof(pointOutNodeLs));
    memset(pointOutEdgeLs, -1, sizeof(pointOutEdgeLs));

    checkBlock = 0;

    std::ifstream tf(TEMPLATE_H);
    std::string head((std::istreambuf_iterator<char>(tf)),
                      std::istreambuf_iterator<char>());
    tf.close();
    htmlFile.open(fileName.c_str());
    htmlFile << head;
}

void VGraph::doneInit()
{
    checkEveryStap = true;
    setCheckPoint();
}

void VGraph::setCheckPoint()
{
    if (checkBlock > 0)
    {
        checkBlock--;
        return;
    }
    StringList sl;

    htmlFile << "[" << std::endl;
    
    sl = sPrintGraphHead_();
    for (int i = 0; i < sl.size(); i++)
    {
        htmlFile << "'" << sl[i] << "'," << std::endl;
    }

    for (VNodeIt nodeIt = nodes.begin(); nodeIt!=nodes.end(); nodeIt++)
    {
        htmlFile << "'" << sPrintNode_(nodeIt) << "'," << std::endl;
    }
    for (VEdgeIt edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++)
    {
         htmlFile << "'" << sPrintEdge_(edgeIt) << "'," << std::endl;
    }

    sl = sPrintGraphTail_();
    for (int i = 0; i < sl.size(); i++)
    {
        htmlFile <<"'" <<sl[i] <<"'," << std::endl;
    }
    
    htmlFile << "]," << std::endl;
}

void VGraph::addNode(int nodeId)
{
    addNode_(nodeId);
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

void VGraph::addNodes(int total)
{
    for (int i = 0; i < total; i++)
    {
        addNode_(i);
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

void VGraph::addNodes(int firstId, int lastId)
{
    for (int i = firstId; i <= lastId; i++)
    {
        addNode_(i);
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

void VGraph::addEdge(int u, int v)
{
    addEdge_(getEdgeId_(u, v), "");
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

template<typename T>
 void VGraph::addEdge(int u, int v, T c)
 {
    addEdge_(getEdgeId_(u, v), toString_(c));
    if (checkEveryStap)
    {
        setCheckPoint();
    }
 }

void VGraph::delNode(int nodeId)
{
    delNode_(nodeId);
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

void VGraph::delEdge(int u, int v)
{
    delEdge_(getEdgeId_(u, v));
    if (checkEveryStap)
    {
        setCheckPoint();
    }
}

int VGraph::setColorNode(int nodeId, std::string color)
{
    int e;
    e = setColorNode_(nodeId, color);
    if (e)
    {
        return e;
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::pointOutNode(int nodeId, int level)
{
    if (!(0 < level && level < NUM_COLOR))
    {
        return -2;
    }
    for (int i = level; i < NUM_COLOR; i++)
    {
        unPointOutNode_(i);
    }
    pointOutNodeLs[level] = nodeId;
    int e;
    e = setColorNode_(nodeId, colors[level]);
    if (e)
    {
        return e;
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::unPointOutNode(int nodeId)
{
    if (nodes.find(nodeId) == nodes.end())
    {
        return -1;
    }
    int level;
    for (level = NUM_COLOR-1; level >= 1; level--)
    {
        if (pointOutNodeLs[level] == nodeId)
        {
            break;
        }
    }
    for (int i = level; i < NUM_COLOR; i++)
    {
        unPointOutNode_(i);
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::setColorEdge(int u, int v, std::string color)
{
    int e;
    e = setColorEdge_(getEdgeId_(u, v), color);
    if (e)
    {
        return e;
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::pointOutEdge(int u, int v, int level)
{
    VEdgeId edgeId;
    edgeId = getEdgeId_(u, v);
    if (!(0 < level && level < NUM_COLOR))
    {
        return -2;
    }
    for (int i = level; i < NUM_COLOR; i++)
    {
        unPointOutEdge_(i);
    }
    pointOutEdgeLs[level] = edgeId;
    int e;
    e = setColorEdge_(edgeId, colors[level]);
    if (e)
    {
        return e;
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::unPointOutEdge(int u, int v)
{
    VEdgeId edgeId;
    edgeId = getEdgeId_(u, v);
    if (edges.find(edgeId) == edges.end())
    {
        return -1;
    }
    int level;
    for (level = NUM_COLOR-1; level >= 1; level--)
    {
        if (pointOutEdgeLs[level] == edgeId)
        {
            break;
        }
    }
    for (int i = level; i < NUM_COLOR; i++)
    {
        unPointOutEdge_(i);
    }
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

template<typename T>
int VGraph::setWeightEdge(int u, int v, T info)
{
    VEdgeId edgeId;
    edgeId = getEdgeId_(u, v);
    if (edges.find(edgeId) == edges.end())
    {
        return -1;
    }
    weightEdge[edgeId] = toString_(info);
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

template<typename T>
int VGraph::setInfoNode(int nodeId, std::string tag, T info)
{
    if (nodes.find(nodeId) == nodes.end())
    {
        return -1;
    }
    nodes[nodeId][tag] = toString_(info);
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::delInfoNode(int nodeId, std::string tag)
{
    if (nodes.find(nodeId) == nodes.end())
    {
        return -1;
    }
    nodes[nodeId].erase(tag);
    if  (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

template<typename T>
int VGraph::setInfoEdge(int u, int v, std::string tag, T info)
{
    VEdgeId x;
    x = getEdgeId_(u, v);
    if (edges.find(x) == edges.end())
    {
        return -1;
    }
    edges[x][tag] = toString_(info);
    if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

int VGraph::delInfoEdge(int u, int v, std::string tag)
{
    VEdgeId x;
    x = getEdgeId_(u, v);
    if (edges.find(x) == edges.end())
    {
        return -1;
    }
    edges[x].erase(tag);
        if (checkEveryStap)
    {
        setCheckPoint();
    }
    return 0;
}

void VGraph::setCheckBlock(int times)
{
    checkBlock = times;
}

void VGraph::saveDot(std::string dotFileName)
{
    std::ofstream dotFile;
    dotFile.open(dotFileName.c_str());

    StringList sl;
    
    sl = sPrintGraphHead_();
    for (int i = 0; i < sl.size(); i++)
    {
        dotFile << sl[i] << std::endl;
    }

    for (VNodeIt nodeIt = nodes.begin(); nodeIt!=nodes.end(); nodeIt++)
    {
        dotFile << sPrintNode_(nodeIt) << std::endl;
    }
    for (VEdgeIt edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++)
    {
        dotFile << sPrintEdge_(edgeIt) << std::endl;
    }

    sl = sPrintGraphTail_();
    for (int i = 0; i < sl.size(); i++)
    {
        dotFile <<sl[i] << std::endl;
    }

    dotFile.close();
}

/* private */

void VGraph::addNode_(int nodeId)
{
    nodes[nodeId] = VProperty();
    colorNode[nodeId] = colors[0];
    shapeNode[nodeId] = "circle";
}

void VGraph::addEdge_(VEdgeId edgeId, std::string c)
{
    if (nodes.find(edgeId.first) == nodes.end())
    {
        addNode_(edgeId.first);
    }
    if (nodes.find(edgeId.second) == nodes.end())
    {
        addNode_(edgeId.second);
    }
    edges[edgeId] = VProperty();
    weightEdge[edgeId] = c;
    colorEdge[edgeId] = colorBlack;
}

void VGraph::delNode_(int nodeId)
{
    nodes.erase(nodeId);
}

void VGraph::delEdge_(VEdgeId edgeId)
{
    edges.erase(edgeId);
    weightEdge.erase(edgeId);
    colorEdge.erase(edgeId);
}

int VGraph::setColorNode_(int nodeId, std::string color)
{
    if (nodes.find(nodeId) == nodes.end())
    {
        return -1;
    }
    colorNode[nodeId] = color;
    return 0;
}

int VGraph::unPointOutNode_(int level)
{
    if (level < 1 || level >= NUM_COLOR)
    {
        return -2;
    }
    int x;
    x = pointOutNodeLs[level];
    if (x == -1)
    {
        return 0;
    }
    pointOutNodeLs[level] = -1;
    setColorNode_(x, colors[0]);
    for (int i = level-1; i >= 1; i--)
    {
        if (pointOutNodeLs[i] == x)
        {
            setColorNode_(x, colors[i]);
            return 0;
        }
    }
    return 0;
}

int VGraph::setColorEdge_(VEdgeId edgeId, std::string color)
{
    if (colorEdge.find(edgeId) == colorEdge.end())
    {
        return -1;
    }
    colorEdge[edgeId] = color;
    return 0;
}

int VGraph::unPointOutEdge_(int level)
{
    if (level < 1 || level >= NUM_COLOR)
    {
        return -2;
    }
    VEdgeId edgeId;
    edgeId = pointOutEdgeLs[level];
    if (edgeId.first == -1)
    {
        return 0;
    }
    pointOutEdgeLs[level] = std::make_pair(-1, -1);
    setColorEdge_(edgeId, colorBlack);
    for (int i = level-1; i >= 1; i--)
    {
        if (pointOutEdgeLs[i] == edgeId)
        {
            setColorEdge_(edgeId, colors[i]);
            return 0;
        }
    }
    return 0;
}

StringList VGraph::sPrintGraphHead_()
{
    StringList sl;
    if (directed)
    {
        sl.push_back("digraph {");
    }
    else
    {
        sl.push_back("graph {");
    }
    sl.push_back("   node [style=\"filled\"]");
    return sl;
}

StringList VGraph::sPrintGraphTail_()
{
    StringList sl;
    sl.push_back("}");
    return sl;
}

std::string VGraph::sPrintNode_(VNodeIt nodeIt)
{
    std::ostringstream oss;
    int nodeId;
    nodeId = nodeIt->first;
    oss << "    " << nodeId << " [";

    oss << "label=\"" << nodeId;
    for (VProperty::iterator it = nodeIt->second.begin(); it != nodeIt->second.end(); it++)
    {
        oss << "\\n" << it->first << ": " << it->second;
    }
    oss << "\" ";
    
    oss << "shape=\"" << shapeNode[nodeId] << "\" "
        << "fillcolor=\"" << colorNode[nodeId] << "\" "
        << "]";
    
    return oss.str();
}

std::string VGraph::sPrintEdge_(VEdgeIt edgeIt)
{
    std::ostringstream oss;
    oss << "    ";
    if (directed)
    {
        oss << edgeIt->first.first << " -> " << edgeIt->first.second;   
    }
    else
    {
        oss << edgeIt->first.first << " -- " << edgeIt->first.second;
    }
    oss << " [";
    oss << "label=\"";
    bool firstLine = true;
    if (!weightEdge[edgeIt->first].empty())
    {
        if (firstLine)
        {
            oss << weightEdge[edgeIt->first];
            firstLine = false;
        }
        else
        {
            oss << "\\n";
            oss << weightEdge[edgeIt->first];
        }
    }

    if (!edgeIt->second.empty())
    {
        if (firstLine)
        {
            VPropertyIt it = edgeIt->second.begin();
            oss << it->first << ": " << it->second;
            for (it++; it != edgeIt->second.end(); it++)
            {
                oss << "\\n" << it->first << ": " << it->second;
            }
            firstLine = false;
        }
        else
        {
            oss << weightEdge[edgeIt->first];
            for (VPropertyIt it = edgeIt->second.begin(); it != edgeIt->second.end(); it++)
            {
                oss << "\\n" << it->first << ": " << it->second;
            }
        }
    }
    oss << "\" ";
    oss << "color=\"" << colorEdge[edgeIt->first] << "\" ";
    oss << "]";

    return oss.str();
}

VEdgeId VGraph::getEdgeId_(int u, int v) const
{
    if (directed)
    {
        return std::make_pair(u, v);
    }
    else
    {
        if (u <= v)
        {
            return std::make_pair(u, v);
        }
        else
        {
            return std::make_pair(v, u);
        }
    }
}

template<typename T>
std::string VGraph::toString_(T info)
{
    std::stringstream strstream;
    strstream.str("");
    strstream << info;
    return strstream.str();
}

#endif
