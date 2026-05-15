// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Prepare strings from fuzzer input
    // We'll use the first part of data for string operations
    size_t str_len = Size > 100 ? 100 : Size;
    std::string str_data(reinterpret_cast<const char*>(Data), str_len);
    
    // Ensure null termination for C strings
    if (!str_data.empty()) {
        str_data[str_data.size() - 1] = '\0';
    }
    
    std::string_view sv_data(str_data);
    
    // 1. First prepend_child with string_view
    root.prepend_child(sv_data);
    
    // 2. First child call
    root.child(str_data.c_str());
    
    // 3. Second prepend_child with const char*
    root.prepend_child(str_data.c_str());
    
    // 4. Second child call
    root.child(str_data.c_str());
    
    // 5. Third child call
    root.child(str_data.c_str());
    
    // 6. Third prepend_child with string_view
    root.prepend_child(sv_data);
    
    // 7. Fourth child call
    root.child(str_data.c_str());
    
    // 8. Fourth prepend_child with const char*
    root.prepend_child(str_data.c_str());
    
    // 9. Fifth prepend_child with string_view
    root.prepend_child(sv_data);

    return 0;
}