# Comment 
- [1.Vertex使用说明](#1)
   - [1.1相关规定](#11)
   - [1.2接口](#13)
   - [1.3示例程序](#14)
## 1. Vertex 使用说明

本篇文档的目的主要是针对Vertex进行一些接口和使用上的说明，并提供相应的示例程序。

### 1.1 相关规定

1.本库无法访问具体的Vertex对象，只能通过VertexPtr来得到相关内容，因此调用接口时应该写->而不是.<br>
2.VertexPtr与Vertex的迭代器两者的接口一样，不再赘述。<br>
3.其他规定请参考 [点击此处](./graph.md)。

### 1.2 接口
VertexPtr及Vertex迭代器可调用接口表如下所示:


|接口|接口格式|接口说明|
|:--|:--|:--|
|AddAttribute|pair\<AtributePtr,bool\> AddAttribute(key,value)<br>key:属性的键值<br>value:属性的值<br>返回值:first表示相关属性的指针，second表示该属性插入前是否已经存在于该节点中|往节点中加入属性key值为key,value值为value的属性|
|attribute<br>const_attribute|template\<ValueType\>attribute(key)<br>key:属性的键值<br>ValueType:value值的数据类型<br>返回值:返回key对应的value|得到该节点键值为key的value/常量value|
|attribute_value_type_id|BasicDataType attribute_value_type_id(key)<br>key:属性的键值|得到该节点键值为key的value的类型的id|
|attribute_value_type_name|string attribute_value_type_id(key)<br>key:属性的键值<br>返回值:若类型是整型(int,long long,unsigned int等等)，返回"int",若类型是浮点类型(float,double,long double),返回"double"，若是std::string类型，返回"string"，其他类型返回"unknown type"|得到该节点键值为key的value的类型的名字|
|AttributeBegin<br>AttributeBegin||得到这个节点属性的迭代器/常量迭代器|
|CountInEdge|size_t CountInEdge()|得到指向这个节点的边的数量|
|CountOutEdge|size_t CountOutEdge()|得到指出这个节点的边的数量|
|CountInEdgeLabel|size_t CountInEdgeLabel()|得到指向该节点的所有边的不同Label数量|
|CountInEdgeLabel|size_t CountInEdgeLabel()|得到指出该节点的所有边的不同Label数量|
|CountInVertex|size_t CountInVertex()<br>size_t CountInVertex(vertex_label)<br>vertex_label:节点的label|得到指向该节点所有边的起点的数量/VertexLabel为vertex_label的起点的数量|
|CountInVertex|size_t CountInVertex()<br>size_t CountInVertex(vertex_label)<br>vertex_label:节点的label|得到指出该节点所有边的起点的数量/VertexLabel为vertex_label的起点的数量|
|EraseAttribute|bool EraseAttribute(key)<br>AttributeIterator EraseAttribute(attribute_iterator)<br>key:属性的键值<br>attribute_iterator:属性的迭代器<br>返回值：若传入的参数为key,则返回是否成功删除，若传入的参数为迭代器，则返回删除的迭代器的下一个迭代器|删除该节点键值为key的属性或删除属性迭代器为attribute_iterator的属性|
|EraseEdge|bool EraseEdge(edge_id)<br>EdgeIterator EraseEdge(edge_iterator)<br>edge_id:边的id<br>edge_iterator:边的迭代器<br>返回值:若传入的参数为边的id,则返回是否成功被删除，若传入的参数为边的迭代器，则返回被删除的边的下一条边的迭代器|删除EdgeID为edge_id的边或者删除迭代器为edge_iterator的边|
|FindAttributePtr<br>FindConstAttributePtr|AttributePtr FindAttributePtr(key)<br>AttributeConstPtr FindConstAttributePtr(key)<br>key:属性的键值|找到该节点键值为key对应的Attribute指针/常量指针|
|FindInEdge|EdgePtr FindInEdge(edge_id)<br>edge_id:边的id|找到该节点入边的id为edge_id的边|
|FindInVertex|VertexPtr FindInVertex(dst_ptr)<br>VertexPtr FindInVertex(const typename VertexType::IDType& src_id)<br>||
