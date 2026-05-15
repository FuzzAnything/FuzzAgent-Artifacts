// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
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

static std::string make_null_terminated(std::string_view sv) {
    return std::string(sv.data(), sv.length());
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Split input into chunks for string parameters
    size_t chunk_size = Size / 16;
    if (chunk_size == 0) chunk_size = 1;
    
    const char* data_ptr = reinterpret_cast<const char*>(Data);
    
    // Helper to get string view from data
    auto get_string = [&](size_t offset) -> std::string_view {
        if (offset >= Size) return "";
        size_t length = std::min(chunk_size, Size - offset);
        return std::string_view(data_ptr + offset, length);
    };
    
    size_t offset = 0;
    
    // 4 append_child calls
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) {
            // Typed version
            pugi::xml_node_type type = static_cast<pugi::xml_node_type>((Data[offset % Size] % 9) + 1);
            root.append_child(type);
        } else {
            // Named version
            std::string_view name_sv = get_string(offset);
            std::string name = make_null_terminated(name_sv);
            root.append_child(name.c_str());
        }
        offset = (offset + chunk_size) % Size;
    }
    
    // 3 append_attribute calls
    for (int i = 0; i < 3; i++) {
        std::string_view attr_sv = get_string(offset);
        std::string attr_name = make_null_terminated(attr_sv);
        pugi::xml_attribute attr = root.append_attribute(attr_name.c_str());
        if (attr) {
            // Set some value
            std::string_view val_sv = get_string((offset + chunk_size) % Size);
            std::string val = make_null_terminated(val_sv);
            attr.set_value(val.c_str());
        }
        offset = (offset + chunk_size) % Size;
    }
    
    // 8 find_child_by_attribute calls (alternating between overloads)
    for (int i = 0; i < 8; i++) {
        std::string_view name_sv = get_string(offset);
        std::string name = make_null_terminated(name_sv);
        offset = (offset + chunk_size) % Size;
        
        std::string_view attr_name_sv = get_string(offset);
        std::string attr_name = make_null_terminated(attr_name_sv);
        offset = (offset + chunk_size) % Size;
        
        std::string_view attr_val_sv = get_string(offset);
        std::string attr_val = make_null_terminated(attr_val_sv);
        offset = (offset + chunk_size) % Size;
        
        if (i % 2 == 0) {
            // Two-parameter version
            root.find_child_by_attribute(attr_name.c_str(), attr_val.c_str());
        } else {
            // Three-parameter version
            root.find_child_by_attribute(name.c_str(), attr_name.c_str(), attr_val.c_str());
        }
    }
    
    return 0;
}