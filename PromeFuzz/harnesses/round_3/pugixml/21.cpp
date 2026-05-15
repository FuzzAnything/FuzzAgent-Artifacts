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
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Use the first byte to determine variable type
    if (Size == 0) return 0;
    
    uint8_t type_choice = Data[0] % 5;  // 5 possible xpath_value_type values
    pugi::xpath_value_type var_type;
    
    switch (type_choice) {
        case 0: var_type = pugi::xpath_type_none; break;
        case 1: var_type = pugi::xpath_type_node_set; break;
        case 2: var_type = pugi::xpath_type_number; break;
        case 3: var_type = pugi::xpath_type_string; break;
        case 4: var_type = pugi::xpath_type_boolean; break;
        default: var_type = pugi::xpath_type_none; break;
    }
    
    // Use remaining bytes for variable name (ensure null termination)
    size_t name_len = (Size - 1) > 0 ? Size - 1 : 0;
    char* var_name = new char[name_len + 1];
    if (name_len > 0) {
        memcpy(var_name, Data + 1, name_len);
    }
    var_name[name_len] = '\0';
    
    // Create variable set and add variable
    pugi::xpath_variable_set var_set;
    pugi::xpath_variable* var = var_set.add(var_name, var_type);
    
    // Call API functions in specified order if variable was created
    if (var != nullptr) {
        // 1. pugi::xpath_variable_set::add (already called above)
        
        // 2. pugi::xpath_variable::type
        pugi::xpath_value_type retrieved_type = var->type();
        (void)retrieved_type;  // Avoid unused variable warning
        
        // 3. pugi::xpath_variable::name
        const char* retrieved_name = var->name();
        (void)retrieved_name;  // Avoid unused variable warning
        
        // 4. pugi::xpath_variable::get_boolean
        bool bool_val = var->get_boolean();
        (void)bool_val;  // Avoid unused variable warning
        
        // 5. pugi::xpath_variable::get_number
        double num_val = var->get_number();
        (void)num_val;  // Avoid unused variable warning
        
        // 6. pugi::xpath_variable::get_string
        const char* str_val = var->get_string();
        (void)str_val;  // Avoid unused variable warning
    }
    
    // Cleanup
    delete[] var_name;
    
    return 0;
}