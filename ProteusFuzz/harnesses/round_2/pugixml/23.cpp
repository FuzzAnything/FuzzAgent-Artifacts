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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xpath_variable_set vars;
    
    // Convert fuzz input to a string for variable name
    // Ensure null termination and limit length to avoid overflow
    const size_t max_name_len = 256;
    size_t name_len = (Size < max_name_len) ? Size : max_name_len;
    
    // Create a buffer for the name
    char name_buf[max_name_len + 1];
    for (size_t i = 0; i < name_len; ++i) {
        // Filter out null bytes to ensure valid C-string
        if (Data[i] == 0) continue;
        name_buf[i] = static_cast<char>(Data[i]);
    }
    name_buf[name_len] = '\0';
    
    // If the name is empty after filtering, use a default
    const char* var_name = (name_buf[0] == '\0') ? "test_var" : name_buf;

    // 1. pugi::xpath_variable_set::add
    // Try adding with different types to explore states
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_boolean,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_node_set
    };
    
    for (int i = 0; i < 4; ++i) {
        pugi::xpath_variable* var = vars.add(var_name, types[i]);
        
        if (var == nullptr) {
            continue;
        }

        // 2. pugi::xpath_variable::type
        pugi::xpath_value_type current_type = var->type();

        // 3. pugi::xpath_variable::name
        const pugi::char_t* var_name_result = var->name();
        (void)var_name_result; // Use the result to suppress warnings

        // 4. pugi::xpath_variable::get_boolean
        bool bool_val = var->get_boolean();
        (void)bool_val;

        // 5. pugi::xpath_variable::get_number
        double num_val = var->get_number();
        (void)num_val;

        // 6. pugi::xpath_variable::get_string
        const pugi::char_t* str_val = var->get_string();
        (void)str_val;
    }

    return 0;
}