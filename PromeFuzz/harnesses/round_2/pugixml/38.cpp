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
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Create a document and root node to work with
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Prepare string data from fuzzing input
    std::string str_data;
    if (Size > 0) {
        str_data.assign(reinterpret_cast<const char*>(Data), Size);
        // Ensure null termination for const char* parameters
        if (str_data.back() != '\0') {
            str_data.push_back('\0');
        }
    } else {
        str_data = "\0";
    }
    
    std::string_view str_view(str_data.data(), str_data.size() - 1); // Exclude null terminator for string_view
    
    // Sequence of API calls as specified
    // 1. First prepend_child
    root.prepend_child(str_view);  // string_view_t overload
    
    // 2. First child
    root.child(str_data.c_str());  // const char_t* overload
    
    // 3. Second prepend_child
    root.prepend_child(str_data.c_str());  // const char_t* overload
    
    // 4. Second child
    root.child(str_data.c_str());  // const char_t* overload
    
    // 5. Third child
    root.child(str_data.c_str());  // const char_t* overload
    
    // 6. Third prepend_child
    root.prepend_child(str_view);  // string_view_t overload
    
    // 7. Fourth child
    root.child(str_data.c_str());  // const char_t* overload
    
    // 8. Fourth prepend_child
    root.prepend_child(str_data.c_str());  // const char_t* overload
    
    // 9. Fifth prepend_child
    root.prepend_child(str_view);  // string_view_t overload
    
    return 0;
}