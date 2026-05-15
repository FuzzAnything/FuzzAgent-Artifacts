// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Create xpath_variable_set to manage variables
    pugi::xpath_variable_set vars;
    
    // Create variables of different types
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_node_set = vars.add("var_node_set", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_boolean = vars.add("var_boolean", pugi::xpath_type_boolean);
    
    // Verify variables were created
    if (!var_string || !var_node_set || !var_number || !var_boolean) {
        return 0;
    }
    
    // 1. pugi::xpath_variable::get_string
    if (var_string->type() == pugi::xpath_type_string) {
        var_string->get_string();
    }
    
    // 2. pugi::xpath_variable::get_node_set
    if (var_node_set->type() == pugi::xpath_type_node_set) {
        var_node_set->get_node_set();
    }
    
    // Prepare data for set operations
    pugi::xpath_node_set empty_node_set;
    
    // Use fuzzer data for numeric value
    double num_value = 0.0;
    if (Size >= sizeof(double)) {
        std::memcpy(&num_value, Data, sizeof(double));
    } else if (Size > 0) {
        num_value = static_cast<double>(Data[0]);
    }
    
    // Use fuzzer data for string value
    const char* str_value = "";
    if (Size > 0) {
        // Create null-terminated string from fuzzer data
        static char buffer[256];
        size_t copy_size = Size < 255 ? Size : 255;
        std::memcpy(buffer, Data, copy_size);
        buffer[copy_size] = '\0';
        str_value = buffer;
    }
    
    // Use fuzzer data for boolean value
    bool bool_value = false;
    if (Size > 0) {
        bool_value = (Data[0] & 1) != 0;
    }
    
    // 3. pugi::xpath_variable::set (node_set overload)
    var_node_set->set(empty_node_set);
    
    // 4. pugi::xpath_variable::set (double overload)
    var_number->set(num_value);
    
    // 5. pugi::xpath_variable::set (string overload)
    var_string->set(str_value);
    
    // 6. pugi::xpath_variable::set (bool overload)
    var_boolean->set(bool_value);
    
    // 7. pugi::xpath_variable::get_boolean
    if (var_boolean->type() == pugi::xpath_type_boolean) {
        var_boolean->get_boolean();
    }
    
    // 8. pugi::xpath_variable::get_number
    if (var_number->type() == pugi::xpath_type_number) {
        var_number->get_number();
    }
    
    // 9. pugi::xpath_variable::get_string
    if (var_string->type() == pugi::xpath_type_string) {
        var_string->get_string();
    }
    
    // 10. pugi::xpath_variable::get_node_set
    if (var_node_set->type() == pugi::xpath_type_node_set) {
        var_node_set->get_node_set();
    }
    
    return 0;
}