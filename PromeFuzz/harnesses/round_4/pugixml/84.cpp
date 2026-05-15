// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <vector>
#include <string>

static void test_xpath_parse_result() {
    // Test default constructor
    pugi::xpath_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    // Test with error state
    pugi::xpath_parse_result result2;
    // xpath_exception might not be available in all configurations
    // We'll test description directly
    const char* desc2 = result2.description();
    (void)desc2;
}

static void test_xml_parse_result() {
    pugi::xml_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    // Test with different states
    result.status = pugi::status_ok;
    const char* desc_ok = result.description();
    (void)desc_ok;
    
    result.status = pugi::status_file_not_found;
    const char* desc_err = result.description();
    (void)desc_err;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzer data (use first half for query)
        size_t query_size = size / 2;
        if (query_size > 0) {
            std::string query_str(reinterpret_cast<const char*>(data), query_size);
            
            // Test first overload with char* and variables
            pugi::xpath_variable_set vars;
            pugi::xpath_node node1 = root.select_single_node(query_str.c_str(), &vars);
            (void)node1;
            
            // Test second overload with xpath_query
            pugi::xpath_query query(query_str.c_str());
            pugi::xpath_node node2 = root.select_single_node(query);
            (void)node2;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* data, size_t size) {
    // Test xpath_parse_result::description
    test_xpath_parse_result();
    
    // Test xml_parse_result::description
    test_xml_parse_result();
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(data, size);
    
    return 0;
}