// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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
#include <string>

static void test_xml_parse_result(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Load XML from fuzzer input
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    // Test xml_parse_result::description
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_xpath_parse_result() {
    // Create a default xpath_parse_result (failed state)
    pugi::xpath_parse_result parse_result;
    
    // Test xpath_parse_result::description on default object
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse XML from fuzzer input
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzer data (use first 256 bytes max)
        size_t query_size = size > 256 ? 256 : size;
        std::string query_str(reinterpret_cast<const char*>(data), query_size);
        
        // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
        pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
        (void)node1;
        
        // Test second overload: select_single_node(const xpath_query& query)
        pugi::xpath_query query(query_str.c_str());
        pugi::xpath_node node2 = root.select_single_node(query);
        (void)node2;
    }
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test xml_parse_result::description
    test_xml_parse_result(data, size);
    
    // Test xpath_parse_result::description
    test_xpath_parse_result();
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(data, size);
    
    return 0;
}