// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
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
#include <cstring>
#include <cstdlib>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xpath_variable_set var_set;
    
    // Use input data to generate a variable name
    // Ensure null-termination and reasonable length
    const size_t max_name_len = 256;
    char name_buffer[max_name_len + 1];
    size_t name_len = (Size < max_name_len) ? Size : max_name_len;
    
    memcpy(name_buffer, Data, name_len);
    name_buffer[name_len] = '\0';
    
    // Determine type based on input data to explore different states
    pugi::xpath_value_type type = pugi::xpath_type_none;
    if (Size > 0) {
        uint8_t type_selector = Data[0] % 5;
        switch (type_selector) {
            case 0: type = pugi::xpath_type_node_set; break;
            case 1: type = pugi::xpath_type_number; break;
            case 2: type = pugi::xpath_type_string; break;
            case 3: type = pugi::xpath_type_boolean; break;
            case 4: type = pugi::xpath_type_none; break;
        }
    }

    // 1. pugi::xpath_variable_set::add
    pugi::xpath_variable* var = var_set.add(name_buffer, type);
    
    if (!var) {
        // If add failed (e.g., invalid type or memory issue), try again with a valid type
        var = var_set.add(name_buffer, pugi::xpath_type_string);
        if (!var) {
            return 0;
        }
    }

    // 2. pugi::xpath_variable::type
    pugi::xpath_value_type current_type = var->type();
    
    // 3. pugi::xpath_variable::name
    const pugi::char_t* name_result = var->name();
    (void)name_result; // Suppress unused variable warning

    // 4. pugi::xpath_variable::get_boolean
    bool bool_val = var->get_boolean();
    (void)bool_val;

    // 5. pugi::xpath_variable::get_number
    double num_val = var->get_number();
    (void)num_val;

    // 6. pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var->get_string();
    (void)str_val;

    // Cleanup: No explicit cleanup needed for xpath_variable_set in this context
    // as it manages its own memory and will be destroyed when out of scope.
    
    // Write dummy file if required by some internal logic (though not strictly needed for these APIs)
    write_dummy_file(Data, Size);

    return 0;
}