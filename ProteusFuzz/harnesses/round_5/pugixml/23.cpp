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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xpath_variable_set var_set;

    // Create a name from the input data, ensuring it is null-terminated
    // We use a small fixed buffer to avoid stack overflow and ensure safety
    char name_buf[256];
    size_t name_len = (Size < sizeof(name_buf) - 1) ? Size : sizeof(name_buf) - 1;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';

    // Determine type based on input data to explore different states
    // We cycle through the available types to test different code paths
    pugi::xpath_value_type type = pugi::xpath_type_none;
    if (Size > 0) {
        int mod = Data[0] % 5;
        switch (mod) {
            case 0: type = pugi::xpath_type_node_set; break;
            case 1: type = pugi::xpath_type_number; break;
            case 2: type = pugi::xpath_type_string; break;
            case 3: type = pugi::xpath_type_boolean; break;
            case 4: type = pugi::xpath_type_none; break;
        }
    }

    // Step 1: pugi::xpath_variable_set::add
    // pugixml uses implicit conversion or template deduction for char_t, 
    // so we can pass the char* directly or cast explicitly to pugi::char_t*
    pugi::xpath_variable* var = var_set.add(static_cast<pugi::char_t*>(name_buf), type);

    if (!var) {
        // If add failed (e.g., invalid type or memory issue), we can't proceed with member calls
        return 0;
    }

    // Step 2: pugi::xpath_variable::type
    pugi::xpath_value_type current_type = var->type();

    // Step 3: pugi::xpath_variable::name
    const pugi::char_t* var_name = var->name();
    (void)var_name; // Use the value to suppress unused warnings

    // Step 4: pugi::xpath_variable::get_boolean
    bool bool_val = var->get_boolean();
    (void)bool_val;

    // Step 5: pugi::xpath_variable::get_number
    double num_val = var->get_number();
    (void)num_val;

    // Step 6: pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var->get_string();
    (void)str_val;

    // Cleanup is handled automatically by RAII (destructors) for pugi objects.
    // No explicit cleanup needed for the variable set or variable pointer.

    return 0;
}