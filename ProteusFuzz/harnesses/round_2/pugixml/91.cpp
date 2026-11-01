// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
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

static void safe_fuzz_data(const uint8_t* data, size_t size, char* buffer, size_t max_len) {
    if (size == 0) return;
    size_t len = (size < max_len - 1) ? size : max_len - 1;
    memcpy(buffer, data, len);
    buffer[len] = '\0';
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables_copy = variables;

    char name_buf[256];
    char value_buf[256];
    safe_fuzz_data(Data, Size, name_buf, sizeof(name_buf));
    safe_fuzz_data(Data, Size, value_buf, sizeof(value_buf));

    // Test add with various types
    pugi::xpath_variable* var_none = variables.add(name_buf, pugi::xpath_type_none);
    pugi::xpath_variable* var_node_set = variables.add(name_buf, pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = variables.add(name_buf, pugi::xpath_type_number);
    pugi::xpath_variable* var_string = variables.add(name_buf, pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = variables.add(name_buf, pugi::xpath_type_boolean);

    // Test get (non-const)
    pugi::xpath_variable* get_var = variables.get(name_buf);
    if (get_var) {
        const pugi::char_t* name = get_var->name();
        const pugi::char_t* str_val = get_var->get_string();
        (void)name;
        (void)str_val;
    }

    // Test get (const)
    const pugi::xpath_variable_set variables_const = variables;
    const pugi::xpath_variable* get_var_const = variables_const.get(name_buf);
    if (get_var_const) {
        const pugi::char_t* name_const = get_var_const->name();
        const pugi::char_t* str_val_const = get_var_const->get_string();
        (void)name_const;
        (void)str_val_const;
    }

    // Test set (boolean assignment via string value)
    bool set_result = variables.set(name_buf, value_buf);
    (void)set_result;

    // Test add on copy
    pugi::xpath_variable* var_copy = variables_copy.add(name_buf, pugi::xpath_type_string);
    if (var_copy) {
        const pugi::char_t* name_copy = var_copy->name();
        const pugi::char_t* str_copy = var_copy->get_string();
        (void)name_copy;
        (void)str_copy;
    }

    return 0;
}