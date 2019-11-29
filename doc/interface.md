[GUNDAM::Graph使用说明](#1)
* [1.相关规定](#1.1)
* [2.定义](#1.2)
* [3.接口](#1.3)
* [4.示例程序](#1.4)
* [5.相关类型定义](#1.5)
<h2 id="1">GUNDAM::Graph 使用说明</h2>

        本篇文档的目的主要是针对GUNDAM::Graph进行一些接口和使用上的说明，并提供相应的示例程序。

<h3 id="1.1">1.相关规定</h3>

        1.图的节点与边均有ID以及Label，且节点的ID需要两两不同，边的ID需要两两不同
        2.图的节点和边可以选择是否存在Attribute
        3.属性由(key,value)二元组构成。以联通数据为例，phone节点的phonenumber:123456789 属性中，key是phonenumber,value是13456789


<h3 id="1.2">2.定义</h3>

        Graph的一般性定义如下所示:

```c++
//when no "using namespace GUNDAM;"
GUNDAM::Graph<configure...> g;
//using namespace GUNDAM;
using namespace GUNDAM;
Graph<configures...> g;
```
        其中，configures...表示对于图中一些类型的配置项。按需配置即可，不想配置的项可以不写，会有默认类型。配置项书写的前后顺序没有要求，可以随意调换位置。
        configures...中可配置项如下表所示：
|configure|说明|type配置类型|type默认类型/值|
|:--|:--|:--|:--|
|SetVertexIDType\<type\>|设置节点的ID类型|基础数据类型/std::string|unsigned int|
|SetVertexLabelType\<type\>|设置节点的Label类型|基础数据类型/std::string|unsigned int|
|SetVertexHasAttribute\<type\>|设置节点是否拥有属性|true/false|true|
|SetVertexAttributeKeyType\<type\>|设置节点的属性的key的类型|基础数据类型/std::string|unsigned int|
|SetEdgeIDType\<type\>|设置边的ID的类型|基础数据类型/std::string|unsigned int|
|SetEdgeLabelType\<type\>|设置边的Label类型|基础数据类型/std::string|unsigned int|
|SetEdgeHasAttribute\<type\>|设置边是否拥有属性|true/false|true|
|SetEdgeAttributeKeyType\<type\>|设置边的属性的key的类型|基础数据类型/std::string|unsigned int|
|SetAllowMultipleEdge\<type\>|设置图是否有重边|true/false|false|

        一些图的配置样例如下所示:
```c++
//只配置节点id和label,其他采用默认配置
//其他部分配置情况类似
GUNDAM::Graph<SetVertexIDType<int>,SetVertexLabelType<std::string>> g;
//全部采用默认配置
GUNDAM::Graph<> g;
//全配置的结果
GUNDAM::Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint16_t>,
SetEdgeIDType<uint32_t>, SetEdgeLabelType<std::string>,SetAllowMultipleEdge<true>,
SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>,
SetVertexAttributeKeyType<std::string>,
SetEdgeAttributeKeyType<uint32_t>> g;
```

<h3 id="1.3">3.接口</h3>

        Graph的可调用接口表如下所示:
|接口|参数|接口说明|返回值|
|:--|:--|:--|:--|
|AddVertex|(vertex_id,vertex_label)|往Graph中新加入一个ID为vertex_id,Label为vertex_label的节点|pair\<VertexPtr,bool\>的二元组，first表示该节点的指针，second表示加入前Graph中是否存在VertexID为vertex_id的点|
|AddEdge|(src_id,dst_id,edge_label,edge_id)|往Graph中加入一条从src_id到dst_id,EdgeLabel为edge_label,EdgeID为edge_id的有向边|pair\<EdgePtr,bool\>的二元组，first表示该边的指针，second表示加入前Graph中是否存在EdgeID为edge_id的点|
|CountVertex|无/vertex_label|计算这个图的节点数量/满足VertexLabel为vertex_label的节点数量|size_t类型的值，代表满足要求的点的数量|
|FindVertex|vertex_id|找到Graph中VertexID为vertex_id的节点的指针|若图中存在vertex_id对应的节点，则返回其指针，否则返回Null指针|
|FindConstVertex|vertex_id|找到Graph中VertexID为vertex_id的节点的Const指针|若图中存在vertex_id对应的节点，则返回其Const指针，否则返回Null指针|
|FindEdge|edge_id|找到Graph中EdgeID为edge_id的边的指针|若图中存在edge_id对应的边，则返回其指针，否则返回Null指针|
|EraseEdge|edge_id|删除Graph中EdgeID为edge_id的边的指针|若成功删除返回true,否则返回false|
|VertexBegin/VertexCBegin|无/vertex_label|得到Vertex/Label为vertex_label的Vertex的迭代器/常量迭代器|Vertex/Label为vertex_label的Vertex的迭代器/常量迭代器|
|vertex_id_type|无|返回VertexIDType的字符串|若类型是整型(int,long long,unsigned int等等)，返回"int",若类型是浮点类型(float,double,long double),返回"double"，若是std::string类型，返回"string"，其他类型返回"unknown type"|
|vertex_label_type|无|返回VertexLabelType的字符串|若类型是整型(int,long long,unsigned int等等)，返回"int",若类型是浮点类型(float,double,long double),返回"double"，若是std::string类型，返回"string",其他类型返回"unknown type"|
|edge_id_type|无|返回EdgeIDType的字符串|若类型是整型(int,long long,unsigned int等等)，返回"int",若类型是浮点类型(float,double,long double),返回"double"，若是std::string类型，返回"string",其他类型返回"unknown type"|
|edge_label_type|无|返回EdgeLabelType的字符串|若类型是整型(int,long long,unsigned int等等)，返回"int",若类型是浮点类型(float,double,long double),返回"double"，若是std::string类型，返回"string",其他类型返回"unknown type"|

<h3 id="1.4">4.示例程序</h3>

        Graph的相关实例程序如下所示：

```c++
GUNDAM::Graph<> g;
for (int i=1;i<=10;i++){
    //Add Vertex
    g.AddVertex(i,1);
}
for (int i=11;i<=20;i++){
    g.AddVertex(i,2);
}
int edge_id =1;
for (int i=1;i<=10;i++){
    for (int j=1;j<=10;j++){
        if (i==j) continue;
        //add edge
        g.AddEdge(i,j,2,edge_id++);
    }
}
//visit all vertex
for (auto it = g.VertexBegin();!it.IsDone();it++{
    std::cout<<it->id()<<" "<< it->label()<<std::endl;
}
//visit all 2-label vertex
for (auto it = g.VertexBegin(2);!it.IsDone();it++{
    std::cout<<it->id()<<" "<< it->label()<<std::endl;
}
//vertex num
int node_num = g.CountVertex();
//count 1-label vertex
int label_is_1_num = g.CountVertex(1);
//Find Vertex
typename GUNDAM::Graph<>::VertexPtr node_ptr = g.FindVertex(2);
//Find Edge
typename GUNDAN::Graph<>::EdgePtr edge_ptr = g.FindEdge(10);
//Erase Edge
bool erase_flag = g.EraseEdge(11);
//print id and label type
std::cout<<"vertex id type = "<<g.vertex_id_type()<<std::endl;
std::cout<<"vertex label type = "<<g.vertex_label_type()<<std::endl;
std::cout<<"edge id type = "<<g.edge_id_type()<<std::endl;
std::cout<<"edge label type = "<<g.edge_label_type()<<std::endl;

```

<h3 id="1.5">5.相关类型定义</h3>

        GUNDAM中常见的类型定义如下所示：
```c++
using GraphType = GUNDAM::Graph<>;
using VertexType = typename GraphType::VertexType;
using EdgeType = typename GraphType::EdgeType;
using VertexIDType = typename VertexType::IDType;
using VertexLabelType = typename VertexType::LabelType;
using EdgeIDType = typename GraphType::EdgeType::IDType;
using EdgeLabelType = typename EdgeType::LabelType;
using VertexPtr = typename GraphType::VertexPtr;
using VertexConstPtr = typename GraphType::VertexConstPtr;
using EdgePtr = typename GraphType::EdgePtr;
using EdgeConstPtr = typename GraphType::EdgeConstPtr;
using VertexAttributeKeyType = typename GraphType::VertexType::AttributeKeyType;
using EdgeAttributeKeyType = typename GraphType::EdgeType::AttributeKeyType;
```