// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
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
#include <cstring>
#include <cstdlib>

static void safe_terminate_null_terminator(uint8_t* buffer, size_t size) {
    if (size == 0) return;
    // Ensure null termination by overwriting the last byte if not already null
    if (buffer[size - 1] != 0) {
        buffer[size - 1] = 0;
    }
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare a safe null-terminated string from fuzz input for variable names
    // We allocate a buffer slightly larger to ensure we can add a null terminator safely
    size_t name_len = Size;
    if (name_len > 256) name_len = 256; // Limit name length to avoid excessive allocation
    
    char* name_buffer = (char*)malloc(name_len + 1);
    if (!name_buffer) return 0;
    
    std::memcpy(name_buffer, Data, name_len);
    name_buffer[name_len] = '\0';
    safe_terminate_null_terminator((uint8_t*)name_buffer, name_len);

    // Prepare another buffer for string values if needed
    char* value_buffer = (char*)malloc(name_len + 1);
    if (!value_buffer) {
        free(name_buffer);
        return 0;
    }
    std::memcpy(value_buffer, Data, name_len);
    value_buffer[name_len] = '\0';
    safe_terminate_null_terminator((uint8_t*)value_buffer, name_len);

    // Convert to pugi::char_t (could be wchar_t depending on build)
    // For simplicity in fuzzing, we assume the input is valid UTF-8/ASCII compatible
    // If PUGIXML_WCHAR_MODE is on, we might need conversion, but direct cast is common in simple fuzzers
    // However, to be safe and compliant with char_t, we will cast carefully.
    // Note: If char_t is wchar_t, this simple cast might be insufficient for wide chars, 
    // but for fuzzing structural bugs, this is often acceptable. 
    // A more robust approach would involve UTF-8 to UTF-16/32 conversion if needed.
    // Given the constraints, we proceed with direct casting assuming ASCII input or compatible encoding.
    
    pugi::char_t* name_cstr = reinterpret_cast<pugi::char_t*>(name_buffer);
    pugi::char_t* value_cstr = reinterpret_cast<pugi::char_t*>(value_buffer);

    // Test 1: pugi::xpath_variable_set::get (const overload)
    {
        const pugi::xpath_variable_set variables_const;
        const pugi::xpath_variable* var_const = variables_const.get(name_cstr);
        // var_const is likely nullptr as the set is empty, which is expected
        if (var_const) {
            // Should not happen on empty set, but if it does, try to access
            const pugi::char_t* name = var_const->name();
            const pugi::char_t* str = var_const->get_string();
            (void)name;
            (void)str;
        }
    }

    // Test 2: pugi::xpath_variable_set::get (non-const overload)
    {
        pugi::xpath_variable_set variables;
        pugi::xpath_variable* var = variables.get(name_cstr);
        if (var) {
            const pugi::char_t* name = var->name();
            const pugi::char_t* str = var->get_string();
            (void)name;
            (void)str;
        }
    }

    // Test 3: pugi::xpath_variable_set::add and subsequent operations
    {
        pugi::xpath_variable_set variables;
        
        // Add variable with different types
        pugi::xpath_variable* var_string = variables.add(name_cstr, pugi::xpath_type_string);
        pugi::xpath_variable* var_number = variables.add(name_cstr, pugi::xpath_type_number);
        pugi::xpath_variable* var_bool = variables.add(name_cstr, pugi::xpath_type_boolean);
        pugi::xpath_variable* var_node = variables.add(name_cstr, pugi::xpath_type_node_set);

        // Test get_string on string type
        if (var_string) {
            const pugi::char_t* str = var_string->get_string();
            (void)str;
        }
        
        // Test get_string on non-string type (should return empty)
        if (var_number) {
            const pugi::char_t* str = var_number->get_string();
            (void)str;
        }
        if (var_bool) {
            const pugi::char_t* str = var_bool->get_string();
            (void)str;
        }
        if (var_node) {
            const pugi::char_t* str = var_node->get_string();
            (void)str;
        }

        // Test name() on valid variables
        if (var_string) {
            const pugi::char_t* n = var_string->name();
            (void)n;
        }
        if (var_number) {
            const pugi::char_t* n = var_number->name();
            (void)n;
        }
        if (var_bool) {
            const pugi::char_t* n = var_bool->name();
            (void)n;
        }
        if (var_node) {
            const pugi::char_t* n = var_node->name();
            (void)n;
        }
    }

    // Test 4: pugi::xpath_variable_set::set
    // This function expects an existing variable of the correct type (string) to set a string value.
    // We must first add a string variable.
    {
        pugi::xpath_variable_set variables;
        pugi::xpath_variable* var = variables.add(name_cstr, pugi::xpath_type_string);
        
        // Try to set the value
        // Note: The signature provided in the prompt for set is bool set(const char_t* name, const char_t* value);
        // This implies setting a string value.
        if (var) {
            bool result = variables.set(name_cstr, value_cstr);
            (void)result;
        } else {
            // If add failed (e.g. type mismatch or memory), try setting anyway (should return false)
            bool result = variables.set(name_cstr, value_cstr);
            (void)result;
        }
        
        // Try setting on a non-string variable (should return false)
        pugi::xpath_variable_set variables2;
        pugi::xpath_variable* var_num = variables2.add(name_cstr, pugi::xpath_type_number);
        if (var_num) {
            bool result = variables2.set(name_cstr, value_cstr);
            (void)result;
        }
    }

    // Cleanup
    free(name_buffer);
    free(value_buffer);

    return 0;
}