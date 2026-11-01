// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
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
#include <pugixml.hpp>

static void fuzz_xpath_variable_set_get(const uint8_t* data, size_t size) {
    pugi::xpath_variable_set variables;
    
    // Try to add some variables first to have something to get
    if (size > 0) {
        // Use first byte to determine variable name length (clamped)
        size_t name_len = (data[0] % 10) + 1;
        if (name_len > size - 1) name_len = size - 1;
        
        pugi::char_t var_name[11];
        memset(var_name, 0, sizeof(var_name));
        for (size_t i = 0; i < name_len && i < 10; i++) {
            var_name[i] = static_cast<pugi::char_t>(data[i + 1]);
        }
        
        // Add variables of different types
        pugi::xpath_variable* v1 = variables.add(var_name, pugi::xpath_type_string);
        pugi::xpath_variable* v2 = variables.add(var_name, pugi::xpath_type_number);
        pugi::xpath_variable* v3 = variables.add(var_name, pugi::xpath_type_boolean);
        pugi::xpath_variable* v4 = variables.add(var_name, pugi::xpath_type_node_set);
        (void)v1; (void)v2; (void)v3; (void)v4;
    }
    
    // Try to get variables with different names from input
    if (size > 0) {
        size_t name_len = (data[0] % 10) + 1;
        if (name_len > size - 1) name_len = size - 1;
        
        pugi::char_t var_name[11];
        memset(var_name, 0, sizeof(var_name));
        for (size_t i = 0; i < name_len && i < 10; i++) {
            var_name[i] = static_cast<pugi::char_t>(data[i + 1]);
        }
        
        pugi::xpath_variable* var = variables.get(var_name);
        if (var) {
            // Test name() function
            const pugi::char_t* name = var->name();
            (void)name;
            
            // Test type() function
            pugi::xpath_value_type type = var->type();
            (void)type;
            
            // Test get_string() function
            const pugi::char_t* str = var->get_string();
            (void)str;
        }
    }
    
    // Test const version - Avoid copying the set if it contains string variables 
    // to prevent assertion failures in debug builds due to null string pointers.
    // Instead, use a const reference to the original set.
    const pugi::xpath_variable_set& variables_const = variables;
    if (size > 0) {
        size_t name_len = (data[0] % 10) + 1;
        if (name_len > size - 1) name_len = size - 1;
        
        pugi::char_t var_name[11];
        memset(var_name, 0, sizeof(var_name));
        for (size_t i = 0; i < name_len && i < 10; i++) {
            var_name[i] = static_cast<pugi::char_t>(data[i + 1]);
        }
        
        const pugi::xpath_variable* var_const = variables_const.get(var_name);
        if (var_const) {
            const pugi::char_t* name = var_const->name();
            (void)name;
            pugi::xpath_value_type type = var_const->type();
            (void)type;
            const pugi::char_t* str = var_const->get_string();
            (void)str;
        }
    }
}

static void fuzz_xpath_query_return_type(const uint8_t* data, size_t size) {
    pugi::xpath_query query;
    
    // Test return_type on uninitialized query
    pugi::xpath_value_type type = query.return_type();
    (void)type;
    
    // Try to create queries from input data
    if (size > 0) {
        // Create a null-terminated string from input
        pugi::char_t query_str[256];
        size_t len = (size < 255) ? size : 255;
        for (size_t i = 0; i < len; i++) {
            query_str[i] = static_cast<pugi::char_t>(data[i]);
        }
        query_str[len] = '\0';
        
        pugi::xpath_variable_set vars;
        // Create a new query, passing pointer to variables set
        // Wrap in try-catch to handle exceptions thrown by pugixml on invalid queries
        try {
            pugi::xpath_query new_query(query_str, &vars);
            type = new_query.return_type();
            (void)type;
        } catch (...) {
            // Ignore exceptions from invalid XPath queries
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz xpath_variable_set::get and related functions
    fuzz_xpath_variable_set_get(Data, Size);
    
    // Fuzz xpath_query::return_type
    fuzz_xpath_query_return_type(Data, Size);
    
    return 0;
}