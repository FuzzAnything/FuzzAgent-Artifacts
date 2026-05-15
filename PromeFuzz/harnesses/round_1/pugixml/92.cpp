// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    // Use a small portion of data for variable names to avoid excessive memory usage
    const size_t name_len = Size > 0 ? (Data[0] % 32) + 1 : 1;
    if (name_len > Size) return 0;
    
    // Create variable name from input data
    char var_name[33] = {0};
    size_t copy_len = name_len < 32 ? name_len : 32;
    for (size_t i = 0; i < copy_len && i < Size; ++i) {
        var_name[i] = Data[i] ? Data[i] : 'a'; // Replace null bytes
    }
    var_name[copy_len] = '\0';
    
    // Initialize pugixml objects
    pugi::xpath_variable_set var_set;
    pugi::xpath_query query;
    pugi::xpath_node_set node_set;
    
    // Test xpath_variable_set::add with different types
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean
    };
    
    // Use input data to determine which type to test
    size_t type_idx = Size > 0 ? Data[0] % 4 : 0;
    pugi::xpath_variable* var = var_set.add(var_name, types[type_idx]);
    
    if (var) {
        // Test xpath_variable::type
        pugi::xpath_value_type var_type = var->type();
        
        // Test xpath_variable::get_number based on type
        if (var_type == pugi::xpath_type_number) {
            double num = var->get_number();
            (void)num; // Use result to avoid optimization
        }
        
        // Test xpath_variable::get_node_set based on type
        if (var_type == pugi::xpath_type_node_set) {
            const pugi::xpath_node_set& nodes = var->get_node_set();
            (void)nodes; // Use result to avoid optimization
        }
        
        // Test xpath_variable_set::get
        pugi::xpath_variable* retrieved_var = var_set.get(var_name);
        (void)retrieved_var;
        
        // Test const version of get
        const pugi::xpath_variable_set& const_var_set = var_set;
        const pugi::xpath_variable* const_var = const_var_set.get(var_name);
        (void)const_var;
    }
    
    // Test xpath_query::return_type
    pugi::xpath_value_type query_return_type = query.return_type();
    (void)query_return_type;
    
    // Test with different variable names using remaining data
    if (Size > 1) {
        char alt_name[33] = {0};
        size_t alt_len = (Data[1] % 16) + 1;
        size_t alt_copy = alt_len < 32 ? alt_len : 32;
        
        for (size_t i = 0; i < alt_copy && i + 1 < Size; ++i) {
            alt_name[i] = Data[i + 1] ? Data[i + 1] : 'b';
        }
        alt_name[alt_copy] = '\0';
        
        // Try adding another variable with different type
        size_t alt_type_idx = Size > 1 ? Data[1] % 4 : 0;
        var_set.add(alt_name, types[alt_type_idx]);
    }
    
    // Cleanup happens automatically when objects go out of scope
    return 0;
}