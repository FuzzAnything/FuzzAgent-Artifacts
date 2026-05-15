// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static void test_select_single_node(pugi::xml_node& node, const char* query, pugi::xpath_variable_set* vars) {
    // First overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node result1 = node.select_single_node(query, vars);
    (void)result1;
    
    // Second overload: select_single_node(const xpath_query& query)
    pugi::xpath_query xpath_query_obj(query);
    if (xpath_query_obj) {
        pugi::xpath_node result2 = node.select_single_node(xpath_query_obj);
        (void)result2;
    }
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result parse_result;
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_xml_parse_result_description() {
    pugi::xml_parse_result parse_result;
    const char* desc = parse_result.description();
    (void)desc;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document from fuzzer input
    if (Size == 0) return 0;
    
    std::string xml_content(reinterpret_cast<const char*>(Data), Size);
    
    // Create XML document and parse the content
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size());
    
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // If parsing succeeded, get the root node and test select_single_node
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzer input
        size_t query_size = Size > 100 ? 100 : Size;
        std::string xpath_query(reinterpret_cast<const char*>(Data), query_size);
        
        // Clean the query string to avoid null bytes
        for (char& c : xpath_query) {
            if (c == 0) c = ' ';
        }
        
        // Test select_single_node with null variables
        test_select_single_node(root, xpath_query.c_str(), nullptr);
        
        // Test select_single_node with empty variables set
        pugi::xpath_variable_set vars;
        test_select_single_node(root, xpath_query.c_str(), &vars);
    }
    
    // Test xpath_parse_result::description
    test_xpath_parse_result_description();
    
    return 0;
}