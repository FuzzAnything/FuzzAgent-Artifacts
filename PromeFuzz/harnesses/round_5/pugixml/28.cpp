// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    if (size == 0) return "";
    size_t len = std::min(size, max_len);
    std::string str(reinterpret_cast<const char*>(data), len);
    if (str.back() != '\0') {
        str.push_back('\0');
    }
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated string from fuzz data for node names
    std::string node_name = create_null_terminated_string(Data, Size);
    const char* name_ptr = node_name.empty() ? "default" : node_name.c_str();

    // Initialize XML document
    pugi::xml_document doc;
    pugi::xml_node root = doc;

    // 1. First append_child with type
    pugi::xml_node child1 = root.append_child(pugi::node_element);
    
    // 2. Second append_child with name
    pugi::xml_node child2 = root.append_child(name_ptr);
    
    // 3. First child call
    pugi::xml_node found1 = root.child(name_ptr);
    
    // 4. First append_buffer call
    pugi::xml_parse_result result1 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    // 5. Second child call
    pugi::xml_node found2 = root.child(name_ptr);
    
    // 6. Third child call
    pugi::xml_node found3 = root.child("dummy");
    
    // 7. remove_children
    bool removed = root.remove_children();
    
    // 8. Fourth child call (after removal)
    pugi::xml_node found4 = root.child(name_ptr);
    
    // 9. Second append_buffer call
    pugi::xml_parse_result result2 = root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    // 10. Fifth child call
    pugi::xml_node found5 = root.child(name_ptr);

    // Cleanup happens automatically when doc goes out of scope
    return 0;
}