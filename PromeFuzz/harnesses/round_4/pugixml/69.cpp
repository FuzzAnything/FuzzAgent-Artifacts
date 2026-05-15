// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
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

static void test_xpath_variable_set(pugi::xpath_variable_set& set, const uint8_t* data, size_t size) {
    // Use fuzzer data to create variable names and types
    if (size < 2) return;
    
    // Create a name from the data (ensure null-terminated)
    size_t name_len = data[0] % 32 + 1;
    if (name_len > size - 1) name_len = size - 1;
    
    char name[33];
    memcpy(name, data + 1, name_len);
    name[name_len] = '\0';
    
    // Determine type from data
    pugi::xpath_value_type type = static_cast<pugi::xpath_value_type>(data[0] % 5);
    
    // Test add function
    pugi::xpath_variable* var = set.add(name, type);
    
    if (var) {
        // Test type function
        pugi::xpath_value_type var_type = var->type();
        
        // Test get function
        pugi::xpath_variable* var2 = set.get(name);
        
        // Test const get function
        const pugi::xpath_variable_set& const_set = set;
        const pugi::xpath_variable* var_const = const_set.get(name);
        
        // Test getters based on type
        switch (var_type) {
            case pugi::xpath_type_node_set:
                var->get_node_set();
                if (var_const) var_const->get_node_set();
                break;
            case pugi::xpath_type_number:
                var->get_number();
                if (var_const) var_const->get_number();
                break;
            default:
                // For other types, just call the getters anyway (they should handle type mismatches)
                var->get_node_set();
                var->get_number();
                break;
        }
    }
    
    // Test get with non-existent name
    set.get("non_existent_variable");
}

static void test_xpath_query(const uint8_t* data, size_t size) {
    // Create a query string from fuzzer data
    if (size == 0) return;
    
    // Ensure null-terminated string
    size_t expr_len = size < 256 ? size : 255;
    char expr[256];
    memcpy(expr, data, expr_len);
    expr[expr_len] = '\0';
    
    try {
        // Test xpath_query constructor and return_type
        pugi::xpath_query query(expr);
        query.return_type();
        
        // Test move constructor
        pugi::xpath_query moved_query(std::move(query));
        moved_query.return_type();
    } catch (...) {
        // Ignore exceptions from invalid queries
    }
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size) {
    // Test xpath_variable_set functions
    pugi::xpath_variable_set set;
    test_xpath_variable_set(set, data, size);
    
    // Test xpath_query functions
    test_xpath_query(data, size);
    
    return 0;
}