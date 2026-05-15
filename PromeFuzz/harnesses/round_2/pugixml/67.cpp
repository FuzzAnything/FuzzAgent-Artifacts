// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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
#include <fstream>

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    result.status = pugi::status_ok;
    const char* desc = result.description();
    (void)desc;
    
    // Test with different status values
    result.status = pugi::status_file_not_found;
    result.description();
    
    result.status = pugi::status_io_error;
    result.description();
    
    result.status = pugi::status_out_of_memory;
    result.description();
    
    result.status = pugi::status_internal_error;
    result.description();
    
    result.status = pugi::status_unrecognized_tag;
    result.description();
    
    result.status = pugi::status_bad_pi;
    result.description();
    
    result.status = pugi::status_bad_comment;
    result.description();
    
    result.status = pugi::status_bad_cdata;
    result.description();
    
    result.status = pugi::status_bad_doctype;
    result.description();
    
    result.status = pugi::status_bad_pcdata;
    result.description();
    
    result.status = pugi::status_bad_start_element;
    result.description();
    
    result.status = pugi::status_bad_attribute;
    result.description();
    
    result.status = pugi::status_bad_end_element;
    result.description();
    
    result.status = pugi::status_end_element_mismatch;
    result.description();
    
    result.status = pugi::status_append_invalid_root;
    result.description();
    
    result.status = pugi::status_no_document_element;
    result.description();
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    // Test with error set
    result.error = "Test error";
    result.offset = 10;
    result.description();
}

static void test_xpath_exception_functions(const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    // Create an xpath_parse_result with error
    pugi::xpath_parse_result parse_result;
    parse_result.error = "XPath parsing error";
    parse_result.offset = static_cast<ptrdiff_t>(size);
    
    // Check if xpath_exception is available by checking if PUGIXML_NO_EXCEPTIONS is not defined
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        throw pugi::xpath_exception(parse_result);
    } catch (const pugi::xpath_exception& e) {
        // Test result() function
        const pugi::xpath_parse_result& caught_result = e.result();
        (void)caught_result;
        
        // Test what() function
        const char* what_msg = e.what();
        (void)what_msg;
    }
    #endif
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    // Create a dummy XML document
    pugi::xml_document doc;
    const char* xml_content = "<root><element>text</element><element2 attr=\"value\"/></root>";
    pugi::xml_parse_result parse_result = doc.load_string(xml_content);
    
    if (!parse_result) return;
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) return;
    
    // Test first overload: select_single_node with char_t* query
    {
        // Create a query string from input data (limit to reasonable length)
        size_t query_len = size > 100 ? 100 : size;
        std::string query_str(reinterpret_cast<const char*>(data), query_len);
        
        // Test with null variables
        pugi::xpath_node result = root.select_single_node(query_str.c_str(), nullptr);
        (void)result;
        
        // Test with empty variable set
        pugi::xpath_variable_set vars;
        result = root.select_single_node(query_str.c_str(), &vars);
        (void)result;
        
        // Test with some common XPath queries
        const char* test_queries[] = {
            "//element",
            "/root",
            "element",
            ".",
            "*",
            "@attr",
            "text()",
            ""
        };
        
        for (const char* test_query : test_queries) {
            result = root.select_single_node(test_query, nullptr);
            (void)result;
        }
    }
    
    // Test second overload: select_single_node with xpath_query
    {
        // Create xpath_query from input data
        size_t query_len = size > 100 ? 100 : size;
        std::string query_str(reinterpret_cast<const char*>(data), query_len);
        
        pugi::xpath_query query(query_str.c_str());
        pugi::xpath_node result = root.select_single_node(query);
        (void)result;
        
        // Test with valid queries
        const char* valid_queries[] = {
            "//element",
            "/root",
            "element",
            "."
        };
        
        for (const char* valid_query : valid_queries) {
            pugi::xpath_query valid_xpath_query(valid_query);
            result = root.select_single_node(valid_xpath_query);
            (void)result;
        }
    }
}

static void test_with_file(const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    // Write data to dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    
    // Try to load XML from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Test description on the parse result
    const char* desc = result.description();
    (void)desc;
    
    // If parsing succeeded, test select_single_node on the document
    if (result) {
        pugi::xml_node root = doc.root();
        if (root) {
            // Test with a simple query
            pugi::xpath_node node = root.select_single_node("//*", nullptr);
            (void)node;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // Test xpath_parse_result::description
    test_xpath_parse_result_description();
    
    // Test xpath_exception::result and xpath_exception::what
    test_xpath_exception_functions(Data, Size);
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(Data, Size);
    
    // Test with file input
    test_with_file(Data, Size);
    
    return 0;
}