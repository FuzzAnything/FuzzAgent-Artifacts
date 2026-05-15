// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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
#include <cstring>
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    write_dummy_file(Data, Size);
    
    // Initialize required objects
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    pugi::xpath_variable_set vars;
    pugi::xml_node empty_node;
    pugi::xpath_node empty_xpath_node;
    
    // Try parsing from memory buffer
    parse_result = doc.load_buffer(Data, Size);
    
    // If parsing succeeded, get root node
    pugi::xml_node root = doc.root();
    
    // Create XPath query from input data (use first part of input as query string)
    const char* query_str = reinterpret_cast<const char*>(Data);
    size_t query_len = Size;
    if (query_len > 0) {
        // Ensure null termination for query string
        char* safe_query = new char[query_len + 1];
        memcpy(safe_query, query_str, query_len);
        safe_query[query_len] = '\0';
        
        // First overload: select_single_node with char_t* and xpath_variable_set*
        pugi::xpath_node result1 = root.select_single_node(safe_query, &vars);
        
        // Second overload: select_single_node with xpath_query object
        pugi::xpath_query query_obj(safe_query);
        pugi::xpath_node result2 = root.select_single_node(query_obj);
        
        // Call node() on both results
        pugi::xml_node node1 = result1.node();
        pugi::xml_node node2 = result2.node();
        
        // Also call node() on empty xpath_node
        pugi::xml_node node3 = empty_xpath_node.node();
        
        // Create another xpath_node from empty xml_node and call node()
        pugi::xpath_node xnode_from_empty(empty_node);
        pugi::xml_node node4 = xnode_from_empty.node();
        
        delete[] safe_query;
    } else {
        // Handle empty input case
        pugi::xpath_node result1 = root.select_single_node("", &vars);
        pugi::xpath_query query_obj("");
        pugi::xpath_node result2 = root.select_single_node(query_obj);
        
        pugi::xml_node node1 = result1.node();
        pugi::xml_node node2 = result2.node();
        pugi::xml_node node3 = empty_xpath_node.node();
        
        pugi::xpath_node xnode_from_empty(empty_node);
        pugi::xml_node node4 = xnode_from_empty.node();
    }
    
    // Try parsing from file as well
    pugi::xml_document doc2;
    parse_result = doc2.load_file("./dummy_file");
    if (parse_result) {
        pugi::xml_node root2 = doc2.root();
        const char* test_query = "//node";
        
        pugi::xpath_node file_result1 = root2.select_single_node(test_query, &vars);
        pugi::xpath_query file_query_obj(test_query);
        pugi::xpath_node file_result2 = root2.select_single_node(file_query_obj);
        
        pugi::xml_node file_node1 = file_result1.node();
        pugi::xml_node file_node2 = file_result2.node();
    }
    
    return 0;
}