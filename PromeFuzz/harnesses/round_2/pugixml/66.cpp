// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    // Test with various error states
    result.status = pugi::status_ok;
    const char* desc1 = result.description();
    (void)desc1;
    
    result.status = pugi::status_file_not_found;
    const char* desc2 = result.description();
    (void)desc2;
    
    result.status = pugi::status_io_error;
    const char* desc3 = result.description();
    (void)desc3;
    
    result.status = pugi::status_out_of_memory;
    const char* desc4 = result.description();
    (void)desc4;
    
    result.status = pugi::status_internal_error;
    const char* desc5 = result.description();
    (void)desc5;
    
    // Test with unknown error code
    result.status = static_cast<pugi::xml_parse_status>(999);
    const char* desc6 = result.description();
    (void)desc6;
}

static void test_xpath_exception_functions(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create xpath_parse_result with error
    pugi::xpath_parse_result parse_result;
    parse_result.error = "Test error";
    parse_result.offset = static_cast<ptrdiff_t>(size);
    
    // Test xpath_exception constructor and methods
    // Note: xpath_exception might not be available if PUGIXML_NO_EXCEPTIONS is defined
    // We'll use conditional compilation to handle this
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        pugi::xpath_exception exc(parse_result);
        // Test result() function
        const pugi::xpath_parse_result& result = exc.result();
        (void)result;
        
        // Test what() function
        const char* what_msg = exc.what();
        (void)what_msg;
    } catch (...) {
        // Ignore any exceptions
    }
    #endif
}

static void test_xpath_parse_result_description(const uint8_t* data, size_t size) {
    pugi::xpath_parse_result result;
    
    // Test with error
    if (size > 0) {
        std::string error_str(reinterpret_cast<const char*>(data), std::min(size, static_cast<size_t>(100)));
        result.error = error_str.c_str();
        result.offset = static_cast<ptrdiff_t>(size);
    } else {
        result.error = "Test error";
        result.offset = 0;
    }
    
    const char* desc1 = result.description();
    (void)desc1;
    
    // Test without error
    result.error = nullptr;
    result.offset = 0;
    const char* desc2 = result.description();
    (void)desc2;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create XML document from fuzzer data
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (!parse_result) {
        // If parsing fails, create a minimal valid document
        const char* minimal_xml = "<root><child>text</child></root>";
        doc.load_string(minimal_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) return;
    
    // Create XPath query from fuzzer data (limited to 100 chars for safety)
    std::string xpath_str;
    if (size > 0) {
        xpath_str = std::string(reinterpret_cast<const char*>(data), 
                               std::min(size, static_cast<size_t>(100)));
    } else {
        xpath_str = "//child";
    }
    
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    try {
        pugi::xpath_node node1 = root.select_single_node(xpath_str.c_str(), nullptr);
        (void)node1;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test second overload: select_single_node(const xpath_query& query)
    try {
        pugi::xpath_query query(xpath_str.c_str());
        pugi::xpath_node node2 = root.select_single_node(query);
        (void)node2;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with empty query
    try {
        pugi::xpath_node node3 = root.select_single_node("");
        (void)node3;
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with valid simple query
    try {
        pugi::xpath_node node4 = root.select_single_node("//*");
        (void)node4;
    } catch (...) {
        // Ignore exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* data, size_t size) {
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // Test xpath_exception::result and xpath_exception::what
    test_xpath_exception_functions(data, size);
    
    // Test xpath_parse_result::description
    test_xpath_parse_result_description(data, size);
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(data, size);
    
    return 0;
}