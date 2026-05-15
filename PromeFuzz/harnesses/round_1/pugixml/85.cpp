// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

static void test_xpath_parse_result() {
    pugi::xpath_parse_result parse_result;
    parse_result.error = "Test error";
    parse_result.offset = 10;
    
    // Test pugi::xpath_parse_result::description
    const char* desc = parse_result.description();
    (void)desc;
    
    // Test with null error
    pugi::xpath_parse_result null_error_result;
    null_error_result.error = nullptr;
    null_error_result.offset = 0;
    const char* null_desc = null_error_result.description();
    (void)null_desc;
    
    // Test with empty error string
    pugi::xpath_parse_result empty_error_result;
    empty_error_result.error = "";
    empty_error_result.offset = 5;
    const char* empty_desc = empty_error_result.description();
    (void)empty_desc;
}

static void test_xml_parse_result() {
    pugi::xml_parse_result xml_result;
    xml_result.status = pugi::status_ok;
    
    // Test pugi::xml_parse_result::description
    const char* xml_desc = xml_result.description();
    (void)xml_desc;
    
    // Test with different status codes
    xml_result.status = pugi::status_file_not_found;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_io_error;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_out_of_memory;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_internal_error;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_unrecognized_tag;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_pi;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_comment;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_cdata;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_doctype;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_pcdata;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_start_element;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_attribute;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_bad_end_element;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_end_element_mismatch;
    xml_desc = xml_result.description();
    (void)xml_desc;
    
    xml_result.status = pugi::status_no_document_element;
    xml_desc = xml_result.description();
    (void)xml_desc;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse XML from fuzz data
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result.status == pugi::status_ok) {
        pugi::xml_node root = doc.root();
        
        // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
        pugi::xpath_node result1 = root.select_single_node("//*");
        (void)result1;
        
        // Test with different queries
        result1 = root.select_single_node("/node");
        (void)result1;
        
        result1 = root.select_single_node("child::*");
        (void)result1;
        
        result1 = root.select_single_node("//node()");
        (void)result1;
        
        result1 = root.select_single_node(".");
        (void)result1;
        
        result1 = root.select_single_node("..");
        (void)result1;
        
        result1 = root.select_single_node("*");
        (void)result1;
        
        result1 = root.select_single_node("@*");
        (void)result1;
        
        result1 = root.select_single_node("text()");
        (void)result1;
        
        result1 = root.select_single_node("comment()");
        (void)result1;
        
        result1 = root.select_single_node("processing-instruction()");
        (void)result1;
        
        // Test with empty string query
        result1 = root.select_single_node("");
        (void)result1;
        
        // Test second overload: select_single_node(const xpath_query& query)
        pugi::xpath_query query("//*");
        pugi::xpath_node result2 = root.select_single_node(query);
        (void)result2;
        
        // Test with different queries
        pugi::xpath_query query2("/node");
        result2 = root.select_single_node(query2);
        (void)result2;
        
        pugi::xpath_query query3("child::*");
        result2 = root.select_single_node(query3);
        (void)result2;
        
        pugi::xpath_query query4("//node()");
        result2 = root.select_single_node(query4);
        (void)result2;
        
        pugi::xpath_query query5(".");
        result2 = root.select_single_node(query5);
        (void)result2;
        
        pugi::xpath_query query6("..");
        result2 = root.select_single_node(query6);
        (void)result2;
        
        pugi::xpath_query query7("*");
        result2 = root.select_single_node(query7);
        (void)result2;
        
        pugi::xpath_query query8("@*");
        result2 = root.select_single_node(query8);
        (void)result2;
        
        pugi::xpath_query query9("text()");
        result2 = root.select_single_node(query9);
        (void)result2;
        
        pugi::xpath_query query10("comment()");
        result2 = root.select_single_node(query10);
        (void)result2;
        
        pugi::xpath_query query11("processing-instruction()");
        result2 = root.select_single_node(query11);
        (void)result2;
        
        // Test with empty xpath_query
        pugi::xpath_query empty_query("");
        result2 = root.select_single_node(empty_query);
        (void)result2;
        
        // Test with empty node
        pugi::xml_node empty_node;
        pugi::xpath_node empty_result = empty_node.select_single_node("//*");
        (void)empty_result;
        
        pugi::xpath_node empty_result2 = empty_node.select_single_node(query);
        (void)empty_result2;
    }
}

static void test_with_file(const uint8_t* data, size_t size) {
    // Write fuzz data to file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
        
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
        
        // Test xml_parse_result::description
        const char* desc = parse_result.description();
        (void)desc;
        
        if (parse_result.status == pugi::status_ok) {
            pugi::xml_node root = doc.root();
            
            // Test select_single_node with file-loaded document
            pugi::xpath_node result = root.select_single_node("//*");
            (void)result;
            
            pugi::xpath_query query("//node()");
            pugi::xpath_node result2 = root.select_single_node(query);
            (void)result2;
        }
        
        // Clean up
        std::remove("./dummy_file");
    }
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Test xpath_parse_result functions
    test_xpath_parse_result();
    
    // Test xml_parse_result::description
    test_xml_parse_result();
    
    // Test select_single_node functions
    test_select_single_node(data, size);
    
    // Test with file input
    test_with_file(data, size);
    
    // Test with various XML parsing scenarios
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Test loading from buffer
    parse_result = doc.load_buffer(data, size);
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    // Test loading from string
    std::string xml_string(reinterpret_cast<const char*>(data), size);
    parse_result = doc.load_string(xml_string.c_str());
    parse_desc = parse_result.description();
    (void)parse_desc;
    
    // Test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_root = empty_doc.root();
    pugi::xpath_node empty_xpath_result = empty_root.select_single_node("//*");
    (void)empty_xpath_result;
    
    return 0;
}