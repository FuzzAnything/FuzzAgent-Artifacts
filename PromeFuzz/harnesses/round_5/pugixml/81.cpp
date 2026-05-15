// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <string>

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create XML document
    pugi::xml_document doc;
    std::string xml_content(reinterpret_cast<const char*>(data), size);
    
    // Try to parse XML
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str());
    
    // Test xml_parse_result::description
    static_cast<void>(parse_result.description());
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from part of the data
        size_t query_size = size > 100 ? 100 : size;
        std::string xpath_query(reinterpret_cast<const char*>(data), query_size);
        
        // Create variable set
        pugi::xpath_variable_set vars;
        
        // Test select_single_node (deprecated but we're testing it)
        pugi::xpath_node result = root.select_single_node(xpath_query.c_str(), &vars);
        
        // Also test without variables
        pugi::xpath_node result2 = root.select_single_node(xpath_query.c_str());
    }
}

static void test_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    std::string xpath_str(reinterpret_cast<const char*>(data), size);
    
    // Create XPath query
    pugi::xpath_query query(xpath_str.c_str());
    
    // Test xpath_query::result
    const pugi::xpath_parse_result& parse_result = query.result();
    
    // Test xpath_parse_result::description
    static_cast<void>(parse_result.description());
    
    // Create XML document to test evaluation
    pugi::xml_document doc;
    doc.load_string("<root><element>test</element></root>");
    
    // Try to evaluate the query
    pugi::xpath_node_set nodes = doc.select_nodes(query);
}

static void test_file_operations(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Write data to dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    
    // Try to load from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Test description
    static_cast<void>(result.description());
    
    // Clean up
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* data, size_t size) {
    // Test all target functions with different approaches
    
    // Test 1: XML parsing and select_single_node
    test_select_single_node(data, size);
    
    // Test 2: XPath query compilation and evaluation
    test_xpath_query(data, size);
    
    // Test 3: File operations
    test_file_operations(data, size);
    
    // Test with empty/default constructed objects
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr;
    pugi::xpath_variable_set empty_vars;
    
    // Try select_single_node on empty node
    std::string dummy_query = "//test";
    pugi::xpath_node empty_result = empty_node.select_single_node(dummy_query.c_str());
    
    // Test move constructor for xpath_variable_set
    pugi::xpath_variable_set moved_vars(std::move(empty_vars));
    
    return 0;
}