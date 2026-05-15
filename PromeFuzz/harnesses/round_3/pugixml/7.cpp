// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Initialize required objects
    pugi::xml_document doc;
    pugi::xpath_variable_set vars;
    pugi::xml_node node;
    pugi::xpath_node xpath_node(node);
    
    // 1. Add a variable to the variable set
    // Use first byte to determine variable type (mod 5 for the 5 xpath_value_type values)
    if (Size > 0) {
        pugi::xpath_value_type type = static_cast<pugi::xpath_value_type>(Data[0] % 5);
        vars.add("var", type);
    }
    
    // 2. Load XML document from buffer
    // Use remaining data as XML content
    const void* xml_data = Size > 0 ? Data + 1 : Data;
    size_t xml_size = Size > 0 ? Size - 1 : 0;
    
    pugi::xml_parse_result result = doc.load_buffer(xml_data, xml_size);
    
    // Only proceed if XML was parsed successfully
    if (result.status == pugi::status_ok) {
        // Get the document element as context node
        pugi::xml_node root = doc.document_element();
        if (root) {
            xpath_node = pugi::xpath_node(root);
            
            // Create an XPath query with a simple expression
            // Use a fixed expression for simplicity
            const char* xpath_expr = "//*";
            pugi::xpath_query query(xpath_expr, &vars);
            
            // 3. Evaluate as boolean
            try {
                bool bool_result = query.evaluate_boolean(xpath_node);
                (void)bool_result; // Avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
            
            // 4. Evaluate as number
            try {
                double num_result = query.evaluate_number(xpath_node);
                (void)num_result; // Avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
            
            // 5. Evaluate as string
            try {
                pugi::string_t str_result = query.evaluate_string(xpath_node);
                (void)str_result; // Avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
            
            // 6. Evaluate as node
            try {
                pugi::xpath_node node_result = query.evaluate_node(xpath_node);
                (void)node_result; // Avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
            
            // 7. Evaluate as node set
            try {
                pugi::xpath_node_set node_set_result = query.evaluate_node_set(xpath_node);
                (void)node_set_result; // Avoid unused variable warning
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    return 0;
}