// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a buffer for the query, ensuring null termination
    std::vector<pugi::char_t> query_buffer(Size + 1);
    for (size_t i = 0; i < Size; ++i) {
        query_buffer[i] = static_cast<pugi::char_t>(Data[i]);
    }
    query_buffer[Size] = 0;
    const pugi::char_t* query = query_buffer.data();

    // Prepare XML document
    pugi::xml_document doc;
    if (!doc.load_string("<root><item id=\"1\">Value</item><item id=\"2\">Data</item></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // Prepare variable set
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const;

    // Fuzz target 1: pugi::xpath_variable::name
    // Add a variable first to get a valid instance
    pugi::xpath_variable* var = variables.add("test_var", pugi::xpath_type_string);
    if (var) {
        const pugi::char_t* name = var->name();
        (void)name; // Use to avoid unused warning
    }

    // Fuzz target 2: pugi::xpath_variable_set::get
    pugi::xpath_variable* var_get = variables.get("test_var");
    (void)var_get;
    const pugi::xpath_variable* var_get_const = variables_const.get("test_var");
    (void)var_get_const;

    // Fuzz target 3: pugi::xpath_variable_set::set
    // Attempt to set a string value
    bool set_result = variables.set("test_var", "new_value");
    (void)set_result;

    // Fuzz target 5: pugi::xpath_variable_set::add
    // Add various types to test type matching
    pugi::xpath_variable* var_num = variables.add("num_var", pugi::xpath_type_number);
    (void)var_num;
    pugi::xpath_variable* var_bool = variables.add("bool_var", pugi::xpath_type_boolean);
    (void)var_bool;
    pugi::xpath_variable* var_node = variables.add("node_var", pugi::xpath_type_node_set);
    (void)var_node;
    pugi::xpath_variable* var_none = variables.add("none_var", pugi::xpath_type_none);
    (void)var_none;

    // Fuzz target 6: pugi::xpath_variable::get_string
    if (var) {
        const pugi::char_t* str_val = var->get_string();
        (void)str_val;
    }

    // Additional exploration: try to get string from non-string variables
    if (var_num) {
        const pugi::char_t* str_num = var_num->get_string();
        (void)str_num;
    }

    // Fuzz target 4: pugi::xml_node::select_nodes
    // pugixml does not throw exceptions by default, so we just call the function.
    // If the query is invalid, it returns an empty set.
    try {
        // Execute query with variables
        pugi::xpath_node_set nodes = root.select_nodes(query, &variables);
        (void)nodes;
        
        // Execute query without variables
        pugi::xpath_node_set nodes_no_vars = root.select_nodes(query, nullptr);
        (void)nodes_no_vars;
    } catch (...) {
        // Silently catch any unexpected exceptions to prevent fuzzer crash
    }

    return 0;
}