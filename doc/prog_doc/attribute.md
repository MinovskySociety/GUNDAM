# Attribute

## Overview

Both vertex and edge support dynamic attribute that can be configured at runtime and share the same interface.

Dynamic type system:

```c++
const std::string kValue0 = "value0";
const int32_t     kValue1 = 1;
vertex_handle->AddAttribute("key0", kValue0);
vertex_handle->AddAttribute("key1", kValue1);
std::cout << vertex_handle->template attribute<std::string>("key0") 
          << std::endl;
std::cout << vertex_handle->template attribute<int32_t>("key1")
          << std::endl;
// std::cout << vertex_handle->template attribute<int32_t>("key0") 
//           << std::endl;  // runtime type error, undefined output
```

Output:
```
value0
1
```

## Interaface

### AddAttribute(key, value)

### attribute(key)

### AttributeBegin()

### FindAttribute(key)

### SetAttribute(key)

### EraseAttribute(attribute_iterator)

### EraseAttribute(key)
