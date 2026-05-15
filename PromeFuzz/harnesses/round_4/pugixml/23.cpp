// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
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

static void fuzz_xpath_variable(const uint8_t *Data, size_t Size) {
    pugi::xpath_variable_set set;
    
    // Use fuzz data to create variable name - ensure null termination
    std::string var_name;
    if (Size > 0) {
        size_t name_len = std::min(Size, (size_t)100);
        var_name.assign(reinterpret_cast<const char*>(Data), name_len);
    } else {
        var_name = "var";
    }
    
    // Cycle through different types based on input size
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_boolean,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_node_set,
        pugi::xpath_type_none
    };
    
    for (int i = 0; i < 5; ++i) {
        pugi::xpath_variable* var = set.add(var_name.c_str(), types[i]);
        
        if (var) {
            // Call functions in specified order
            pugi::xpath_value_type var_type = var->type();
            const pugi::char_t* var_name_ptr = var->name();
            bool bool_val = var->get_boolean();
            double num_val = var->get_number();
            const pugi::char_t* str_val = var->get_string();
            
            // Use variables to avoid compiler warnings
            (void)var_type;
            (void)var_name_ptr;
            (void)bool_val;
            (void)num_val;
            (void)str_val;
        }
    }
    
    // Test with different variable names based on input data
    if (Size > 1) {
        std::string alt_name;
        size_t alt_len = std::min(Size/2, (size_t)50);
        alt_name.assign(reinterpret_cast<const char*>(Data) + Size/2, alt_len);
        
        pugi::xpath_variable* alt_var = set.add(alt_name.c_str(), pugi::xpath_type_string);
        if (alt_var) {
            (void)alt_var->type();
            (void)alt_var->name();
            (void)alt_var->get_boolean();
            (void)alt_var->get_number();
            (void)alt_var->get_string();
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    fuzz_xpath_variable(Data, Size);
    return 0;
}