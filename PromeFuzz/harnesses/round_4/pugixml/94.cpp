// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <fstream>

static void test_xpath_parse_result() {
    // Test xpath_parse_result::description
    pugi::xpath_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    pugi::xpath_parse_result result2;
    result2.error = "Test error";
    result2.offset = 10;
    const char* desc2 = result2.description();
    (void)desc2;
}

static void test_xml_parse_result() {
    // Test xml_parse_result::description
    pugi::xml_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    pugi::xml_parse_result result2;
    result2.status = pugi::status_ok;
    const char* desc2 = result2.description();
    (void)desc2;
    
    pugi::xml_parse_result result3;
    result3.status = pugi::status_file_not_found;
    const char* desc3 = result3.description();
    (void)desc3;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    // Create XML document from fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    if (parse_result) {
        // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
        pugi::xml_node root = doc.document_element();
        if (root) {
            // Create XPath query from data (use first 100 bytes or less)
            size_t query_size = size > 100 ? 100 : size;
            std::string query_str(reinterpret_cast<const char*>(data), query_size);
            
            pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
            (void)node1;
            
            // Test with variables
            pugi::xpath_variable_set vars;
            pugi::xpath_node node2 = root.select_single_node(query_str.c_str(), &vars);
            (void)node2;
            
            // Test second overload: select_single_node(const xpath_query& query)
            pugi::xpath_query query(query_str.c_str());
            if (query) {
                pugi::xpath_node node3 = root.select_single_node(query);
                (void)node3;
            }
        }
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xpath_node empty_result = empty_node.select_single_node("test");
        (void)empty_result;
    }
}

static void test_with_file(const uint8_t* data, size_t size) {
    // Write data to file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
        
        // Parse from file
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file("./dummy_file");
        const char* desc = result.description();
        (void)desc;
        
        if (result) {
            // Test select_single_node with file data
            pugi::xml_node root = doc.document_element();
            if (root) {
                pugi::xpath_node node = root.select_single_node("//node");
                (void)node;
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test all target functions except xpath_exception which may not be available
    test_xpath_parse_result();
    test_xml_parse_result();
    test_select_single_node(data, size);
    test_with_file(data, size);
    
    return 0;
}