// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
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

static void test_xpath_variable_functions(pugi::xpath_variable* var) {
    if (var) {
        // Test type() function
        pugi::xpath_value_type var_type = var->type();
        
        // Test get_number() based on type
        if (var_type == pugi::xpath_type_number) {
            double num = var->get_number();
            (void)num; // Use result to avoid unused variable warning
        }
        
        // Test get_node_set() based on type
        if (var_type == pugi::xpath_type_node_set) {
            const pugi::xpath_node_set& node_set = var->get_node_set();
            (void)node_set; // Use result to avoid unused variable warning
        }
    }
}

static void test_xpath_query_functions(const pugi::xpath_query& query) {
    // Test return_type() function
    pugi::xpath_value_type return_type = query.return_type();
    (void)return_type; // Use result to avoid unused variable warning
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create xpath_variable_set
    pugi::xpath_variable_set variables;
    
    // Use fuzzer input to determine variable types to add
    uint8_t var_type_selector = Data[0] % 5;
    pugi::xpath_value_type types_to_test[] = {
        pugi::xpath_type_boolean,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_node_set,
        pugi::xpath_type_none
    };
    
    // Add variables with different types
    pugi::xpath_variable* var1 = variables.add("var1", types_to_test[var_type_selector]);
    pugi::xpath_variable* var2 = variables.add("var2", pugi::xpath_type_number);
    pugi::xpath_variable* var3 = variables.add("var3", pugi::xpath_type_node_set);
    
    // Test get() function
    pugi::xpath_variable* retrieved_var = variables.get("var1");
    (void)retrieved_var; // Use result to avoid unused variable warning
    
    // Test with const version
    const pugi::xpath_variable_set variables_const = variables;
    const pugi::xpath_variable* var_const = variables_const.get("var2");
    (void)var_const; // Use result to avoid unused variable warning
    
    // Test xpath_variable functions
    test_xpath_variable_functions(var1);
    test_xpath_variable_functions(var2);
    test_xpath_variable_functions(var3);
    
    // Create xpath_query with variables
    try {
        // Use part of fuzzer input as query string (ensure null termination)
        size_t query_len = Size > 100 ? 100 : Size;
        std::string query_str(reinterpret_cast<const char*>(Data), query_len);
        
        pugi::xpath_query query(query_str.c_str(), &variables);
        
        // Test xpath_query functions
        test_xpath_query_functions(query);
    } catch (...) {
        // Handle exceptions from invalid query compilation
    }
    
    // Test copy constructors
    pugi::xpath_node_set node_set_copy;
    pugi::xpath_variable_set var_set_copy(variables);
    
    return 0;
}