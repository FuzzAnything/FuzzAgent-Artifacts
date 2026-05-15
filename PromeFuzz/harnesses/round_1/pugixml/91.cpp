// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static void fuzz_xpath_query_evaluate_node(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a context node (try root, then first child)
        pugi::xml_node context_node = doc.root().first_child();
        if (!context_node) context_node = doc.root();
        
        pugi::xpath_node xpath_context(context_node);
        
        pugi::xpath_node result = query.evaluate_node(xpath_context);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions as per API documentation
    }
}

static void fuzz_xml_node_select_nodes(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a node to call select_nodes on
        pugi::xml_node node = doc.root().first_child();
        if (!node) node = doc.root();
        
        pugi::xpath_node_set result = node.select_nodes(query);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions
    }
}

static void fuzz_xpath_query_evaluate_boolean(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a context node
        pugi::xml_node context_node = doc.root().first_child();
        if (!context_node) context_node = doc.root();
        
        pugi::xpath_node xpath_context(context_node);
        
        bool result = query.evaluate_boolean(xpath_context);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions
    }
}

static void fuzz_xpath_query_evaluate_string(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a context node
        pugi::xml_node context_node = doc.root().first_child();
        if (!context_node) context_node = doc.root();
        
        pugi::xpath_node xpath_context(context_node);
        
        pugi::string_t result = query.evaluate_string(xpath_context);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions
    }
}

static void fuzz_xpath_query_evaluate_number(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a context node
        pugi::xml_node context_node = doc.root().first_child();
        if (!context_node) context_node = doc.root();
        
        pugi::xpath_node xpath_context(context_node);
        
        double result = query.evaluate_number(xpath_context);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions
    }
}

static void fuzz_xpath_query_evaluate_node_set(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size < 2) return;
    
    // Create XPath query from part of the input
    size_t xpath_len = size / 2;
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_len);
    
    try {
        pugi::xpath_query query(xpath_str.c_str());
        if (!query) return;
        
        // Get a context node
        pugi::xml_node context_node = doc.root().first_child();
        if (!context_node) context_node = doc.root();
        
        pugi::xpath_node xpath_context(context_node);
        
        pugi::xpath_node_set result = query.evaluate_node_set(xpath_context);
        (void)result; // Use result to avoid unused variable warning
    } catch (...) {
        // Catch all exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create XML document from input
    pugi::xml_document doc;
    try {
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf8);
        
        // If parsing fails, create a minimal valid XML document
        if (!result) {
            const char* minimal_xml = "<root><child attr=\"value\">text</child></root>";
            doc.load_string(minimal_xml);
        }
    } catch (...) {
        // If document loading throws, create minimal XML
        const char* minimal_xml = "<root><child attr=\"value\">text</child></root>";
        doc.load_string(minimal_xml);
    }
    
    // Test all target API functions
    fuzz_xpath_query_evaluate_node(Data, Size, doc);
    fuzz_xml_node_select_nodes(Data, Size, doc);
    fuzz_xpath_query_evaluate_boolean(Data, Size, doc);
    fuzz_xpath_query_evaluate_string(Data, Size, doc);
    fuzz_xpath_query_evaluate_number(Data, Size, doc);
    fuzz_xpath_query_evaluate_node_set(Data, Size, doc);
    
    return 0;
}