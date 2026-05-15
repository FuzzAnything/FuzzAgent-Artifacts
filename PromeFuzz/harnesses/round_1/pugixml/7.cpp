// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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

static pugi::xml_document doc;
static pugi::xpath_variable_set vars;
static pugi::xpath_query query;

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Reset state for each iteration
    doc.reset();
    vars = pugi::xpath_variable_set();
    query = pugi::xpath_query();

    // 1. Add a variable to xpath_variable_set
    // Use first byte to determine variable type, rest for name (if any)
    if (Size > 0) {
        pugi::xpath_value_type type = static_cast<pugi::xpath_value_type>(Data[0] % 5);
        
        // Create a safe null-terminated string for the variable name
        // Use at most Size-1 bytes for the name, ensure null termination
        size_t name_len = Size > 1 ? Size - 1 : 0;
        if (name_len > 0) {
            // Create a buffer with null termination
            std::vector<char> name_buf(name_len + 1);
            memcpy(name_buf.data(), Data + 1, name_len);
            name_buf[name_len] = '\0';
            
            vars.add(name_buf.data(), type);
        } else {
            // Empty name
            vars.add("", type);
        }
    }

    // 2. Load XML document from buffer
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // 3-7. Evaluate XPath queries if document loaded successfully
    if (result.status == pugi::status_ok) {
        pugi::xpath_node context(doc);
        
        // Create a simple XPath query for evaluation
        const char* xpath_expr = "/*";
        query = pugi::xpath_query(xpath_expr);
        
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
    }
    
    return 0;
}