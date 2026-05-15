// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Create XML document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node and first child
    pugi::xml_node root = doc.root();
    pugi::xml_node first_child = root.first_child();
    
    // Create string views from input data for attribute names
    // Use safe bounds checking to avoid out-of-bounds access
    size_t data_len = Size;
    const char* data_ptr = reinterpret_cast<const char*>(Data);
    
    // Prepare string views for attribute lookups
    std::string_view attr1, attr2, attr3, attr4, attr5, attr6, attr7;
    std::string_view attr8, attr9, attr10, attr11;
    std::string_view set_name_str;
    
    // Initialize string views with safe bounds checking
    auto safe_sv = [&](size_t offset, size_t length) -> std::string_view {
        if (offset >= data_len) return "";
        size_t actual_length = std::min(length, data_len - offset);
        return std::string_view(data_ptr + offset, actual_length);
    };
    
    // Distribute the input data across different attribute names
    size_t chunk_size = data_len / 12;
    if (chunk_size == 0) chunk_size = 1;
    
    attr1 = safe_sv(0, chunk_size);
    attr2 = safe_sv(chunk_size, chunk_size);
    attr3 = safe_sv(2 * chunk_size, chunk_size);
    attr4 = safe_sv(3 * chunk_size, chunk_size);
    attr5 = safe_sv(4 * chunk_size, chunk_size);
    attr6 = safe_sv(5 * chunk_size, chunk_size);
    attr7 = safe_sv(6 * chunk_size, chunk_size);
    set_name_str = safe_sv(7 * chunk_size, chunk_size);
    attr8 = safe_sv(8 * chunk_size, chunk_size);
    attr9 = safe_sv(9 * chunk_size, chunk_size);
    attr10 = safe_sv(10 * chunk_size, chunk_size);
    attr11 = safe_sv(11 * chunk_size, chunk_size);
    
    // Create hint attributes for the second overload
    pugi::xml_attribute hint1, hint2, hint3, hint4, hint5, hint6, hint7;
    pugi::xml_attribute hint8, hint9, hint10, hint11;
    
    // Call attribute functions in specified order
    // First 7 calls before set_name
    pugi::xml_attribute attr_result1 = first_child.attribute(attr1.data());
    pugi::xml_attribute attr_result2 = first_child.attribute(attr2.data());
    pugi::xml_attribute attr_result3 = first_child.attribute(attr3, hint3);
    pugi::xml_attribute attr_result4 = first_child.attribute(attr4.data());
    pugi::xml_attribute attr_result5 = first_child.attribute(attr5, hint5);
    pugi::xml_attribute attr_result6 = first_child.attribute(attr6.data());
    pugi::xml_attribute attr_result7 = first_child.attribute(attr7, hint7);
    
    // Call set_name on an attribute
    // Create a dummy attribute and try to set its name
    pugi::xml_attribute dummy_attr;
    if (!set_name_str.empty()) {
        dummy_attr.set_name(set_name_str);
    }
    
    // Last 4 calls after set_name
    pugi::xml_attribute attr_result8 = first_child.attribute(attr8.data());
    pugi::xml_attribute attr_result9 = first_child.attribute(attr9, hint9);
    pugi::xml_attribute attr_result10 = first_child.attribute(attr10.data());
    pugi::xml_attribute attr_result11 = first_child.attribute(attr11, hint11);
    
    // Use the results to avoid compiler optimizations
    (void)attr_result1;
    (void)attr_result2;
    (void)attr_result3;
    (void)attr_result4;
    (void)attr_result5;
    (void)attr_result6;
    (void)attr_result7;
    (void)attr_result8;
    (void)attr_result9;
    (void)attr_result10;
    (void)attr_result11;
    
    return 0;
}