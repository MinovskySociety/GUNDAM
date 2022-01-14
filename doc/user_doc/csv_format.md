# Graph CSV format

## Vertex

### Vertex file format
The basic format consist of two columns:

| vertex_id:data_type | label_id:data_type |
| :-----------------: | :----------------: |

The supported data type see [here](/doc/user_doc/data_type.md).

Example: 

> | vertex_id:int | label_id:int |
> | :-----------: | :----------: |
> |       1       |      0       |
> |       2       |      1       |
> |       3       |      0       |

And a more complicated example:

> | vertex_id:int | label_id:int |
> | :-----------: | :----------: |
> |       1       |      0       |
> |       2       |      1       |
> |       3       |      1       |
> |       4       |      2       |
> |       5       |      2       |
> |       6       |      2       |
> |       7       |      3       |
> |       8       |      3       |
> |       9       |      4       |

### Vertex set file format
| vertex_id:data_type | label_id:data_type | graph_id:data_type |
| :-----------------: | :----------------: | :----: |

### Vertex file with attribute
| vertex_id:data_type | label_id:data_type | attr_key_0:data_type | attr_key_1:data_type | ... |
| :-----------------: | :----------------: | :----: | :----: | :----: |

## Edge

### Edge file format

| edge_id:data_type | source_id:data_type | target_id:data_type | label_id:data_type | 
| :-----------------: | :----------------: | :----: | :----: |

### Edge set file format

| edge_id:data_type | source_id:data_type | target_id:data_type | label_id:data_type | graph_id:data_type |
| :-----------------: | :----------------: | :----: | :----: | :----: |

### Edge file with attribute

| edge_id:data_type | source_id:data_type | target_id:data_type | label_id:data_type |  attr_key_0:data_type | attr_key_1:data_type | ... |
| :-----------------: | :----------------: | :----: | :----: | :----: | :----: | :----: |

