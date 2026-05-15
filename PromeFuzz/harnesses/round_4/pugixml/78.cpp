// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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
#include <fstream>
#include <vector>

static void test_xpath_parse_result_description() {
    // Test with default constructor (failed state)
    pugi::xpath_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    // Test with custom error
    pugi::xpath_parse_result result2;
    result2.error = "Test error";
    result2.offset = 10;
    const char* desc2 = result2.description();
    (void)desc2;
    
    // Test with null error (success case)
    pugi::xpath_parse_result result3;
    result3.error = nullptr;
    result3.offset = 0;
    const char* desc3 = result3.description();
    (void)desc3;
}

static void test_xml_parse_result_description() {
    // Create various parse results with different states
    pugi::xml_parse_result result1;
    result1.status = pugi::status_ok;
    const char* desc1 = result1.description();
    (void)desc1;
    
    pugi::xml_parse_result result2;
    result2.status = pugi::status_file_not_found;
    const char* desc2 = result2.description();
    (void)desc2;
    
    pugi::xml_parse_result result3;
    result3.status = pugi::status_io_error;
    const char* desc3 = result3.description();
    (void)desc3;
    
    pugi::xml_parse_result result4;
    result4.status = pugi::status_out_of_memory;
    const char* desc4 = result4.description();
    (void)desc4;
    
    pugi::xml_parse_result result5;
    result5.status = pugi::status_internal_error;
    const char* desc5 = result5.description();
    (void)desc5;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
        if (size > 0) {
            // Use first byte as simple query character
            char query_str[2] = {static_cast<char>(data[0] % 128), '\0'};
            pugi::xpath_node node1 = root.select_single_node(query_str);
            (void)node1;
            
            // Test with null variables
            pugi::xpath_node node2 = root.select_single_node(query_str, nullptr);
            (void)node2;
        }
        
        // Test second overload: select_single_node(const xpath_query& query)
        if (size > 1) {
            // Create xpath_query from data
            std::vector<char> query_data(data, data + std::min(size, static_cast<size_t>(100)));
            query_data.push_back('\0');
            
            try {
                pugi::xpath_query xquery(query_data.data());
                pugi::xpath_node node3 = root.select_single_node(xquery);
                (void)node3;
            } catch (...) {
                // Catch any exceptions
            }
        }
    }
    
    // Also test with empty node
    pugi::xml_node empty_node;
    pugi::xpath_node empty_result = empty_node.select_single_node("test");
    (void)empty_result;
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    // Test xpath_parse_result::description
    test_xpath_parse_result_description();
    
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(Data, Size);
    
    // Additional test: create XML file and parse it
    if (Size > 0) {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
            
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file("./dummy_file");
            const char* desc = result.description();
            (void)desc;
            
            // Remove the file
            std::remove("./dummy_file");
        }
    }
    
    // Test with empty xpath_parse_result
    pugi::xpath_parse_result empty_xpath_result;
    const char* empty_desc = empty_xpath_result.description();
    (void)empty_desc;
    
    // Create xpath_node from xml_node
    pugi::xml_node node;
    pugi::xpath_node xnode(node);
    (void)xnode;
    
    // Create xml_attribute
    pugi::xml_attribute attr;
    (void)attr;
    
    // Create xml_node from internal pointer (nullptr for empty node)
    pugi::xml_node node_from_ptr(nullptr);
    (void)node_from_ptr;
    
    return 0;
}