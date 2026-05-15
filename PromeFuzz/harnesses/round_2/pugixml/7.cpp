// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // 1. Add variable to variable set
    pugi::xpath_variable_set vars;
    pugi::xpath_variable* var = vars.add("test_var", pugi::xpath_type_boolean);
    if (var) {
        var->set(true);
    }
    
    // 2. Load XML document from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, we can still try to evaluate on empty document
    if (result.status != pugi::status_ok) {
        // Create empty document for evaluation
        doc.reset();
    }
    
    // Create context node for XPath evaluation
    pugi::xml_node root = doc.document_element();
    pugi::xpath_node context(root);
    
    // Create XPath query with a simple expression
    const char* xpath_expr = "//*";
    pugi::xpath_query query(xpath_expr, &vars);
    
    // Only evaluate if query is valid
    if (query) {
        // 3. Evaluate as boolean
        try {
            bool bool_result = query.evaluate_boolean(context);
            (void)bool_result; // Prevent unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
        
        // 4. Evaluate as number
        try {
            double num_result = query.evaluate_number(context);
            (void)num_result; // Prevent unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
        
        // 5. Evaluate as string
        try {
            pugi::string_t str_result = query.evaluate_string(context);
            (void)str_result; // Prevent unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
        
        // 6. Evaluate as node
        try {
            pugi::xpath_node node_result = query.evaluate_node(context);
            (void)node_result; // Prevent unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
        
        // 7. Evaluate as node set
        try {
            pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
            (void)node_set_result; // Prevent unused variable warning
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}