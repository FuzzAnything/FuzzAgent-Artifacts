// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static const char* dummy_file = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    pugi::xpath_variable_set variables;
    pugi::xml_document doc;
    
    // Write data to dummy file if needed
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // 2. Invoke target functions in order
    
    // Step 1: pugi::xpath_variable_set::add
    // Ensure the variable name is null-terminated to prevent heap-buffer-overflow
    std::string var_name_str(reinterpret_cast<const char*>(Data), Size);
    // Replace any null bytes inside the string with a safe character to ensure validity
    for (size_t i = 0; i < var_name_str.size(); ++i) {
        if (var_name_str[i] == '\0') {
            var_name_str[i] = '_';
        }
    }
    const char* var_name = var_name_str.c_str();
    
    pugi::xpath_variable* v1 = variables.add(var_name, pugi::xpath_type_number);
    pugi::xpath_variable* v2 = variables.add(var_name, pugi::xpath_type_string);
    pugi::xpath_variable* v3 = variables.add(var_name, pugi::xpath_type_boolean);
    pugi::xpath_variable* v4 = variables.add(var_name, pugi::xpath_type_node_set);
    (void)v1; (void)v2; (void)v3; (void)v4;

    // Step 2: pugi::xml_document::load_buffer
    pugi::xml_parse_result res = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    // Get a context node
    pugi::xpath_node context_node;
    if (doc.root().type() == pugi::node_element) {
        context_node = pugi::xpath_node(doc.root());
    } else if (doc.root().type() == pugi::node_document) {
        pugi::xml_node first_elem = doc.root().first_child();
        if (first_elem) {
            context_node = pugi::xpath_node(first_elem);
        }
    }

    // Prepare an XPath query string from the input
    // Ensure the query string is null-terminated for xpath_query constructor
    std::string query_str(reinterpret_cast<const char*>(Data), Size);
    for (size_t i = 0; i < query_str.size(); ++i) {
        if (query_str[i] == '\0') {
            query_str[i] = ' ';
        }
    }
    const char* query_ptr = query_str.c_str();

    try {
        pugi::xpath_query query(query_ptr, &variables);

        // Step 3: pugi::xpath_query::evaluate_boolean
        bool b_res = query.evaluate_boolean(context_node);
        (void)b_res;

        // Step 4: pugi::xpath_query::evaluate_number
        double n_res = query.evaluate_number(context_node);
        (void)n_res;

        // Step 5: pugi::xpath_query::evaluate_string
        pugi::string_t s_res = query.evaluate_string(context_node);
        (void)s_res;

        // Step 6: pugi::xpath_query::evaluate_node
        pugi::xpath_node node_res = query.evaluate_node(context_node);
        (void)node_res;

        // Step 7: pugi::xpath_query::evaluate_node_set
        pugi::xpath_node_set set_res = query.evaluate_node_set(context_node);
        (void)set_res;

    } catch (...) {
        // Handle exceptions
    }

    return 0;
}