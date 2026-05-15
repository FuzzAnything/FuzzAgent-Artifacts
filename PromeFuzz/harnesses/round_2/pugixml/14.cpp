// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
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

static void create_dummy_xml(const uint8_t* data, size_t size, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* Data, size_t Size) {
    // Create dummy XML file for parsing
    create_dummy_xml(Data, Size, "./dummy_file");
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Create xpath_variable_set
    pugi::xpath_variable_set vars;
    
    // Add variables first (set requires existing variables)
    vars.add("str_var", pugi::xpath_type_string);
    vars.add("bool_var", pugi::xpath_type_boolean);
    vars.add("num_var", pugi::xpath_type_number);
    vars.add("nodeset_var", pugi::xpath_type_node_set);
    
    // Prepare data for set calls
    const char* str_val = "test_string";
    bool bool_val = Size > 0 ? (Data[0] & 1) : false;
    double num_val = Size > 8 ? *reinterpret_cast<const double*>(Data) : 0.0;
    
    // Create a simple xpath_node_set for testing
    pugi::xml_node root = doc.root();
    pugi::xpath_node_set nodeset;
    if (root) {
        nodeset = root.select_nodes("*");
    }
    
    // Call pugi::xpath_variable_set::set 4 times with different overloads
    // 1. String overload
    vars.set("str_var", str_val);
    
    // 2. Boolean overload
    vars.set("bool_var", bool_val);
    
    // 3. Double overload
    vars.set("num_var", num_val);
    
    // 4. xpath_node_set overload
    vars.set("nodeset_var", nodeset);
    
    // Call pugi::xml_node::select_nodes
    if (root) {
        pugi::xpath_node_set selected = root.select_nodes("//*", &vars);
        (void)selected; // Use result to avoid unused variable warning
    }
    
    // Call pugi::xpath_variable_set::get twice
    const pugi::xpath_variable* var1 = vars.get("str_var");
    const pugi::xpath_variable* var2 = vars.get("bool_var");
    (void)var1; // Use results to avoid unused variable warnings
    (void)var2;
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}