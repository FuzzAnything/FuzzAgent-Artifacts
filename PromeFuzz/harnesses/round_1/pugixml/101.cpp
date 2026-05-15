// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>

static void test_xpath_parse_result(const pugi::xpath_parse_result& parse_result) {
    // Test xpath_parse_result::description
    const char* description = parse_result.description();
    (void)description;
}

static void test_xml_parse_result(const pugi::xml_parse_result& parse_result) {
    // Test xml_parse_result::description
    const char* description = parse_result.description();
    (void)description;
}

static void test_select_single_node(pugi::xml_node& node, const pugi::char_t* query) {
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node result1 = node.select_single_node(query);
    (void)result1;
    
    // Test second overload: select_single_node(const xpath_query& query)
    // Create xpath_query object
    pugi::xpath_query xpath_query_obj(query);
    pugi::xpath_node result2 = node.select_single_node(xpath_query_obj);
    (void)result2;
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create dummy XML file for parsing
    const char* dummy_file = "./dummy_file";
    std::ofstream file(dummy_file, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    
    // Write fuzzer data to file
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file(dummy_file);
    
    // Test xml_parse_result::description
    test_xml_parse_result(parse_result);
    
    // If parsing succeeded, test node operations
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Create a simple XPath query from the data
        // Use a small portion of data to avoid overly long queries
        size_t query_size = Size > 100 ? 100 : Size;
        char query[101];
        memcpy(query, Data, query_size);
        query[query_size] = '\0';
        
        // Test select_single_node
        test_select_single_node(root, query);
    }
    
    // Test xpath_parse_result
    pugi::xpath_parse_result xpath_parse_res;
    xpath_parse_res.offset = Size % 100;
    
    // Test xpath_parse_result::description
    test_xpath_parse_result(xpath_parse_res);
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}