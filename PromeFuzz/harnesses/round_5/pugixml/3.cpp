// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_node::attributes_begin at pugixml.cpp:5628:54 in pugixml.hpp
// pugi::xml_node::attributes_end at pugixml.cpp:5633:54 in pugixml.hpp
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
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment, pugi::encoding_utf8);
    
    // Get root node (or empty node if parsing failed)
    pugi::xml_node node = doc.root();
    
    // First sequence: children() -> begin() -> end() -> children()
    auto children_range1 = node.children();
    auto begin1 = node.begin();
    auto end1 = node.end();
    auto children_range2 = node.children();
    
    // Second sequence: xml_object_range::begin() -> xml_object_range::end()
    auto range_begin1 = children_range2.begin();
    auto range_end1 = children_range2.end();
    
    // Third sequence: child() with a name from fuzzer input
    if (Size > 0) {
        // Create null-terminated string for child name
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        std::vector<char> name(name_len + 1);
        memcpy(name.data(), Data, name_len);
        name[name_len] = '\0';
        
        pugi::xml_node child_node = node.child(name.data());
        (void)child_node; // Use variable to avoid unused warning
    }
    
    // Fourth sequence: attributes() -> attributes_begin() -> attributes_end() -> attributes()
    auto attributes_range1 = node.attributes();
    auto attr_begin1 = node.attributes_begin();
    auto attr_end1 = node.attributes_end();
    auto attributes_range2 = node.attributes();
    
    // Fifth sequence: xml_object_range::begin() -> xml_object_range::end() on attributes range
    auto attr_range_begin1 = attributes_range2.begin();
    auto attr_range_end1 = attributes_range2.end();
    
    // Also test with string_view for child() if available (C++17)
    #if __cplusplus >= 201703L
    if (Size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(Data), Size > 50 ? 50 : Size);
        pugi::xml_node child_node2 = node.child(sv.data());
        (void)child_node2;
    }
    #endif
    
    // Additional exploration: iterate through children and attributes
    for (pugi::xml_node child : node.children()) {
        (void)child;
    }
    
    for (pugi::xml_attribute attr : node.attributes()) {
        (void)attr;
    }
    
    // Test with empty node (default constructed)
    pugi::xml_node empty_node;
    auto empty_children = empty_node.children();
    auto empty_begin = empty_node.begin();
    auto empty_end = empty_node.end();
    auto empty_attr_range = empty_node.attributes();
    auto empty_attr_begin = empty_node.attributes_begin();
    auto empty_attr_end = empty_node.attributes_end();
    
    return 0;
}