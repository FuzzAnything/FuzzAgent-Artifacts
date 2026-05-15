// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>

static void test_xpath_parse_result() {
    // Test with default (failed) state
    pugi::xpath_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    // Test with custom error offset
    pugi::xpath_parse_result result2;
    result2.error = "Test error";
    result2.offset = 42;
    const char* desc2 = result2.description();
    (void)desc2;
}

static void test_xml_node_select_single_node(const uint8_t* data, size_t size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result.status == pugi::status_ok) {
        pugi::xml_node root = doc.root();
        
        // Test first overload with const char* query
        pugi::xpath_node node1 = root.select_single_node("//*");
        (void)node1;
        
        // Test with different query
        pugi::xpath_node node2 = root.select_single_node("/*");
        (void)node2;
        
        // Test second overload with xpath_query object
        pugi::xpath_query query1("//node");
        pugi::xpath_node node3 = root.select_single_node(query1);
        (void)node3;
        
        pugi::xpath_query query2("child::*");
        pugi::xpath_node node4 = root.select_single_node(query2);
        (void)node4;
        
        // Test with empty query
        pugi::xpath_query query3("");
        pugi::xpath_node node5 = root.select_single_node(query3);
        (void)node5;
    }
}

static void test_xml_parse_result() {
    // Test with default (failed) state
    pugi::xml_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1;
    
    // Test with different status codes
    pugi::xml_parse_result result2;
    result2.status = pugi::status_ok;
    const char* desc2 = result2.description();
    (void)desc2;
    
    pugi::xml_parse_result result3;
    result3.status = pugi::status_file_not_found;
    const char* desc3 = result3.description();
    (void)desc3;
    
    pugi::xml_parse_result result4;
    result4.status = pugi::status_io_error;
    const char* desc4 = result4.description();
    (void)desc4;
    
    pugi::xml_parse_result result5;
    result5.status = pugi::status_out_of_memory;
    const char* desc5 = result5.description();
    (void)desc5;
    
    pugi::xml_parse_result result6;
    result6.status = pugi::status_internal_error;
    const char* desc6 = result6.description();
    (void)desc6;
    
    pugi::xml_parse_result result7;
    result7.status = pugi::status_unrecognized_tag;
    const char* desc7 = result7.description();
    (void)desc7;
    
    pugi::xml_parse_result result8;
    result8.status = pugi::status_bad_pi;
    const char* desc8 = result8.description();
    (void)desc8;
    
    pugi::xml_parse_result result9;
    result9.status = pugi::status_bad_comment;
    const char* desc9 = result9.description();
    (void)desc9;
    
    pugi::xml_parse_result result10;
    result10.status = pugi::status_bad_cdata;
    const char* desc10 = result10.description();
    (void)desc10;
    
    pugi::xml_parse_result result11;
    result11.status = pugi::status_bad_doctype;
    const char* desc11 = result11.description();
    (void)desc11;
    
    pugi::xml_parse_result result12;
    result12.status = pugi::status_bad_pcdata;
    const char* desc12 = result12.description();
    (void)desc12;
    
    pugi::xml_parse_result result13;
    result13.status = pugi::status_bad_start_element;
    const char* desc13 = result13.description();
    (void)desc13;
    
    pugi::xml_parse_result result14;
    result14.status = pugi::status_bad_attribute;
    const char* desc14 = result14.description();
    (void)desc14;
    
    pugi::xml_parse_result result15;
    result15.status = pugi::status_bad_end_element;
    const char* desc15 = result15.description();
    (void)desc15;
    
    pugi::xml_parse_result result16;
    result16.status = pugi::status_end_element_mismatch;
    const char* desc16 = result16.description();
    (void)desc16;
    
    pugi::xml_parse_result result17;
    result17.status = pugi::status_no_document_element;
    const char* desc17 = result17.description();
    (void)desc17;
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* data, size_t size) {
    // Test all target API functions
    
    // 1. Test xpath_parse_result::description
    test_xpath_parse_result();
    
    // 2. Test xml_node::select_single_node (both overloads)
    test_xml_node_select_single_node(data, size);
    
    // 3. Test xml_parse_result::description
    test_xml_parse_result();
    
    // Also test with file input
    if (size > 0) {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(data), size);
            file.close();
            
            // Parse from file
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file("./dummy_file");
            const char* desc = result.description();
            (void)desc;
            
            // Test select_single_node on file-loaded document
            if (result.status == pugi::status_ok) {
                pugi::xml_node root = doc.root();
                pugi::xpath_node node = root.select_single_node("//*");
                (void)node;
                
                pugi::xpath_query query("/*");
                pugi::xpath_node node2 = root.select_single_node(query);
                (void)node2;
            }
        }
    }
    
    return 0;
}