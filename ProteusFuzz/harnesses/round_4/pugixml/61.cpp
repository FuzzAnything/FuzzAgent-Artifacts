// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>

static void fuzz_xpath_variable_set(pugi::xpath_variable_set& variables, const uint8_t* data, size_t size) {
    if (size < 2) return;

    // Create a buffer to hold a null-terminated string from the fuzz input
    // We limit the length to avoid huge allocations and ensure null termination
    const size_t max_len = 256;
    pugi::char_t buffer[max_len + 1];
    size_t len = (size < max_len) ? size : max_len;
    
    // Copy data to buffer
    std::memcpy(buffer, data, len);
    buffer[len] = '\0';

    // Use the buffer as the variable name
    const pugi::char_t* name = buffer;

    // Fuzz add: try different types
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_none,
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean
    };

    for (int i = 0; i < 5; ++i) {
        pugi::xpath_variable* var = variables.add(name, types[i]);
        if (var) {
            // Fuzz name retrieval
            const pugi::char_t* var_name = var->name();
            (void)var_name; // Use to avoid unused warning

            // Fuzz get_string only if type is string
            if (types[i] == pugi::xpath_type_string) {
                const pugi::char_t* str = var->get_string();
                (void)str;
            }
        }
    }

    // Fuzz set with boolean-like values
    // The set function expects a string representation for the value.
    // We use "true" or "false" based on the first byte of input.
    bool bool_val = (data[0] % 2 == 0);
    const pugi::char_t* bool_str = bool_val ? "true" : "false";
    variables.set(name, bool_str);

    // Fuzz get (non-const)
    pugi::xpath_variable* var = variables.get(name);
    if (var) {
        const pugi::char_t* vname = var->name();
        (void)vname;
        const pugi::char_t* vstr = var->get_string();
        (void)vstr;
    }

    // Fuzz get (const)
    const pugi::xpath_variable_set& variables_const = variables;
    const pugi::xpath_variable* var_const = variables_const.get(name);
    if (var_const) {
        const pugi::char_t* vname_const = var_const->name();
        (void)vname_const;
        const pugi::char_t* vstr_const = var_const->get_string();
        (void)vstr_const;
    }
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xpath_variable_set variables;
    
    // Fuzz the variable set operations
    fuzz_xpath_variable_set(variables, Data, Size);

    // Move semantics test
    pugi::xpath_variable_set variables_moved(std::move(variables));
    fuzz_xpath_variable_set(variables_moved, Data, Size);

    return 0;
}