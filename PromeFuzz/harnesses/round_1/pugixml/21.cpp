// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
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
#include "pugixml.hpp"

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xpath_variable_set var_set;
    
    // Use first byte to determine variable type
    pugi::xpath_value_type var_type;
    switch (Data[0] % 5) {
        case 0: var_type = pugi::xpath_type_boolean; break;
        case 1: var_type = pugi::xpath_type_number; break;
        case 2: var_type = pugi::xpath_type_string; break;
        case 3: var_type = pugi::xpath_type_node_set; break;
        default: var_type = pugi::xpath_type_none; break;
    }
    
    // Create variable name from input data (ensure null termination)
    static char var_name[256];
    size_t name_len = (Size - 1) < 255 ? (Size - 1) : 255;
    for (size_t i = 0; i < name_len; ++i) {
        var_name[i] = Data[1 + i];
    }
    var_name[name_len] = '\0';
    
    // Add variable to set
    pugi::xpath_variable* var = var_set.add(var_name, var_type);
    
    if (var) {
        // Get variable type
        pugi::xpath_value_type retrieved_type = var->type();
        (void)retrieved_type;
        
        // Get variable name
        const pugi::char_t* retrieved_name = var->name();
        (void)retrieved_name;
        
        // Get boolean value
        bool bool_val = var->get_boolean();
        (void)bool_val;
        
        // Get number value
        double num_val = var->get_number();
        (void)num_val;
        
        // Get string value
        const pugi::char_t* str_val = var->get_string();
        (void)str_val;
    }
    
    return 0;
}