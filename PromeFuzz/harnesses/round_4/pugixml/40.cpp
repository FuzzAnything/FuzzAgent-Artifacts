// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0; // Skip invalid XML
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // First pugi::xml_node::child call
    std::string_view child1_name = "child1";
    pugi::xml_node child1 = root.child(child1_name.data());
    
    // pugi::xml_node::attribute call
    std::string_view attr_name = "attr1";
    pugi::xml_attribute attr = root.attribute(attr_name.data());
    
    // Second pugi::xml_node::child call
    std::string_view child2_name = "child2";
    pugi::xml_node child2 = root.child(child2_name.data());
    
    // Create XPath query from input (use first part of data as XPath string)
    std::string xpath_str;
    if (Size > 0) {
        size_t xpath_len = Size > 100 ? 100 : Size; // Limit XPath length
        xpath_str.assign(reinterpret_cast<const char*>(Data), xpath_len);
        // Ensure null termination
        xpath_str.push_back('\0');
        xpath_str.pop_back();
    } else {
        xpath_str = "//*"; // Default XPath
    }
    
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_str.c_str());
    } catch (...) {
        // Catch any exception (including xpath_exception if available)
        query = pugi::xpath_query("//*");
    }
    
    // Create context node
    pugi::xpath_node context(root);
    
    // pugi::xpath_query::evaluate_boolean
    bool bool_result = false;
    try {
        bool_result = query.evaluate_boolean(context);
    } catch (...) {
        // Handle any exception
    }
    
    // pugi::xpath_query::evaluate_number
    double num_result = 0.0;
    try {
        num_result = query.evaluate_number(context);
    } catch (...) {
        // Handle any exception
    }
    
    // First pugi::xpath_query::evaluate_string (return string)
    std::string string_result1;
    try {
        pugi::string_t str_tmp = query.evaluate_string(context);
        string_result1 = str_tmp.c_str();
    } catch (...) {
        // Handle any exception
    }
    
    // Second pugi::xpath_query::evaluate_string (buffer version)
    char buffer[1024];
    size_t buffer_result = 0;
    try {
        buffer_result = query.evaluate_string(buffer, sizeof(buffer)/sizeof(buffer[0]), context);
    } catch (...) {
        // Handle any exception
    }
    
    // pugi::xpath_query::evaluate_node_set
    pugi::xpath_node_set node_set;
    try {
        node_set = query.evaluate_node_set(context);
    } catch (...) {
        // Handle any exception
    }
    
    // pugi::xpath_query::evaluate_node
    pugi::xpath_node node_result;
    try {
        node_result = query.evaluate_node(context);
    } catch (...) {
        // Handle any exception
    }
    
    // Use results to avoid optimization
    (void)child1;
    (void)attr;
    (void)child2;
    (void)bool_result;
    (void)num_result;
    (void)string_result1;
    (void)buffer_result;
    (void)node_set;
    (void)node_result;
    
    return 0;
}