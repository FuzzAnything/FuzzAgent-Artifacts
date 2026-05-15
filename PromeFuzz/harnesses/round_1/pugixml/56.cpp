// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include "pugixml.hpp"

static void test_xpath_variable_functions(pugi::xpath_variable* var, const pugi::xpath_node_set& node_set) {
    // Test type() function
    pugi::xpath_value_type var_type = var->type();
    
    // Test set() function with node set
    bool set_result = var->set(node_set);
    
    // Test get_node_set() function
    const pugi::xpath_node_set& retrieved_set = var->get_node_set();
    
    // Test get_string() function
    const pugi::char_t* str_value = var->get_string();
    
    // Test name() function
    const pugi::char_t* var_name = var->name();
    
    // Test get_boolean() function
    bool bool_value = var->get_boolean();
    
    // Use the results to avoid compiler warnings
    (void)var_type;
    (void)set_result;
    (void)retrieved_set;
    (void)str_value;
    (void)var_name;
    (void)bool_value;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    // Create dummy XML document for testing
    pugi::xml_document doc;
    
    // Try to parse the fuzzer input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Create xpath_node_set from the document
    pugi::xpath_node_set node_set;
    
    if (result) {
        // If parsing succeeded, create a node set with the document root
        pugi::xml_node root = doc.document_element();
        if (!root) {
            root = doc.append_child("root");
        }
        
        // Create xpath_node from the root
        pugi::xpath_node xpath_node(root);
        // Create node set with the single node using array constructor
        pugi::xpath_node nodes[] = {xpath_node};
        node_set = pugi::xpath_node_set(nodes, nodes + 1);
    } else {
        // If parsing failed, create an empty node set
        // Create a dummy document with a root node
        pugi::xml_document dummy_doc;
        pugi::xml_node root = dummy_doc.append_child("root");
        pugi::xpath_node xpath_node(root);
        // Create node set with the single node using array constructor
        pugi::xpath_node nodes[] = {xpath_node};
        node_set = pugi::xpath_node_set(nodes, nodes + 1);
    }
    
    // Create xpath_variable_set and add variables of different types
    pugi::xpath_variable_set vars;
    
    // Use add() method to create variables of specific types
    pugi::xpath_variable* node_set_var = vars.add("node_set_var", pugi::xpath_type_node_set);
    if (node_set_var) {
        test_xpath_variable_functions(node_set_var, node_set);
    }
    
    // Test with other variable types to explore different code paths
    pugi::xpath_variable* bool_var = vars.add("bool_var", pugi::xpath_type_boolean);
    if (bool_var) {
        // Test with boolean variable
        test_xpath_variable_functions(bool_var, node_set);
    }
    
    pugi::xpath_variable* string_var = vars.add("string_var", pugi::xpath_type_string);
    if (string_var) {
        // Test with string variable
        test_xpath_variable_functions(string_var, node_set);
    }
    
    pugi::xpath_variable* number_var = vars.add("number_var", pugi::xpath_type_number);
    if (number_var) {
        // Test with number variable
        test_xpath_variable_functions(number_var, node_set);
    }
    
    // Try to get variables by name and test them
    pugi::xpath_variable* retrieved_var = vars.get("node_set_var");
    if (retrieved_var) {
        test_xpath_variable_functions(retrieved_var, node_set);
    }
    
    // Create another node set with different properties
    pugi::xpath_node_set empty_set;
    pugi::xpath_node_set single_node_set = node_set;
    
    // Test setting with empty node set
    if (node_set_var) {
        node_set_var->set(empty_set);
    }
    
    // Test setting with the original node set again
    if (node_set_var) {
        node_set_var->set(single_node_set);
    }
    
    return 0;
}