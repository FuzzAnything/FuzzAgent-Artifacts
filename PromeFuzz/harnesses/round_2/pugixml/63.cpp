// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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
#include <fstream>

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    // Test default constructed result
    const char* desc = result.description();
    (void)desc;
    
    // Test with various error states
    result.status = pugi::status_ok;
    desc = result.description();
    
    result.status = pugi::status_file_not_found;
    desc = result.description();
    
    result.status = pugi::status_io_error;
    desc = result.description();
    
    result.status = pugi::status_out_of_memory;
    desc = result.description();
    
    result.status = pugi::status_internal_error;
    desc = result.description();
    
    result.status = pugi::status_unrecognized_tag;
    desc = result.description();
    
    result.status = pugi::status_bad_pi;
    desc = result.description();
    
    result.status = pugi::status_bad_comment;
    desc = result.description();
    
    result.status = pugi::status_bad_cdata;
    desc = result.description();
    
    result.status = pugi::status_bad_doctype;
    desc = result.description();
    
    result.status = pugi::status_bad_pcdata;
    desc = result.description();
    
    result.status = pugi::status_bad_start_element;
    desc = result.description();
    
    result.status = pugi::status_bad_attribute;
    desc = result.description();
    
    result.status = pugi::status_bad_end_element;
    desc = result.description();
    
    result.status = pugi::status_end_element_mismatch;
    desc = result.description();
    
    result.status = pugi::status_append_invalid_root;
    desc = result.description();
    
    result.status = pugi::status_no_document_element;
    desc = result.description();
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    // Test default constructed result
    const char* desc = result.description();
    (void)desc;
    
    // Test with error set
    result.error = "Test error";
    result.offset = 10;
    desc = result.description();
    
    // Test with null error (success case)
    result.error = nullptr;
    result.offset = 0;
    desc = result.description();
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the fuzzer input as XML
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Test first overload with char_t* query
        pugi::xpath_variable_set vars;
        pugi::xpath_node node1 = root.select_single_node("//node");
        (void)node1;
        
        // Test with variables
        pugi::xpath_node node2 = root.select_single_node("//node", &vars);
        (void)node2;
        
        // Test with empty query
        pugi::xpath_node node3 = root.select_single_node("");
        (void)node3;
        
        // Test second overload with xpath_query
        // Note: xpath_query constructor can throw xpath_exception
        // We'll use try-catch but only if exceptions are enabled
        #ifndef PUGIXML_NO_EXCEPTIONS
        try {
            pugi::xpath_query query("//node");
            pugi::xpath_node node4 = root.select_single_node(query);
            (void)node4;
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test with more complex query
        try {
            pugi::xpath_query query2("/*[@attr='value']");
            pugi::xpath_node node5 = root.select_single_node(query2);
            (void)node5;
        } catch (...) {
            // Ignore exceptions
        }
        #else
        // If exceptions are disabled, we can still create queries that are valid
        pugi::xpath_query query("//node");
        pugi::xpath_node node4 = root.select_single_node(query);
        (void)node4;
        
        pugi::xpath_query query2("/*[@attr='value']");
        pugi::xpath_node node5 = root.select_single_node(query2);
        (void)node5;
        #endif
    }
}

static void test_xpath_exception(const uint8_t* data, size_t size) {
    // Create a file with fuzzer data
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
    
    // Note: xpath_exception is only available when exceptions are enabled
    #ifndef PUGIXML_NO_EXCEPTIONS
    // Try to create an invalid xpath_query to trigger exception
    try {
        // Use data to create query string (limit to 100 chars to avoid huge strings)
        size_t query_len = size > 100 ? 100 : size;
        std::string query_str(reinterpret_cast<const char*>(data), query_len);
        
        pugi::xpath_query query(query_str.c_str());
        // If we get here, query compiled successfully
        (void)query;
    } catch (const pugi::xpath_exception& e) {
        // Test xpath_exception::result()
        const pugi::xpath_parse_result& result = e.result();
        (void)result;
        
        // Test xpath_exception::what()
        const char* what_msg = e.what();
        (void)what_msg;
    }
    
    // Test with specific invalid queries
    try {
        pugi::xpath_query query("invalid[query");
        (void)query;
    } catch (const pugi::xpath_exception& e) {
        const pugi::xpath_parse_result& result = e.result();
        (void)result;
        const char* what_msg = e.what();
        (void)what_msg;
    }
    
    try {
        pugi::xpath_query query("//");
        (void)query;
    } catch (const pugi::xpath_exception& e) {
        const pugi::xpath_parse_result& result = e.result();
        (void)result;
        const char* what_msg = e.what();
        (void)what_msg;
    }
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* data, size_t size) {
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // Test xpath_parse_result::description
    test_xpath_parse_result_description();
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(data, size);
    
    // Test xpath_exception::result and xpath_exception::what
    test_xpath_exception(data, size);
    
    return 0;
}