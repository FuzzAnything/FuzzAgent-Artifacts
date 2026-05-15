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
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::children
    auto children_range1 = root.children();
    
    // 2. pugi::xml_node::begin
    auto begin1 = root.begin();
    
    // 3. pugi::xml_node::end
    auto end1 = root.end();
    
    // 4. pugi::xml_node::children
    auto children_range2 = root.children();
    
    // 5. pugi::xml_object_range::begin
    auto range_begin1 = children_range2.begin();
    
    // 6. pugi::xml_object_range::end
    auto range_end1 = children_range2.end();
    
    // 7. pugi::xml_node::child
    // Create a null-terminated string from fuzzer input for child name
    static char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (Size > 0) {
        memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }
    pugi::xml_node child = root.child(child_name);
    
    // 8. pugi::xml_node::attributes
    auto attributes_range1 = root.attributes();
    
    // 9. pugi::xml_node::attributes_begin
    auto attr_begin1 = root.attributes_begin();
    
    // 10. pugi::xml_node::attributes_end
    auto attr_end1 = root.attributes_end();
    
    // 11. pugi::xml_node::attributes
    auto attributes_range2 = root.attributes();
    
    // 12. pugi::xml_object_range::begin
    auto attr_range_begin1 = attributes_range2.begin();
    
    // 13. pugi::xml_object_range::end
    auto attr_range_end1 = attributes_range2.end();
    
    // Additional exploration: iterate through children and attributes
    for (auto it = root.begin(); it != root.end(); ++it) {
        // Do nothing, just exercise the iterator
        (void)it;
    }
    
    for (auto attr : root.attributes()) {
        // Do nothing, just exercise the range-based loop
        (void)attr;
    }
    
    return 0;
}