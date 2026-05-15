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

static pugi::xpath_variable* add_variable_from_fuzz_data(pugi::xpath_variable_set& set, const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    // Use first byte to determine variable type
    pugi::xpath_value_type type;
    switch (data[0] % 5) {
        case 0: type = pugi::xpath_type_node_set; break;
        case 1: type = pugi::xpath_type_number; break;
        case 2: type = pugi::xpath_type_string; break;
        case 3: type = pugi::xpath_type_boolean; break;
        default: type = pugi::xpath_type_none; break;
    }
    
    // Create name from remaining data (truncate at 100 chars for safety)
    char name[101] = {0};
    size_t name_len = (size - 1) < 100 ? (size - 1) : 100;
    if (name_len > 0 && size > 1) {
        for (size_t i = 0; i < name_len; i++) {
            name[i] = data[i + 1] ? data[i + 1] : 'A'; // Replace nulls
        }
        name[name_len] = '\0';
    } else {
        name[0] = 'v'; name[1] = 'a'; name[2] = 'r'; name[3] = '\0';
    }
    
    return set.add(name, type);
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xpath_variable_set var_set;
    
    // 1. pugi::xpath_variable_set::add
    pugi::xpath_variable* var = add_variable_from_fuzz_data(var_set, Data, Size);
    if (!var) return 0;
    
    // 2. pugi::xpath_variable::type
    pugi::xpath_value_type var_type = var->type();
    
    // 3. pugi::xpath_variable::name
    const pugi::char_t* var_name = var->name();
    (void)var_name; // Use to avoid unused variable warning
    
    // 4. pugi::xpath_variable::get_boolean
    bool bool_val = var->get_boolean();
    (void)bool_val;
    
    // 5. pugi::xpath_variable::get_number
    double num_val = var->get_number();
    (void)num_val;
    
    // 6. pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var->get_string();
    (void)str_val;
    
    return 0;
}