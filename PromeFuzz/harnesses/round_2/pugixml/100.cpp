// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which functions to test
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    try {
        // Test xpath_variable_set::add and xpath_variable_set::get
        pugi::xpath_variable_set var_set;
        
        // Add some variables with different types
        const char* var_names[] = {"var1", "var2", "var3", "var4", "var5"};
        pugi::xpath_value_type types[] = {
            pugi::xpath_type_node_set,
            pugi::xpath_type_number,
            pugi::xpath_type_string,
            pugi::xpath_type_boolean,
            pugi::xpath_type_none
        };
        
        for (int i = 0; i < 5 && i * 2 < (int)Size; i++) {
            // Use fuzz data to determine which type to add
            uint8_t type_selector = Data[i % Size];
            pugi::xpath_variable* var = var_set.add(var_names[i], types[type_selector % 5]);
            
            if (var) {
                // Test xpath_variable::type
                pugi::xpath_value_type var_type = var->type();
                (void)var_type; // Avoid unused variable warning
                
                // Test get functions based on type
                switch (var_type) {
                    case pugi::xpath_type_node_set: {
                        const pugi::xpath_node_set& node_set = var->get_node_set();
                        (void)node_set;
                        break;
                    }
                    case pugi::xpath_type_number: {
                        double num = var->get_number();
                        (void)num;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
        // Test xpath_variable_set::get with const version
        const pugi::xpath_variable_set& const_var_set = var_set;
        for (int i = 0; i < 5; i++) {
            const pugi::xpath_variable* var = const_var_set.get(var_names[i]);
            if (var) {
                // Test the variable methods
                pugi::xpath_value_type t = var->type();
                (void)t;
                
                if (t == pugi::xpath_type_node_set) {
                    const pugi::xpath_node_set& ns = var->get_node_set();
                    (void)ns;
                } else if (t == pugi::xpath_type_number) {
                    double n = var->get_number();
                    (void)n;
                }
            }
        }
        
        // Test xpath_query::return_type
        if (Size > 0) {
            // Create a dummy XPath query string from fuzz data
            // Ensure null termination
            size_t query_len = Size > 256 ? 256 : Size;
            char query[257];
            memcpy(query, Data, query_len);
            query[query_len] = '\0';
            
            pugi::xpath_query xpath_query(query);
            pugi::xpath_value_type return_type = xpath_query.return_type();
            (void)return_type;
        }
        
        // Test copy constructors
        pugi::xpath_variable_set copied_var_set(var_set);
        pugi::xpath_node_set empty_node_set;
        pugi::xpath_node_set copied_node_set(empty_node_set);
        
        // Test get on copied set
        for (int i = 0; i < 5; i++) {
            pugi::xpath_variable* var = copied_var_set.get(var_names[i]);
            if (var) {
                pugi::xpath_value_type t = var->type();
                (void)t;
            }
        }
        
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }
    
    return 0;
}