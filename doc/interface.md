# GUNDAM 使用说明
        本篇文档的目的主要是针对GUNDAM库进行一些接口和使用上的说明，并提供相应的示例程序。

## GUNDAM::Graph

### 相关规定
        1.图的节点与边均有ID以及Label
        2.属性由(key,value)二元组构成。以联通数据为例，phone节点的phonenumber:123456789 属性中，key是phonenumber,value是13456789
### 定义
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
|configure|说明|配置类型|默认类型/值|
|:--|:--|:--|:--|
|SetVertexIDType|设置节点的ID类型|基础数据类型/std::string|unsigned int|
|SetVertexLabelType|设置节点的Label类型|基础数据类型/std::string|unsigned int|
|SetVertexHasAttribute|设置节点是否拥有属性|true/false|true|
|SetVertexAttributeKeyType|设置节点的属性的key的类型|基础数据类型/std::string|unsigned int|
|SetEdgeIDType|设置边的ID的类型|基础数据类型/std::string|unsigned int|
|SetEdgeLabelType|设置边的Label类型|基础数据类型/std::string|unsigned int|
|SetEdgeHasAttribute|设置边是否拥有属性|true/false|true|
|SetEdgeAttributeKeyType|设置边的属性的key的类型|基础数据类型/std::string|unsigned int|
|SetAllowMultipleEdge|设置图是否有重边|true/false|false|

        一些图的配置样例如下所示:
```c++
//只配置节点id和label,其他采用默认配置
//其他部分配置情况类似
GUNDAM::Graph<SetVertexIDType<int>,SetVertexLabelType<std::string>> g;
//全部采用默认配置
GUNDAM::Graph<> g;
//全配置的结果
GUNDAM::Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint16_t>,
SetEdgeIDType<uint32_t>, SetEdgeLabelType<std::string>,SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>,
SetVertexAttributeKeyType<std::string>,
SetEdgeAttributeKeyType<uint32_t>> g;
```
