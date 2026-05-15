// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <vector>
#include <string>

static void test_xpath_parse_result_description(const pugi::xpath_parse_result& parse_result) {
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_xml_parse_result_description(const pugi::xml_parse_result& parse_result) {
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_select_single_node(pugi::xml_node& node, const char* query_str) {
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node result1 = node.select_single_node(query_str);
    (void)result1;
    
    // Test second overload: select_single_node(const xpath_query& query)
    pugi::xpath_query query(query_str);
    pugi::xpath_node result2 = node.select_single_node(query);
    (void)result2;
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create XML document from input data
    pugi::xml_document doc;
    pugi::xml_parse_result xml_result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    // Test xml_parse_result::description
    test_xml_parse_result_description(xml_result);
    
    // If parsing succeeded, test other functions
    if (xml_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query string from input data (use first 100 bytes or less)
        size_t query_size = Size > 100 ? 100 : Size;
        std::string query_str(reinterpret_cast<const char*>(Data), query_size);
        
        // Test select_single_node on root node
        test_select_single_node(root, query_str.c_str());
        
        // Test on child nodes if any exist
        for (pugi::xml_node child : root.children()) {
            test_select_single_node(child, query_str.c_str());
        }
    }
    
    // Create xpath_parse_result and test related functions
    pugi::xpath_parse_result xpath_result;
    
    // Set some values based on input data
    xpath_result.error = reinterpret_cast<const char*>(Data);
    xpath_result.offset = static_cast<ptrdiff_t>(Size % 1000);
    
    // Test xpath_parse_result::description
    test_xpath_parse_result_description(xpath_result);
    
    return 0;
}