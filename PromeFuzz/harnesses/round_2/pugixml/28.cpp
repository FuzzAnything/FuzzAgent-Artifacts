// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
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

static std::string get_null_terminated_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) {
        return "";
    }
    
    size_t start = offset;
    while (offset < size && data[offset] != 0) {
        offset++;
    }
    
    size_t length = offset - start;
    std::string result(reinterpret_cast<const char*>(data + start), length);
    
    if (offset < size) {
        offset++; // Skip the null terminator
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Create document
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    if (!root) {
        return 0;
    }
    
    // First append_child (with type)
    pugi::xml_node child1 = root.append_child(pugi::node_element);
    
    // Second append_child (with name)
    std::string child_name = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node child2 = root.append_child(child_name.c_str());
    
    // First child call
    std::string child_name1 = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node found_child1 = root.child(child_name1.c_str());
    
    // First append_buffer
    if (Size - offset > 0) {
        size_t buffer_size = std::min(Size - offset, static_cast<size_t>(1024));
        pugi::xml_parse_result result = root.append_buffer(
            Data + offset, 
            buffer_size, 
            pugi::parse_default, 
            pugi::encoding_auto
        );
        offset += buffer_size;
    }
    
    // Second child call
    std::string child_name2 = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node found_child2 = root.child(child_name2.c_str());
    
    // Third child call
    std::string child_name3 = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node found_child3 = root.child(child_name3.c_str());
    
    // remove_children
    bool removed = root.remove_children();
    
    // Fourth child call (after removal)
    std::string child_name4 = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node found_child4 = root.child(child_name4.c_str());
    
    // Second append_buffer
    if (Size - offset > 0) {
        size_t buffer_size = std::min(Size - offset, static_cast<size_t>(1024));
        pugi::xml_parse_result result = root.append_buffer(
            Data + offset, 
            buffer_size, 
            pugi::parse_default, 
            pugi::encoding_auto
        );
    }
    
    // Fifth child call
    std::string child_name5 = get_null_terminated_string(Data, Size, offset);
    pugi::xml_node found_child5 = root.child(child_name5.c_str());
    
    return 0;
}