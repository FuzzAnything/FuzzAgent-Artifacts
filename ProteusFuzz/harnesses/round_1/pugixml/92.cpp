// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a variable set to manage XPath variables
    pugi::xpath_variable_set variables;

    // Convert input data to a C-string for variable names, ensuring null termination
    // We limit the name length to avoid excessive allocations or stack issues
    const size_t max_name_len = 256;
    char name_buffer[max_name_len + 1] = {0};
    size_t copy_len = (Size < max_name_len) ? Size : max_name_len;
    std::memcpy(name_buffer, Data, copy_len);
    name_buffer[copy_len] = '\0';

    // Test 1: Add a variable with different types
    pugi::xpath_variable* var_bool = variables.add(name_buffer, pugi::xpath_type_boolean);
    pugi::xpath_variable* var_num = variables.add(name_buffer, pugi::xpath_type_number);
    pugi::xpath_variable* var_str = variables.add(name_buffer, pugi::xpath_type_string);
    pugi::xpath_variable* var_node = variables.add(name_buffer, pugi::xpath_type_node_set);

    // Test 2: Set boolean value via variable set
    if (var_bool) {
        // Use a byte from input to determine true/false
        bool val = (Data[0] % 2 == 0);
        variables.set(name_buffer, val);
    }

    // Test 3: Set string value via variable instance
    if (var_str) {
        // Use input data as the string value (ensure it's treated as char_t)
        // Note: pugixml char_t might be wchar_t, but set usually handles char_t*
        // We cast Data to char_t* directly. If char_t is wchar_t, this might be unsafe for wide strings,
        // but for fuzzing, it tests the type checking logic.
        var_str->set(reinterpret_cast<const pugi::char_t*>(Data));
    }

    // Test 4: Retrieve variable via get
    pugi::xpath_variable* var_get = variables.get(name_buffer);
    if (var_get) {
        // Test 5: Get variable name
        const pugi::char_t* name = var_get->name();
        (void)name; // Suppress unused warning

        // Test 6: Get string value
        const pugi::char_t* str_val = var_get->get_string();
        (void)str_val; // Suppress unused warning
    }

    // Test 7: Get from const set
    const pugi::xpath_variable_set variables_const = variables;
    const pugi::xpath_variable* var_const = variables_const.get(name_buffer);
    if (var_const) {
        const pugi::char_t* name_const = var_const->name();
        (void)name_const;
        const pugi::char_t* str_const = var_const->get_string();
        (void)str_const;
    }

    // Test 8: Try to set a boolean on a string variable (should fail)
    if (var_str) {
        variables.set(name_buffer, true); // Should return false due to type mismatch
    }

    // Test 9: Try to set a string on a boolean variable (should fail)
    if (var_bool) {
        var_bool->set(reinterpret_cast<const pugi::char_t*>(Data)); // Should return false
    }

    return 0;
}