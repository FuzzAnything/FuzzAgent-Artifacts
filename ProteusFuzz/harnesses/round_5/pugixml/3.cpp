// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a minimal XML document
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some child nodes to make first_child non-trivial
    root.append_child("child1");
    root.append_child("child2");

    // Create variable set and variables
    pugi::xpath_variable_set variables;
    
    // Create variables of different types. 
    // The add function requires a name (const char_t*) and a type.
    pugi::xpath_variable* var_node_set = variables.add("var_node_set", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_string = variables.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_number = variables.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_boolean = variables.add("var_boolean", pugi::xpath_type_boolean);

    if (!var_node_set || !var_string || !var_number || !var_boolean) {
        return 0;
    }

    // 1. pugi::xpath_variable::get_node_set
    // Retrieve node set (will be empty initially as type is node_set but no value set)
    const pugi::xpath_node_set& empty_set = var_node_set->get_node_set();
    (void)empty_set; // Suppress unused warning

    // 2. pugi::xpath_variable::set (string overload)
    // Prepare a string from input data, ensuring null termination
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure we have a valid C-string by replacing potential nulls inside if needed, 
    // but for fuzzing, we just pass the data. We'll create a safe copy.
    char safe_string[256];
    size_t len = Size < 255 ? Size : 255;
    std::memcpy(safe_string, Data, len);
    safe_string[len] = '\0';
    
    var_string->set(safe_string);

    // 3. pugi::xpath_variable::set (number overload)
    // Extract a number from input if possible, otherwise use a default
    double num_val = 0.0;
    if (Size >= sizeof(double)) {
        std::memcpy(&num_val, Data, sizeof(double));
    } else if (Size > 0) {
        num_val = static_cast<double>(Data[0]);
    }
    var_number->set(num_val);

    // 4. pugi::xpath_variable::set (boolean overload)
    // Use first byte to determine boolean
    bool bool_val = (Size > 0 && Data[0] != 0);
    var_boolean->set(bool_val);

    // 5. pugi::xml_node::select_nodes
    // Use a simple XPath query. We can try to use variables in the query if supported,
    // but for safety and simplicity, we use a static query that works on our structure.
    // We pass the variables set to the function.
    pugi::xpath_node_set nodes = root.select_nodes("child::*", &variables);
    (void)nodes;

    // 6. pugi::xpath_variable::get_boolean
    bool retrieved_bool = var_boolean->get_boolean();
    (void)retrieved_bool;

    // 7. pugi::xpath_variable::get_number
    double retrieved_num = var_number->get_number();
    (void)retrieved_num;

    // 8. pugi::xpath_variable::get_string
    const pugi::char_t* retrieved_str = var_string->get_string();
    (void)retrieved_str;

    // 9. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    (void)first;

    return 0;
}