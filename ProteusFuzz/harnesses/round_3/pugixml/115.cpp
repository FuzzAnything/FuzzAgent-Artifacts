// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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

static pugi::xpath_node_set create_xpath_node_set_from_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse the input directly
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try to create a minimal valid XML
        std::string safe_xml = "<?xml version=\"1.0\"?><root></root>";
        if (!doc.load_string(safe_xml.c_str())) {
            return pugi::xpath_node_set();
        }
    }

    // Create XPath query with pointer to variable set
    pugi::xpath_variable_set vars;
    pugi::xpath_query query(".//node", &vars);
    
    if (!query) {
        // Try a simpler query if the first one fails
        query = pugi::xpath_query("//*", &vars);
    }
    
    if (!query) {
        return pugi::xpath_node_set();
    }

    // Evaluate the query to get a node set
    pugi::xpath_node_set result = query.evaluate_node_set(pugi::xpath_node(doc));
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test pugi::xpath_node_set functions
    pugi::xpath_node_set node_set = create_xpath_node_set_from_data(Data, Size);

    // Test empty()
    bool is_empty = node_set.empty();
    (void)is_empty;

    // Test size()
    size_t sz = node_set.size();
    (void)sz;

    // Test begin() and end()
    auto it_begin = node_set.begin();
    auto it_end = node_set.end();
    
    // Iterate safely
    for (auto it = it_begin; it != it_end; ++it) {
        // Access node if needed
        const pugi::xpath_node& node = *it;
        (void)node;
    }

    // Test sort()
    node_set.sort(false); // Ascending
    node_set.sort(true);  // Descending

    // Test xml_object_range
    pugi::xml_node root;
    if (!node_set.empty()) {
        // begin() returns a const_iterator which is a pointer to xpath_node
        root = node_set.begin()->node();
    }
    
    if (root) {
        pugi::xml_object_range range(root.begin(), root.end());
        bool range_empty = range.empty();
        (void)range_empty;
    }

    return 0;
}