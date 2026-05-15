// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Use the fuzz data to create an XML document string
    std::string xml_content(reinterpret_cast<const char*>(Data), Size);
    
    // Parse the XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str());
    
    // If parsing failed, use an empty document
    if (!parse_result) {
        doc.reset();
    }
    
    // Get the root node (or empty node if document is empty)
    pugi::xml_node root = doc.root();
    
    // Create XPath query strings from the fuzz data
    // Use a portion of the data for the query
    size_t query_size = Size > 100 ? 100 : Size;
    std::string query_str(reinterpret_cast<const char*>(Data), query_size);
    
    // Remove null characters from the query string to avoid early termination
    for (char& c : query_str) {
        if (c == '\0') c = ' ';
    }
    
    try {
        // First overload: using xpath_query object
        pugi::xpath_query query_obj(query_str.c_str());
        pugi::xpath_node result1 = root.select_single_node(query_obj);
        
        // Second overload: using string directly
        pugi::xpath_node result2 = root.select_single_node(query_str.c_str());
        
        // Call node() on both results
        pugi::xml_node node1 = result1.node();
        pugi::xml_node node2 = result2.node();
        
        // Try to use the nodes in some way to explore more states
        if (node1) {
            std::string name1 = node1.name();
            std::string value1 = node1.value();
        }
        
        if (node2) {
            std::string name2 = node2.name();
            std::string value2 = node2.value();
        }
        
        // Try with different nodes from the document
        if (root) {
            for (pugi::xml_node child : root.children()) {
                pugi::xpath_query query_obj2(query_str.c_str());
                pugi::xpath_node child_result1 = child.select_single_node(query_obj2);
                pugi::xpath_node child_result2 = child.select_single_node(query_str.c_str());
                
                pugi::xml_node child_node1 = child_result1.node();
                pugi::xml_node child_node2 = child_result2.node();
            }
        }
        
    } catch (...) {
        // Catch all exceptions - including any xpath exceptions
    }
    
    // Try with empty nodes
    pugi::xml_node empty_node;
    pugi::xpath_query empty_query("");
    pugi::xpath_node empty_result1 = empty_node.select_single_node(empty_query);
    pugi::xpath_node empty_result2 = empty_node.select_single_node("");
    
    pugi::xml_node empty_node1 = empty_result1.node();
    pugi::xml_node empty_node2 = empty_result2.node();
    
    // Try with xpath_variable_set (null for now)
    pugi::xpath_variable_set* vars = nullptr;
    pugi::xpath_node result_with_vars = root.select_single_node(query_str.c_str(), vars);
    pugi::xml_node node_with_vars = result_with_vars.node();
    
    return 0;
}