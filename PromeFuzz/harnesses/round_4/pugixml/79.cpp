// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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

static void test_xpath_parse_result_description() {
    // Test with default constructor (failed state)
    pugi::xpath_parse_result result1;
    const char* desc1 = result1.description();
    (void)desc1; // Use to avoid unused variable warning
    
    // Test with custom error state
    pugi::xpath_parse_result result2;
    result2.error = "Test error";
    result2.offset = 42;
    const char* desc2 = result2.description();
    (void)desc2;
    
    // Test with null error (should return "No error")
    pugi::xpath_parse_result result3;
    result3.error = nullptr;
    result3.offset = 0;
    const char* desc3 = result3.description();
    (void)desc3;
}

static void test_xml_parse_result_description() {
    // Test various parse result states
    pugi::xml_parse_result result;
    
    // Test default state
    const char* desc1 = result.description();
    (void)desc1;
    
    // Test with different status codes
    result.status = pugi::status_ok;
    const char* desc2 = result.description();
    (void)desc2;
    
    result.status = pugi::status_file_not_found;
    const char* desc3 = result.description();
    (void)desc3;
    
    result.status = pugi::status_io_error;
    const char* desc4 = result.description();
    (void)desc4;
    
    result.status = pugi::status_out_of_memory;
    const char* desc5 = result.description();
    (void)desc5;
    
    result.status = pugi::status_internal_error;
    const char* desc6 = result.description();
    (void)desc6;
}

static void test_select_single_node(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document for testing
    const char* xml_content = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "<node1 attr=\"value1\">Text1</node1>"
        "<node2 attr=\"value2\">Text2</node2>"
        "<node3>Text3</node3>"
        "</root>";
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content);
    
    if (parse_result) {
        pugi::xml_node root = doc.root().first_child();
        
        if (Size > 0) {
            // Create XPath query from fuzzed data
            std::string query_str(reinterpret_cast<const char*>(Data), 
                                 std::min(Size, static_cast<size_t>(256)));
            
            // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            // Note: xpath_exception may not be available if exceptions are disabled
            // We'll use a try-catch block but handle the case where the exception type doesn't exist
            try {
                pugi::xpath_node result1 = root.select_single_node(query_str.c_str());
                (void)result1;
                
                // Test with xpath_variable_set
                pugi::xpath_variable_set vars;
                pugi::xpath_node result2 = root.select_single_node(query_str.c_str(), &vars);
                (void)result2;
            } catch (...) {
                // Catch any exception that might be thrown
                // We can't test xpath_exception::what() or xpath_exception::result() 
                // if the exception type isn't available
            }
            
            // Test second overload: select_single_node(const xpath_query& query)
            try {
                pugi::xpath_query query(query_str.c_str());
                pugi::xpath_node result3 = root.select_single_node(query);
                (void)result3;
            } catch (...) {
                // Catch any exception that might be thrown
            }
        }
        
        // Also test with some fixed queries
        try {
            pugi::xpath_node fixed_result1 = root.select_single_node("//node1");
            (void)fixed_result1;
            
            pugi::xpath_node fixed_result2 = root.select_single_node("/root/node2");
            (void)fixed_result2;
            
            pugi::xpath_query fixed_query("//node3");
            pugi::xpath_node fixed_result3 = root.select_single_node(fixed_query);
            (void)fixed_result3;
        } catch (...) {
            // Catch any exception
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Test xpath_parse_result::description
    test_xpath_parse_result_description();
    
    // Test xml_parse_result::description
    test_xml_parse_result_description();
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(Data, Size);
    
    return 0;
}