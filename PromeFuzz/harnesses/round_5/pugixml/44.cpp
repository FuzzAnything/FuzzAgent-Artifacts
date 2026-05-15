// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>

static pugi::xpath_node_set create_node_set() {
    pugi::xml_document doc;
    doc.load_string("<root><a/><b/><c/></root>");
    
    pugi::xpath_node_set result;
    pugi::xpath_query query("//*");
    result = query.evaluate_node_set(doc);
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Create a dummy string from fuzz data
    const char* dummy_string = "";
    if (Size > 0) {
        static char buffer[4096];
        size_t copy_size = Size < sizeof(buffer) - 1 ? Size : sizeof(buffer) - 1;
        memcpy(buffer, Data, copy_size);
        buffer[copy_size] = '\0';
        dummy_string = buffer;
    }
    
    // Create xpath_variable_set to manage variables
    pugi::xpath_variable_set vars;
    
    // Create variables with different types
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_node_set = vars.add("var_node_set", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_boolean = vars.add("var_boolean", pugi::xpath_type_boolean);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    
    // 1. get_string on string variable
    if (var_string && var_string->type() == pugi::xpath_type_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val;
    }
    
    // 2. get_node_set on node_set variable
    if (var_node_set && var_node_set->type() == pugi::xpath_type_node_set) {
        const pugi::xpath_node_set& node_set_val = var_node_set->get_node_set();
        (void)node_set_val;
    }
    
    // 3-6. set functions with different types
    if (var_string) {
        var_string->set(dummy_string);
    }
    
    if (var_boolean) {
        bool bool_val = Size > 0 ? (Data[0] & 1) : false;
        var_boolean->set(bool_val);
    }
    
    if (var_number) {
        double num_val = Size > 0 ? static_cast<double>(Data[0]) : 0.0;
        var_number->set(num_val);
    }
    
    if (var_node_set) {
        pugi::xpath_node_set node_set = create_node_set();
        var_node_set->set(node_set);
    }
    
    // 7. get_boolean on boolean variable
    if (var_boolean && var_boolean->type() == pugi::xpath_type_boolean) {
        bool bool_val = var_boolean->get_boolean();
        (void)bool_val;
    }
    
    // 8. get_number on number variable
    if (var_number && var_number->type() == pugi::xpath_type_number) {
        double num_val = var_number->get_number();
        (void)num_val;
    }
    
    // 9. get_string on string variable (after set)
    if (var_string && var_string->type() == pugi::xpath_type_string) {
        const pugi::char_t* str_val = var_string->get_string();
        (void)str_val;
    }
    
    // 10. get_node_set on node_set variable (after set)
    if (var_node_set && var_node_set->type() == pugi::xpath_type_node_set) {
        const pugi::xpath_node_set& node_set_val = var_node_set->get_node_set();
        (void)node_set_val;
    }
    
    return 0;
}