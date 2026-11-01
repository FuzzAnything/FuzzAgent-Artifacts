// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_node root = doc;
    
    // Create a simple XML structure to ensure nodes exist for traversal
    if (!doc.load_string("<root><child>data</child></root>")) {
        return 0;
    }
    root = doc.child("root");

    pugi::xpath_variable_set variables;
    
    // Create variables of different types to test all set overloads
    // Note: add() returns a pointer, so we must dereference it or store the pointer
    pugi::xpath_variable* var_string_ptr = variables.add("string_var", pugi::xpath_type_string);
    pugi::xpath_variable* var_number_ptr = variables.add("number_var", pugi::xpath_type_number);
    pugi::xpath_variable* var_boolean_ptr = variables.add("boolean_var", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_node_set_ptr = variables.add("node_set_var", pugi::xpath_type_node_set);

    // Check if variables were created successfully
    if (!var_string_ptr || !var_number_ptr || !var_boolean_ptr || !var_node_set_ptr) {
        return 0;
    }

    // 1. pugi::xpath_variable::get_node_set
    // Retrieve node set (will be empty initially as type is set but value not assigned via set)
    const pugi::xpath_node_set& ns = var_node_set_ptr->get_node_set();
    (void)ns;

    // 2. pugi::xpath_variable::set (string overload)
    // Use input data to create a string, handling potential nulls
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Replace nulls to ensure valid string for XML
    for (char &c : input_str) {
        if (c == '\0') c = ' ';
    }
    var_string_ptr->set(input_str.c_str());

    // 3. pugi::xpath_variable::set (double overload)
    double num_val = 0.0;
    if (Size >= sizeof(double)) {
        num_val = *reinterpret_cast<const double*>(Data);
    }
    var_number_ptr->set(num_val);

    // 4. pugi::xpath_variable::set (bool overload)
    bool bool_val = (Size > 0) ? (Data[0] != 0) : false;
    var_boolean_ptr->set(bool_val);

    // 5. pugi::xml_node::select_nodes
    // Use a simple query that might use variables or just traverse
    pugi::xpath_node_set result = root.select_nodes("child::*", &variables);
    (void)result;

    // 6. pugi::xpath_variable::get_boolean
    bool retrieved_bool = var_boolean_ptr->get_boolean();
    (void)retrieved_bool;

    // 7. pugi::xpath_variable::get_number
    double retrieved_num = var_number_ptr->get_number();
    (void)retrieved_num;

    // 8. pugi::xpath_variable::get_string
    const pugi::char_t* retrieved_str = var_string_ptr->get_string();
    (void)retrieved_str;

    // 9. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    (void)first;

    return 0;
}