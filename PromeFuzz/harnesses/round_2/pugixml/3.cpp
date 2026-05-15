// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Parse XML from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Create xpath variable set and add variables of different types
    pugi::xpath_variable_set vars;
    pugi::xpath_variable* var_nodeset = vars.add("var_nodeset", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_bool = vars.add("var_bool", pugi::xpath_type_boolean);
    
    // Create a dummy node set for the node set variable
    pugi::xpath_node_set dummy_node_set;
    // Note: We can't directly set node set via xpath_variable::set, 
    // so we'll use get_node_set on an empty node set variable
    
    // 1. pugi::xpath_variable::get_node_set
    if (var_nodeset) {
        const pugi::xpath_node_set& node_set = var_nodeset->get_node_set();
        (void)node_set; // Use to avoid unused variable warning
    }
    
    // 2. pugi::xpath_variable::set (double)
    if (var_number) {
        double num_value = Size > 0 ? static_cast<double>(Data[0]) : 0.0;
        var_number->set(num_value);
    }
    
    // 3. pugi::xpath_variable::set (const char_t*)
    if (var_string) {
        static const pugi::char_t dummy_str[] = PUGIXML_TEXT("dummy");
        var_string->set(dummy_str);
    }
    
    // 4. pugi::xpath_variable::set (bool)
    if (var_bool) {
        bool bool_value = Size > 0;
        var_bool->set(bool_value);
    }
    
    // 5. pugi::xml_node::select_nodes
    if (result) {
        pugi::xml_node root = doc.document_element();
        if (root) {
            // Use a simple XPath query
            pugi::xpath_node_set selected = root.select_nodes(PUGIXML_TEXT("*"), &vars);
            (void)selected; // Use to avoid unused variable warning
        }
    }
    
    // 6. pugi::xpath_variable::get_boolean
    if (var_bool) {
        bool bool_val = var_bool->get_boolean();
        (void)bool_val; // Use to avoid unused variable warning
    }
    
    // 7. pugi::xpath_variable::get_number
    if (var_number) {
        double num_val = var_number->get_number();
        (void)num_val; // Use to avoid unused variable warning
    }
    
    // 8. pugi::xpath_variable::get_string
    if (var_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val; // Use to avoid unused variable warning
    }
    
    // 9. pugi::xml_node::first_child
    if (result) {
        pugi::xml_node root = doc.document_element();
        if (root) {
            pugi::xml_node first_child = root.first_child();
            (void)first_child; // Use to avoid unused variable warning
        }
    }
    
    // Cleanup
    remove("./dummy_file");
    
    return 0;
}