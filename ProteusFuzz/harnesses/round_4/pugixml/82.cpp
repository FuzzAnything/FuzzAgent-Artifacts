// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
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

// Helper to safely convert fuzz input to a null-terminated pugi::char_t string
static const pugi::char_t* safe_string(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    
    // Find the first null terminator or use the whole buffer size
    size_t len = size;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) {
            len = i;
            break;
        }
    }
    
    // Use a static buffer to hold the converted string
    static pugi::char_t buf[256];
    
    // Limit length to buffer size - 1
    if (len > 255) len = 255;
    
    // Copy data into the buffer
    if (sizeof(pugi::char_t) == 1) {
        memcpy(buf, data, len);
        buf[len] = 0;
    } else {
        // For wchar_t, we assume the input is ASCII compatible and cast
        for (size_t i = 0; i < len; ++i) {
            buf[i] = static_cast<pugi::char_t>(data[i]);
        }
        buf[len] = 0;
    }
    
    return buf;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_const; // For const get test
    pugi::xml_node node; // Default constructor creates empty node

    // Helper to get a string from input
    const pugi::char_t* var_name = safe_string(Data, Size);
    const pugi::char_t* query_str = safe_string(Data, Size);
    const pugi::char_t* str_val = safe_string(Data, Size);
    
    // Fuzzing logic

    // 4. pugi::xpath_variable_set::add
    // Add variables of different types to test set/get interactions
    pugi::xpath_variable* var_bool = variables.add(var_name, pugi::xpath_type_boolean);
    pugi::xpath_variable* var_num = variables.add(var_name, pugi::xpath_type_number);
    pugi::xpath_variable* var_str = variables.add(var_name, pugi::xpath_type_string);
    pugi::xpath_variable* var_none = variables.add(var_name, pugi::xpath_type_none);

    // 1. pugi::xpath_variable_set::set
    // Overload 1: set(name, double)
    // We use the first byte of input as a double (cast) or just 0.0/1.0
    double val_double = (Size > 0) ? static_cast<double>(Data[0]) : 0.0;
    if (var_num) {
        variables.set(var_name, val_double);
    }
    // Try on a non-number variable (should fail)
    if (var_bool) {
        variables.set(var_name, val_double);
    }

    // Overload 2: set(name, const char_t*)
    if (var_str) {
        variables.set(var_name, str_val);
    }
    // Try on a non-string variable (should fail)
    if (var_bool) {
        variables.set(var_name, str_val);
    }

    // 2. pugi::xpath_variable_set::get
    // Non-const version
    pugi::xpath_variable* var_found = variables.get(var_name);
    if (var_found) {
        // 3. pugi::xpath_variable::get_string
        // Only call if we expect a string, but fuzzing should try anyway to test robustness
        const pugi::char_t* str_result = var_found->get_string();
        (void)str_result; // Suppress unused warning
    }

    // Const version
    const pugi::xpath_variable* var_const = variables_const.get(var_name);
    if (var_const) {
        const pugi::char_t* str_const = var_const->get_string();
        (void)str_const;
    }

    // 5. pugi::xml_node::select_nodes
    // Test with variables
    // Wrap in try-catch to handle potential xpath_exception thrown by invalid queries
    try {
        pugi::xpath_node_set nodes = node.select_nodes(query_str, &variables);
        (void)nodes;
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
    }
    
    // Test without variables
    try {
        pugi::xpath_node_set nodes_no_vars = node.select_nodes(query_str);
        (void)nodes_no_vars;
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
    }

    // Cleanup is handled by RAII (destructors) for pugi objects.
    // No explicit cleanup needed for stack objects.

    return 0;
}