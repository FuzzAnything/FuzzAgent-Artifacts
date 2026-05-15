// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to file for XML parsing
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    // Get root node (or empty node if parsing failed)
    pugi::xml_node root = doc.root();
    
    // Create xpath_variable_set and add variables
    pugi::xpath_variable_set vars;
    
    // Add variables of different types
    pugi::xpath_variable* var_nodeset = vars.add("var_nodeset", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = vars.add("var_boolean", pugi::xpath_type_boolean);
    
    // Create a dummy node set for the nodeset variable
    pugi::xpath_node_set dummy_node_set;
    if (var_nodeset) {
        // Note: xpath_variable::set for node_set isn't in the provided API,
        // so we'll just use the variable as-is (empty node set)
    }
    
    // 1. Call get_node_set on nodeset variable
    if (var_nodeset) {
        const pugi::xpath_node_set& node_set = var_nodeset->get_node_set();
        (void)node_set; // Use result to avoid unused variable warning
    }
    
    // 2. Call set(double) on number variable
    if (var_number) {
        var_number->set(3.14159);
    }
    
    // 3. Call set(const char_t*) on string variable
    if (var_string) {
        var_string->set("test_string");
    }
    
    // 4. Call set(bool) on boolean variable
    if (var_boolean) {
        var_boolean->set(true);
    }
    
    // 5. Call select_nodes on root node
    // Use a simple XPath query that should work even with empty documents
    pugi::xpath_node_set selected = root.select_nodes("//*", &vars);
    (void)selected; // Use result to avoid unused variable warning
    
    // 6. Call get_boolean on boolean variable
    if (var_boolean) {
        bool bool_val = var_boolean->get_boolean();
        (void)bool_val; // Use result to avoid unused variable warning
    }
    
    // 7. Call get_number on number variable
    if (var_number) {
        double num_val = var_number->get_number();
        (void)num_val; // Use result to avoid unused variable warning
    }
    
    // 8. Call get_string on string variable
    if (var_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val; // Use result to avoid unused variable warning
    }
    
    // 9. Call first_child on root node
    pugi::xml_node first_child = root.first_child();
    (void)first_child; // Use result to avoid unused variable warning
    
    // Cleanup: remove temporary file
    remove(filename);
    
    return 0;
}