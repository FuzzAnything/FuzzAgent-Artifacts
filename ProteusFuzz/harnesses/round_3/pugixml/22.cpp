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

// Helper function to create a safe null-terminated string from fuzz data
// Uses pugi::char_t to ensure compatibility with the library's character type
static const pugi::char_t* safe_string_from_fuzz(const uint8_t* data, size_t size) {
    if (size == 0) {
        static const pugi::char_t empty_str[] = PUGIXML_TEXT("");
        return empty_str;
    }

    // Limit buffer size to prevent overflow
    const size_t max_len = 255;
    size_t len = (size < max_len) ? size : max_len;

    static pugi::char_t buffer[256];
    
    // Copy data and ensure null termination
    std::memcpy(buffer, data, len * sizeof(pugi::char_t));
    buffer[len] = PUGIXML_TEXT('\0');
    
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    pugi::xpath_variable_set var_set;
    
    // Derive type from fuzz input (0-4 for the 5 enum values)
    pugi::xpath_value_type type = static_cast<pugi::xpath_value_type>(Data[0] % 5);
    
    // Create a safe name string from the remaining fuzz data
    const pugi::char_t* name = safe_string_from_fuzz(Data + 1, Size - 1);

    // Invoke: pugi::xpath_variable_set::add
    pugi::xpath_variable* var = var_set.add(name, type);
    
    if (!var) {
        // If add failed, we cannot proceed safely
        return 0;
    }

    // Invoke: pugi::xpath_variable::type
    pugi::xpath_value_type retrieved_type = var->type();
    (void)retrieved_type; // Suppress unused variable warning

    // Invoke: pugi::xpath_variable::name
    const pugi::char_t* retrieved_name = var->name();
    if (retrieved_name) {
        volatile size_t len = std::strlen(retrieved_name);
        (void)len;
    }

    // Invoke: pugi::xpath_variable::get_boolean
    bool bool_val = var->get_boolean();
    (void)bool_val;

    // Invoke: pugi::xpath_variable::get_number
    double num_val = var->get_number();
    (void)num_val;

    // Invoke: pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var->get_string();
    if (str_val) {
        volatile size_t str_len = std::strlen(str_val);
        (void)str_len;
    }

    // 2. Cleanup
    // pugixml manages memory internally for the variable set and variables.
    // No explicit destructor call needed for var_set or var as they are RAII.
    
    return 0;
}