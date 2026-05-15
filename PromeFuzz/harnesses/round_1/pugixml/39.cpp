// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Ensure we have enough data for string operations
    if (Size == 0) {
        return 0;
    }
    
    // Create null-terminated strings from the input data
    // We'll use different segments of the input for different function calls
    size_t segment_size = Size / 12;
    if (segment_size == 0) {
        segment_size = 1;
    }
    
    // Array to hold string data for each function call
    char str_data[12][256];
    std::string_view str_views[12];
    
    // Prepare string data for each function call
    for (int i = 0; i < 12; i++) {
        size_t start = i * segment_size;
        size_t len = (i == 11) ? (Size - start) : segment_size;
        if (len > 255) len = 255;
        
        if (start < Size) {
            // Copy data and ensure null termination
            size_t copy_len = (start + len <= Size) ? len : (Size - start);
            memcpy(str_data[i], Data + start, copy_len);
            str_data[i][copy_len] = '\0';
            str_views[i] = std::string_view(str_data[i], copy_len);
        } else {
            // Use default string if not enough data
            strcpy(str_data[i], "attr");
            str_views[i] = std::string_view("attr");
        }
    }
    
    int str_idx = 0;
    
    // 1. First prepend_attribute (const char* overload)
    node.prepend_attribute(str_data[str_idx++]);
    
    // 2. Second prepend_attribute (string_view overload)
    node.prepend_attribute(str_views[str_idx++]);
    
    // 3. First child call
    node.child(str_data[str_idx++]);
    
    // 4. Third prepend_attribute (const char* overload)
    node.prepend_attribute(str_data[str_idx++]);
    
    // 5. Second child call
    node.child(str_data[str_idx++]);
    
    // 6. Fourth prepend_attribute (string_view overload)
    node.prepend_attribute(str_views[str_idx++]);
    
    // 7. Third child call
    node.child(str_data[str_idx++]);
    
    // 8. Fourth child call
    node.child(str_data[str_idx++]);
    
    // 9. Fifth prepend_attribute (const char* overload)
    node.prepend_attribute(str_data[str_idx++]);
    
    // 10. Fifth child call
    node.child(str_data[str_idx++]);
    
    // 11. Sixth child call
    node.child(str_data[str_idx++]);
    
    // 12. Sixth prepend_attribute (string_view overload)
    node.prepend_attribute(str_views[str_idx++]);
    
    return 0;
}