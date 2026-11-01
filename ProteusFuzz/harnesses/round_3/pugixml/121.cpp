// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        return 0;
    }

    // Test xpath_node_set operations
    pugi::xpath_node_set set;
    
    // Try various XPath queries to populate the set
    const char* query = reinterpret_cast<const char*>(Data);
    if (Size > 0 && Data[0] == '/') {
        set = doc.select_nodes(query);
    } else {
        // Fallback to simple queries if input doesn't look like a valid XPath
        set = doc.select_nodes("/");
    }

    // Test empty()
    bool is_empty = set.empty();
    (void)is_empty;

    // Test size()
    size_t sz = set.size();
    (void)sz;

    // Test begin() and end()
    pugi::xpath_node_set::const_iterator it = set.begin();
    pugi::xpath_node_set::const_iterator end_it = set.end();
    
    // Iterate safely
    for (; it != end_it; ++it) {
        // Just traverse to exercise the iterator logic
        (void)it->node();
        (void)it->attribute();
    }

    // Test sort()
    set.sort(false);
    set.sort(true);

    // Test xml_object_range operations
    pugi::xml_node root = doc.first_child();
    if (root) {
        pugi::xml_object_range range(root.begin(), root.end());
        
        // Test empty() on range
        bool range_empty = range.empty();
        (void)range_empty;
    }

    return 0;
}