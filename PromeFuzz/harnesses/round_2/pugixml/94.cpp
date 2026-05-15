// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML document from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file(filename);
    
    // Get the root node (or document node if parsing failed)
    pugi::xml_node node = doc.root();
    
    // If parsing failed, create a dummy node to still test the functions
    if (!parse_result) {
        // Create a simple document with a root node to have something to work with
        const char* dummy_xml = "<root><child>text</child></root>";
        pugi::xml_document dummy_doc;
        dummy_doc.load_string(dummy_xml);
        node = dummy_doc.root();
    }
    
    // Create XPath query string from fuzzer input (limit length to avoid overflow)
    std::string query_str;
    if (Size > 0) {
        size_t query_len = Size > 1024 ? 1024 : Size;
        query_str.assign(reinterpret_cast<const char*>(Data), query_len);
        // Ensure null termination
        if (query_str.back() != '\0') {
            query_str.push_back('\0');
        }
    } else {
        query_str = "//*";
    }
    
    // Test select_single_node with char_t* overload
    {
        pugi::xpath_node result1 = node.select_single_node(query_str.c_str());
        (void)result1; // Use result to avoid unused variable warning
    }
    
    // Test select_single_node with xpath_query overload
    {
        pugi::xpath_query query(query_str.c_str());
        pugi::xpath_node result2 = node.select_single_node(query);
        (void)result2; // Use result to avoid unused variable warning
    }
    
    // Test select_node with char_t* overload
    {
        pugi::xpath_node result3 = node.select_node(query_str.c_str());
        (void)result3; // Use result to avoid unused variable warning
    }
    
    // Test xpath_parse_result::description()
    // Create xpath_query to get parse result
    pugi::xpath_query query2(query_str.c_str());
    const pugi::xpath_parse_result& xpath_parse_result = query2.result();
    
    // Get description from parse result
    const char* description = xpath_parse_result.description();
    (void)description; // Use result to avoid unused variable warning
    
    // Also test with a manually created parse result
    pugi::xpath_parse_result manual_result;
    manual_result.error = "Test error";
    manual_result.offset = 0;
    const char* manual_description = manual_result.description();
    (void)manual_description; // Use result to avoid unused variable warning
    
    // Test with different node types
    pugi::xml_node child = node.first_child();
    if (child) {
        pugi::xpath_node child_result = child.select_node(query_str.c_str());
        (void)child_result; // Use result to avoid unused variable warning
    }
    
    // Test with empty query
    {
        pugi::xpath_node empty_result = node.select_node("");
        (void)empty_result; // Use result to avoid unused variable warning
    }
    
    // Test with xpath_variable_set (null for now, but call the overload)
    {
        pugi::xpath_node result_with_vars = node.select_single_node(query_str.c_str(), nullptr);
        (void)result_with_vars; // Use result to avoid unused variable warning
    }
    
    {
        pugi::xpath_node result_with_vars2 = node.select_node(query_str.c_str(), nullptr);
        (void)result_with_vars2; // Use result to avoid unused variable warning
    }
    
    return 0;
}