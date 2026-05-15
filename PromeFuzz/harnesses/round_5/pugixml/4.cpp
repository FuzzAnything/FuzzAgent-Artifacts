// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    
    // Try to parse input as XML, but use fallback if parsing fails
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    if (!result) {
        // If parsing fails, create a minimal valid XML document
        const char* dummy_xml = "<root><child>text</child></root>";
        doc.load_string(dummy_xml);
    }
    
    // Get root node for operations
    pugi::xml_node root = doc.root();
    
    // Create xpath_variable_set and variables
    pugi::xpath_variable_set vars;
    
    // Create variables of different types
    pugi::xpath_variable* var_node_set = vars.add("var1", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_string = vars.add("var2", pugi::xpath_type_string);
    pugi::xpath_variable* var_bool = vars.add("var3", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_number = vars.add("var4", pugi::xpath_type_number);
    
    // 1. pugi::xpath_variable::get_node_set
    if (var_node_set) {
        const pugi::xpath_node_set& node_set = var_node_set->get_node_set();
        (void)node_set; // Use result to avoid unused variable warning
    }
    
    // 2. pugi::xpath_variable::set (string)
    if (var_string && Size > 0) {
        // Use input data for string value, ensure null termination
        size_t str_len = Size > 100 ? 100 : Size;
        char* temp_str = new char[str_len + 1];
        memcpy(temp_str, Data, str_len);
        temp_str[str_len] = '\0';
        var_string->set(temp_str);
        delete[] temp_str;
    }
    
    // 3. pugi::xpath_variable::set (bool)
    if (var_bool) {
        bool bool_val = (Size % 2) == 0;
        var_bool->set(bool_val);
    }
    
    // 4. pugi::xpath_variable::set (double)
    if (var_number) {
        double num_val = 0.0;
        if (Size >= sizeof(double)) {
            memcpy(&num_val, Data, sizeof(double));
        } else if (Size > 0) {
            num_val = static_cast<double>(Data[0]);
        }
        var_number->set(num_val);
    }
    
    // 5. pugi::xml_node::select_nodes
    // Create a simple XPath query
    const char* query = "//*";
    pugi::xpath_node_set selected_nodes = root.select_nodes(query, &vars);
    (void)selected_nodes;
    
    // 6. pugi::xpath_variable::get_boolean
    if (var_bool) {
        bool bool_val = var_bool->get_boolean();
        (void)bool_val;
    }
    
    // 7. pugi::xpath_variable::get_number
    if (var_number) {
        double num_val = var_number->get_number();
        (void)num_val;
    }
    
    // 8. pugi::xpath_variable::get_string
    if (var_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val;
    }
    
    // 9. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    (void)first_child;
    
    return 0;
}