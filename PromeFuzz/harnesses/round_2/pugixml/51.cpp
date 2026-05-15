// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
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
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    std::ofstream file("./dummy_file", std::ios::binary);
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Initialize XML document and parse dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Create xpath_variable_set for testing
    pugi::xpath_variable_set variables;
    pugi::xpath_variable_set variables2;
    
    // Create test XML node
    pugi::xml_node node = doc.root();
    
    // Test xpath_variable_set::add with various types
    static const char* var_names[] = {"var1", "var2", "var3", "var4", "var5"};
    static pugi::xpath_value_type types[] = {
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean,
        pugi::xpath_type_none
    };
    
    // Add variables with different names and types
    for (int i = 0; i < 5 && i < static_cast<int>(Size); i++) {
        pugi::xpath_variable* var = variables.add(var_names[i], types[i % 5]);
        
        // Test xpath_variable_set::get - non-const version
        if (var) {
            pugi::xpath_variable* retrieved = variables.get(var_names[i]);
            
            // Test xpath_variable_set::set based on type
            if (retrieved) {
                if (types[i % 5] == pugi::xpath_type_boolean) {
                    bool bool_val = (Data[i] % 2) == 0;
                    variables.set(var_names[i], bool_val);
                } else if (types[i % 5] == pugi::xpath_type_string) {
                    const char* str_val = "test_string";
                    variables.set(var_names[i], str_val);
                }
            }
        }
    }
    
    // Test xpath_variable_set::get - const version
    const pugi::xpath_variable_set& const_variables = variables;
    for (int i = 0; i < 5 && i < static_cast<int>(Size); i++) {
        const pugi::xpath_variable* const_var = const_variables.get(var_names[i]);
        (void)const_var; // Avoid unused variable warning
    }
    
    // Test xml_node::select_nodes with various queries
    static const char* queries[] = {
        "//node",
        "/root",
        "child::*",
        "@*",
        "text()"
    };
    
    for (int i = 0; i < 5 && i < static_cast<int>(Size); i++) {
        pugi::xpath_node_set node_set = node.select_nodes(queries[i % 5], &variables);
        
        // Test copy constructor of xpath_node_set
        pugi::xpath_node_set copied_set(node_set);
        (void)copied_set; // Avoid unused variable warning
        
        // Test with null variables
        pugi::xpath_node_set node_set2 = node.select_nodes(queries[i % 5], nullptr);
    }
    
    // Test xpath_variable_set::set with boolean values
    for (int i = 0; i < 3 && i < static_cast<int>(Size); i++) {
        variables.set(var_names[i], (Data[i] % 2) == 1);
    }
    
    // Test xpath_variable_set::set with string values
    for (int i = 0; i < 3 && i < static_cast<int>(Size); i++) {
        const char* test_str = "fuzz_value";
        variables.set(var_names[i], test_str);
    }
    
    // Test move constructor of xpath_variable_set
    pugi::xpath_variable_set moved_variables(std::move(variables2));
    
    // Test with empty node
    pugi::xml_node empty_node;
    pugi::xpath_node_set empty_set = empty_node.select_nodes("//*", &variables);
    
    // Test xpath_node constructor
    if (node.first_attribute()) {
        pugi::xpath_node xnode(node.first_attribute(), node);
        (void)xnode; // Avoid unused variable warning
    }
    
    return 0;
}