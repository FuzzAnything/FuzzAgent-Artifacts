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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xpath_variable_set var_set;
    
    // Create a safe, null-terminated string for the variable name
    char safe_name[256];
    size_t len = (Size < 255) ? Size : 255;
    memcpy(safe_name, Data, len);
    safe_name[len] = '\0';
    
    const pugi::char_t* safe_name_ptr = reinterpret_cast<const pugi::char_t*>(safe_name);

    // Determine type from input data to explore different states
    pugi::xpath_value_type type = pugi::xpath_type_none;
    if (Size > 0) {
        uint8_t type_byte = Data[0] % 5; // 0 to 4
        type = static_cast<pugi::xpath_value_type>(type_byte);
    }

    // 1. pugi::xpath_variable_set::add
    pugi::xpath_variable* var = var_set.add(safe_name_ptr, type);
    
    if (!var) {
        return 0;
    }

    // 2. pugi::xpath_variable::type
    pugi::xpath_value_type current_type = var->type();
    
    // 3. pugi::xpath_variable::name
    const pugi::char_t* var_name = var->name();
    (void)var_name; 

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