// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document for testing
    const char* xml_content = "<root><child attr=\"value\">text</child><child2>more text</child2></root>";
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content);
    
    if (!parse_result) {
        return 0;
    }
    
    pugi::xml_node root = doc.root().first_child();
    
    // Prepare XPath query from fuzzer input
    std::string query_str;
    if (Size > 0) {
        query_str.assign(reinterpret_cast<const char*>(Data), Size);
    } else {
        query_str = "root";
    }
    
    // First overload: select_single_node with xpath_query object
    pugi::xpath_query query(query_str.c_str());
    pugi::xpath_node result1 = root.select_single_node(query);
    
    // Second overload: select_single_node with char_t* and optional variables
    pugi::xpath_variable_set vars;
    pugi::xpath_node result2 = root.select_single_node(query_str.c_str(), &vars);
    
    // Call node() on both results
    pugi::xml_node node1 = result1.node();
    pugi::xml_node node2 = result2.node();
    
    // Try to use the nodes to explore more states
    if (node1) {
        // Do something with node1
        const char* name = node1.name();
        const char* value = node1.value();
        (void)name;
        (void)value;
    }
    
    if (node2) {
        // Do something with node2
        const char* name = node2.name();
        const char* value = node2.value();
        (void)name;
        (void)value;
    }
    
    return 0;
}