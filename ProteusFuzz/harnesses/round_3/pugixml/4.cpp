// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // If parsing fails, we can still test with an empty document or root node
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc;
    }

    // 1. pugi::xml_node::children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();

    // 2. pugi::xml_node::begin
    pugi::xml_node_iterator begin_iter = root.begin();

    // 3. pugi::xml_node::end
    pugi::xml_node_iterator end_iter = root.end();

    // 4. pugi::xml_node::children (again)
    pugi::xml_object_range<pugi::xml_node_iterator> children_range2 = root.children();

    // 5. pugi::xml_object_range::begin
    pugi::xml_node_iterator range_begin = children_range2.begin();

    // 6. pugi::xml_object_range::end
    pugi::xml_node_iterator range_end = children_range2.end();

    // 7. pugi::xml_node::child
    // We need a valid C-string for the name. 
    // To avoid use-after-free, we create a local std::string that lives long enough.
    std::string name_str = "";
    size_t null_pos = xml_str.find('\0');
    if (null_pos != std::string::npos && null_pos > 0) {
        name_str = xml_str.substr(0, null_pos);
    } else {
        // Fallback to a safe empty string
        name_str = "";
    }
    pugi::xml_node child_node = root.child(name_str.c_str());

    // 8. pugi::xml_node::attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range = root.attributes();

    // 9. pugi::xml_node::attributes_begin
    pugi::xml_attribute_iterator attrs_begin = root.attributes_begin();

    // 10. pugi::xml_node::attributes_end
    pugi::xml_attribute_iterator attrs_end = root.attributes_end();

    // 11. pugi::xml_node::attributes (again)
    pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range2 = root.attributes();

    // 12. pugi::xml_object_range::begin (for attributes)
    pugi::xml_attribute_iterator attrs_range_begin = attrs_range2.begin();

    // 13. pugi::xml_object_range::end (for attributes)
    pugi::xml_attribute_iterator attrs_range_end = attrs_range2.end();

    // Perform some dummy iteration to ensure state changes are exercised
    (void)begin_iter;
    (void)end_iter;
    (void)range_begin;
    (void)range_end;
    (void)child_node;
    (void)attrs_begin;
    (void)attrs_end;
    (void)attrs_range_begin;
    (void)attrs_range_end;

    // Iterate children if possible
    for (auto it = children_range.begin(); it != children_range.end(); ++it) {
        (void)it;
    }

    // Iterate attributes if possible
    for (auto it = attrs_range.begin(); it != attrs_range.end(); ++it) {
        (void)it;
    }

    return 0;
}