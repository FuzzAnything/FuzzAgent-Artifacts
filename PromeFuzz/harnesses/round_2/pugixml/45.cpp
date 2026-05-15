// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
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

static pugi::xpath_node_set create_dummy_node_set() {
    static pugi::xml_document doc;
    static bool initialized = false;
    if (!initialized) {
        doc.load_string("<root><node1/><node2/><node3/></root>");
        initialized = true;
    }
    
    // Use select_nodes to get an xpath_node_set
    return doc.select_nodes("/root/*");
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Create xpath_variable_set and add variables
    pugi::xpath_variable_set vars;
    
    // Add variables of different types
    pugi::xpath_variable* var_string = vars.add("var_string", pugi::xpath_type_string);
    pugi::xpath_variable* var_node_set = vars.add("var_node_set", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("var_number", pugi::xpath_type_number);
    pugi::xpath_variable* var_boolean = vars.add("var_boolean", pugi::xpath_type_boolean);
    
    if (!var_string || !var_node_set || !var_number || !var_boolean) {
        return 0;
    }
    
    // Initialize variables with dummy values
    var_string->set("initial_string");
    var_node_set->set(create_dummy_node_set());
    var_number->set(3.14159);
    var_boolean->set(true);
    
    // Target function calls in specified order
    // 1. pugi::xpath_variable::get_string
    const pugi::char_t* str_val = var_string->get_string();
    (void)str_val;
    
    // 2. pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& ns_val = var_node_set->get_node_set();
    (void)ns_val;
    
    // 3. pugi::xpath_variable::set (xpath_node_set overload)
    pugi::xpath_node_set new_ns = create_dummy_node_set();
    var_node_set->set(new_ns);
    
    // 4. pugi::xpath_variable::set (double overload)
    double num_val = 2.71828;
    var_number->set(num_val);
    
    // 5. pugi::xpath_variable::set (const char_t* overload)
    const pugi::char_t* new_str = "new_test_string";
    var_string->set(new_str);
    
    // 6. pugi::xpath_variable::set (bool overload)
    bool bool_val = false;
    var_boolean->set(bool_val);
    
    // 7. pugi::xpath_variable::get_boolean
    bool bool_result = var_boolean->get_boolean();
    (void)bool_result;
    
    // 8. pugi::xpath_variable::get_number
    double num_result = var_number->get_number();
    (void)num_result;
    
    // 9. pugi::xpath_variable::get_string
    const pugi::char_t* str_result = var_string->get_string();
    (void)str_result;
    
    // 10. pugi::xpath_variable::get_node_set
    const pugi::xpath_node_set& ns_result = var_node_set->get_node_set();
    (void)ns_result;
    
    // Additional fuzzing: use fuzz data to create string values
    if (Size > 0) {
        // Create null-terminated string from fuzz data
        pugi::char_t* fuzz_str = new pugi::char_t[Size + 1];
        for (size_t i = 0; i < Size; ++i) {
            fuzz_str[i] = static_cast<pugi::char_t>(Data[i]);
        }
        fuzz_str[Size] = 0;
        
        // Try setting string variable with fuzz data
        var_string->set(fuzz_str);
        
        // Try setting number variable with derived value
        double fuzz_num = static_cast<double>(Data[0]) / 256.0;
        var_number->set(fuzz_num);
        
        // Try setting boolean variable with derived value
        bool fuzz_bool = (Data[0] % 2) == 0;
        var_boolean->set(fuzz_bool);
        
        delete[] fuzz_str;
    }
    
    return 0;
}