// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
#include "pugixml.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

static void test_xpath_parse_result(const pugi::xpath_parse_result& parse_result) {
    // Test xpath_parse_result::description()
    const char* desc = parse_result.description();
    (void)desc; // Use to avoid unused variable warning
}

static void test_xml_parse_result(const pugi::xml_parse_result& parse_result) {
    // Test xml_parse_result::description()
    const char* desc = parse_result.description();
    (void)desc; // Use to avoid unused variable warning
}

static void test_select_single_node(pugi::xml_node& node, const char* query) {
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node xnode1 = node.select_single_node(query);
    (void)xnode1; // Use to avoid unused variable warning
    
    // Test second overload: select_single_node(const xpath_query& query)
    pugi::xpath_query xquery(query);
    pugi::xpath_node xnode2 = node.select_single_node(xquery);
    (void)xnode2; // Use to avoid unused variable warning
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzed data
    pugi::xml_document doc;
    pugi::xml_parse_result xml_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description()
    test_xml_parse_result(xml_result);
    
    // If parsing succeeded, try to use the document
    if (xml_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create a simple XPath query from the data
        // Use first 256 bytes or less for query string
        size_t query_size = Size > 256 ? 256 : Size;
        char query[257];
        memcpy(query, Data, query_size);
        query[query_size] = '\0'; // Null-terminate
        
        // Test select_single_node with the query
        test_select_single_node(root, query);
        
        // Also test with empty node
        pugi::xml_node empty_node;
        test_select_single_node(empty_node, query);
    }
    
    // Test xpath_parse_result with default constructor
    pugi::xpath_parse_result xpath_result;
    test_xpath_parse_result(xpath_result);
    
    // Test xpath_parse_result with custom error
    pugi::xpath_parse_result custom_result;
    custom_result.error = "Test error";
    custom_result.offset = 0;
    test_xpath_parse_result(custom_result);
    
    // Test with empty XML document
    pugi::xml_document empty_doc;
    pugi::xml_parse_result empty_result = empty_doc.load_string("");
    test_xml_parse_result(empty_result);
    
    // Test with a simple XML string
    const char* simple_xml = "<root><child>text</child></root>";
    pugi::xml_document simple_doc;
    pugi::xml_parse_result simple_result = simple_doc.load_string(simple_xml);
    test_xml_parse_result(simple_result);
    
    if (simple_result) {
        pugi::xml_node simple_root = simple_doc.root();
        test_select_single_node(simple_root, "//child");
        test_select_single_node(simple_root, "invalid[xpath]");
    }
    
    return 0;
}