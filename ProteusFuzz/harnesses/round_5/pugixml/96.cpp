// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
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

// Disable exceptions for pugixml to prevent crashes during fuzzing
// This macro must be defined before including pugixml.hpp in a real build,
// but since we are linking against a pre-built library, we assume the library
// might have been built with exceptions enabled. We handle exceptions manually.

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xpath_variable_set variables;
    
    // Convert input data to a valid string for variable names and queries
    // Ensure null termination and handle potential invalid characters
    std::size_t max_len = Size < 256 ? Size : 256;
    char buffer[257];
    std::memcpy(buffer, Data, max_len);
    buffer[max_len] = '\0';

    // Fuzzing logic for pugi::xpath_variable_set::add
    // Try adding variables with different types
    pugi::xpath_variable* var1 = variables.add(buffer, pugi::xpath_type_string);
    pugi::xpath_variable* var2 = variables.add(buffer, pugi::xpath_type_number);
    pugi::xpath_variable* var3 = variables.add(buffer, pugi::xpath_type_boolean);
    pugi::xpath_variable* var4 = variables.add(buffer, pugi::xpath_type_node_set);

    // Fuzzing logic for pugi::xpath_variable_set::get
    pugi::xpath_variable* retrieved = variables.get(buffer);
    
    // Test const overload for get
    const pugi::xpath_variable_set& variables_const_ref = variables;
    const pugi::xpath_variable* var_const = variables_const_ref.get(buffer);
    
    // Fuzzing logic for pugi::xpath_variable methods
    if (var1) {
        pugi::xpath_value_type type1 = var1->type();
        const pugi::char_t* name1 = var1->name();
        const pugi::char_t* str1 = var1->get_string();
        
        // Check consistency
        if (type1 == pugi::xpath_type_string) {
            (void)str1; // Use the string value
        }
    }
    
    if (var2) {
        pugi::xpath_value_type type2 = var2->type();
        const pugi::char_t* name2 = var2->name();
        (void)type2;
        (void)name2;
    }

    if (var3) {
        pugi::xpath_value_type type3 = var3->type();
        const pugi::char_t* name3 = var3->name();
        (void)type3;
        (void)name3;
    }

    if (var4) {
        pugi::xpath_value_type type4 = var4->type();
        const pugi::char_t* name4 = var4->name();
        (void)type4;
        (void)name4;
    }

    // Fuzzing logic for pugi::xpath_query::return_type
    // Wrap in try-catch to handle potential exceptions from invalid XPath expressions
    try {
        pugi::xpath_query query(buffer, &variables);
        pugi::xpath_value_type ret_type = query.return_type();
        (void)ret_type;
    } catch (...) {
        // Ignore exceptions from invalid queries
    }

    // Cleanup is handled by destructors of local objects
    return 0;
}