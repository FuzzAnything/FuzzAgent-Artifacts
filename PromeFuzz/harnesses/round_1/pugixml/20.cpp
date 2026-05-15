// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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
#include <stdint.h>
#include <string.h>

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzzing input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, use a default document to still exercise the API
    if (!result) {
        const char* default_xml = "<root><child1/><child2/><child3/></root>";
        doc.load_string(default_xml);
    }
    
    // Get root node for XPath queries
    pugi::xml_node root = doc.root();
    
    // Create an XPath query string from fuzzing data (or use default)
    std::string xpath_query;
    if (Size > 0) {
        // Use first part of data as XPath query, but ensure it's null-terminated
        size_t query_len = Size > 100 ? 100 : Size; // Limit query length
        xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
        
        // Replace non-printable characters with spaces to avoid invalid queries
        for (char& c : xpath_query) {
            if (c < 32 || c > 126) c = ' ';
        }
    } else {
        xpath_query = "//*"; // Default query if no data
    }
    
    // Execute XPath query to get node set - use try-catch for safety
    pugi::xpath_node_set node_set;
    try {
        node_set = root.select_nodes(xpath_query.c_str());
    } catch (...) {
        // Catch any exceptions and use empty node set
        node_set = pugi::xpath_node_set();
    }
    
    // Call the target API functions in specified order
    // 1. pugi::xpath_node_set::size
    size_t set_size = node_set.size();
    (void)set_size; // Use variable to avoid unused warning
    
    // 2. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t set_type = node_set.type();
    (void)set_type;
    
    // 3. pugi::xpath_node_set::empty
    bool is_empty = node_set.empty();
    (void)is_empty;
    
    // 4. pugi::xpath_node_set::first
    pugi::xpath_node first_node = node_set.first();
    (void)first_node;
    
    // 5. pugi::xpath_node_set::end
    pugi::xpath_node_set::const_iterator end_iter = node_set.end();
    (void)end_iter;
    
    // 6. pugi::xml_node::select_nodes
    // Create another query using remaining data (or default)
    std::string second_query;
    if (Size > 50) {
        // Use different part of data for second query
        size_t offset = Size / 2;
        size_t second_len = Size - offset > 100 ? 100 : Size - offset;
        second_query.assign(reinterpret_cast<const char*>(Data + offset), second_len);
        
        // Replace non-printable characters
        for (char& c : second_query) {
            if (c < 32 || c > 126) c = ' ';
        }
    } else {
        second_query = "//node()"; // Default second query
    }
    
    pugi::xpath_node_set second_set;
    try {
        second_set = root.select_nodes(second_query.c_str());
    } catch (...) {
        second_set = pugi::xpath_node_set();
    }
    
    // Also test with empty query
    pugi::xpath_node_set empty_query_set;
    try {
        empty_query_set = root.select_nodes("");
    } catch (...) {
        empty_query_set = pugi::xpath_node_set();
    }
    
    // Test with a child node if available
    pugi::xml_node child = root.first_child();
    if (child) {
        pugi::xpath_node_set child_set;
        try {
            child_set = child.select_nodes("*");
        } catch (...) {
            child_set = pugi::xpath_node_set();
        }
        (void)child_set;
    }
    
    // Create a manually constructed xpath_node_set to test different types
    static pugi::xpath_node empty_nodes[1];
    pugi::xpath_node_set manual_set(empty_nodes, empty_nodes, pugi::xpath_node_set::type_unsorted);
    
    // Call functions on manual set
    manual_set.size();
    manual_set.type();
    manual_set.empty();
    manual_set.first();
    manual_set.end();
    
    // Create another manual set with different type
    pugi::xpath_node_set manual_set_sorted(empty_nodes, empty_nodes, pugi::xpath_node_set::type_sorted);
    (void)manual_set_sorted;
    
    // Create set with reverse type
    pugi::xpath_node_set manual_set_reverse(empty_nodes, empty_nodes, pugi::xpath_node_set::type_sorted_reverse);
    (void)manual_set_reverse;
    
    return 0;
}