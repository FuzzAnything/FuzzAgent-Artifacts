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
    if (Size == 0) return 0;

    // Use first byte to determine variable type
    uint8_t type_selector = Data[0] % 5;
    pugi::xpath_value_type var_type;
    switch (type_selector) {
        case 0: var_type = pugi::xpath_value_type::xpath_type_boolean; break;
        case 1: var_type = pugi::xpath_value_type::xpath_type_number; break;
        case 2: var_type = pugi::xpath_value_type::xpath_type_string; break;
        case 3: var_type = pugi::xpath_value_type::xpath_type_node_set; break;
        case 4: var_type = pugi::xpath_value_type::xpath_type_none; break;
        default: var_type = pugi::xpath_value_type::xpath_type_boolean; break;
    }

    // Use remaining bytes for variable name (ensure null-terminated)
    const char* var_name = reinterpret_cast<const char*>(Data + 1);
    size_t name_len = (Size - 1 < 256) ? Size - 1 : 256;
    char safe_name[257];
    if (name_len > 0) {
        for (size_t i = 0; i < name_len; ++i) {
            safe_name[i] = var_name[i];
        }
        safe_name[name_len] = '\0';
    } else {
        safe_name[0] = 'v';
        safe_name[1] = '\0';
    }

    // Create variable set and add variable
    pugi::xpath_variable_set var_set;
    pugi::xpath_variable* var = var_set.add(safe_name, var_type);

    if (var != nullptr) {
        // Call target functions in specified order
        pugi::xpath_value_type retrieved_type = var->type();
        (void)retrieved_type; // Use to avoid unused variable warning
        
        const pugi::char_t* var_name_ptr = var->name();
        (void)var_name_ptr;
        
        bool bool_val = var->get_boolean();
        (void)bool_val;
        
        double num_val = var->get_number();
        (void)num_val;
        
        const pugi::char_t* str_val = var->get_string();
        (void)str_val;
    }

    return 0;
}