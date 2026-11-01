// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    pugi::xml_document doc;
    pugi::xpath_variable_set var_set;
    
    // Initialize variable set with a variable
    const char* var_name = "test_var";
    // Add variable: name, type (xpath_type_number)
    pugi::xpath_variable* var = var_set.add(var_name, pugi::xpath_type_number);
    (void)var; 

    // 2. Load buffer
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    pugi::xml_node root = doc.root();
    pugi::xpath_node context_node(root);

    // 3. Create XPath query
    // Use a fixed safe query string to avoid encoding issues with raw binary input
    const char* query_str = "/*";
    
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(query_str);
    } catch (...) {
        return 0;
    }

    if (!query) {
        return 0;
    }

    // 4. Invoke target functions in order
    
    // a. evaluate_boolean
    try {
        bool b = query.evaluate_boolean(context_node);
        (void)b;
    } catch (...) {
        // Ignore exceptions
    }

    // b. evaluate_number
    try {
        double n = query.evaluate_number(context_node);
        (void)n;
    } catch (...) {
        // Ignore exceptions
    }

    // c. evaluate_string
    try {
        pugi::string_t s = query.evaluate_string(context_node);
        (void)s;
    } catch (...) {
        // Ignore exceptions
    }

    // d. evaluate_node
    try {
        pugi::xpath_node node = query.evaluate_node(context_node);
        (void)node;
    } catch (...) {
        // Ignore exceptions
    }

    // e. evaluate_node_set
    try {
        pugi::xpath_node_set nodes = query.evaluate_node_set(context_node);
        (void)nodes;
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}