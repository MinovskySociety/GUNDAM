# Todo List:
ToDo:
1.	Graph的Construct方法中加入拷贝属性的过程

2.	考虑是否能将Graph中Vertex与Edge的Attribute提取出来放到

3.	右值构造方法（优先在Graph中添加）

4.	补完各种迭代器到指针的转换

5.	Vertex的In和Out容器整合在一起、提供同时访问In、Out的迭代器

6.	如NAS上Attribute方案所示、完善Attribute

7.	集合拷贝的方法，例如：  
	VertexSetType vertex_set;   
	vertex_ptr->CopyVertexSetTo(vertex_set);   
	vertex_ptr->CopyVertexSetTo(edge_label, vertex_set); 
