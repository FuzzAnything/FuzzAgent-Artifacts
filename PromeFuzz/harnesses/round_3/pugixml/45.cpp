// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>

static pugi::xml_document create_dummy_document() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    root.append_attribute("attr") = "value";
    root.append_child("child").text() = "text";
    return doc;
}

static pugi::xpath_node_set create_dummy_node_set(pugi::xml_document& doc) {
    pugi::xpath_node_set set;
    pugi::xml_node root = doc.child("root");
    if (root) {
        // Create a single node set using available constructors
        pugi::xpath_node node(root);
        // Use the move constructor as specified in the related constructors
        pugi::xpath_node_set temp;
        // We'll just return a set with one node
        // Create a temporary set and use move semantics
        pugi::xpath_node_set temp_set;
        // Since we can't use set() method, we'll create a minimal set
        // by using the default constructor and then we'll rely on
        // the move constructor for any operations
    }
    return set;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    static pugi::xml_document dummy_doc = create_dummy_document();
    static pugi::xpath_node_set dummy_set = create_dummy_node_set(dummy_doc);
    
    // Create xpath_variable_set and add variables of different types
    pugi::xpath_variable_set vars;
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_boolean = vars.add("var_boolean", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_node_set = vars.add("var_node_set", pugi::xpath_type_node_set);
    
    // Use the first variable (string type) for the sequence of calls
    pugi::xpath_variable* var = var_string;
    
    // 1. get_string
    const pugi::char_t* str_val = var->get_string();
    (void)str_val;
    
    // 2. get_node_set
    const pugi::xpath_node_set& node_set_val = var->get_node_set();
    (void)node_set_val;
    
    // 3-6. set (all 4 overloads)
    double num_val = 0.0;
    if (Size >= sizeof(double)) {
        memcpy(&num_val, Data, sizeof(double));
    }
    var->set(num_val);
    
    var->set(dummy_set);
    
    if (Size > 0) {
        // Create null-terminated string from fuzzer data
        size_t str_len = Size < 100 ? Size : 100;
        char buffer[101];
        memcpy(buffer, Data, str_len);
        buffer[str_len] = '\0';
        var->set(buffer);
    } else {
        var->set("");
    }
    
    bool bool_val = (Size > 0) ? (Data[0] & 1) : false;
    var->set(bool_val);
    
    // 7. get_boolean
    bool bool_result = var->get_boolean();
    (void)bool_result;
    
    // 8. get_number
    double num_result = var->get_number();
    (void)num_result;
    
    // 9. get_string
    const pugi::char_t* str_result = var->get_string();
    (void)str_result;
    
    // 10. get_node_set
    const pugi::xpath_node_set& node_set_result = var->get_node_set();
    (void)node_set_result;
    
    // Also test with other variable types to explore more states
    if (var_number) {
        var_number->set(num_val);
        var_number->get_number();
    }
    
    if (var_boolean) {
        var_boolean->set(bool_val);
        var_boolean->get_boolean();
    }
    
    if (var_node_set) {
        var_node_set->set(dummy_set);
        var_node_set->get_node_set();
    }
    
    return 0;
}