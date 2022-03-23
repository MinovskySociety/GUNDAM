# Serialize

Graph:
>  v1 <- v2 -> v3 <- v4

Serialized string:

> <Graph vertex 1 1 2 0 3 1 4 0 edge 2 1 2 2 2 3 2 3 4 3 2 4 >

Can be decomposed as:

Vertex:

> |  vertex_id  | vertex_label |
> | :---------: | :----------: |
> | 1 | 1 |
> | 2 | 0 |
> | 3 | 1 |
> | 4 | 0 |


Edge:

> | src_id | dst_id | edge_label | edge_id |
> | :----: | :----: | :--------: | :-----: |
> | 2 | 1 | 2 | 2 |
> | 2 | 3 | 2 | 3 |
> | 4 | 3 | 2 | 4 |