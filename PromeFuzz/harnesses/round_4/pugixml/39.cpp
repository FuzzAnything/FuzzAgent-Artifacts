// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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

static std::string extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return "";
    
    size_t length = 0;
    while (offset + length < size && data[offset + length] != 0) {
        ++length;
    }
    
    std::string result(reinterpret_cast<const char*>(data + offset), length);
    offset += length + 1; // Skip null terminator if present
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    static pugi::xml_document doc;
    static pugi::xml_node node = doc;
    
    size_t offset = 0;
    
    // Extract 9 strings for the 9 function calls
    std::string str1 = extract_string(Data, Size, offset);
    std::string str2 = extract_string(Data, Size, offset);
    std::string str3 = extract_string(Data, Size, offset);
    std::string str4 = extract_string(Data, Size, offset);
    std::string str5 = extract_string(Data, Size, offset);
    std::string str6 = extract_string(Data, Size, offset);
    std::string str7 = extract_string(Data, Size, offset);
    std::string str8 = extract_string(Data, Size, offset);
    std::string str9 = extract_string(Data, Size, offset);
    
    // Ensure null termination for const char* parameters
    if (!str1.empty()) {
        node.prepend_child(str1.c_str());
    }
    
    if (!str2.empty()) {
        node.child(str2.c_str());
    }
    
    if (!str3.empty()) {
        std::string_view sv3(str3);
        node.prepend_child(sv3);
    }
    
    if (!str4.empty()) {
        node.child(str4.c_str());
    }
    
    if (!str5.empty()) {
        node.child(str5.c_str());
    }
    
    if (!str6.empty()) {
        node.prepend_child(str6.c_str());
    }
    
    if (!str7.empty()) {
        node.child(str7.c_str());
    }
    
    if (!str8.empty()) {
        std::string_view sv8(str8);
        node.prepend_child(sv8);
    }
    
    if (!str9.empty()) {
        node.prepend_child(str9.c_str());
    }
    
    // Reset document for next iteration
    doc.reset();
    node = doc;
    
    return 0;
}