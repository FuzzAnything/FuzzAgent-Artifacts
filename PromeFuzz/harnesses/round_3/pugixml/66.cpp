// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Use a small subset of data for variable names and query strings
    size_t name_len = Size > 0 ? (Data[0] % 32) + 1 : 1;
    if (name_len > Size) name_len = Size;
    
    // Create variable name from input data
    std::string var_name;
    if (name_len > 0) {
        var_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (!var_name.empty()) {
            var_name[name_len-1] = '\0';
        }
    } else {
        var_name = "var";
    }
    
    // Use remaining data for query string if any
    std::string query_str;
    if (Size > name_len) {
        size_t query_len = Size - name_len;
        if (query_len > 256) query_len = 256; // Limit query length
        query_str.assign(reinterpret_cast<const char*>(Data + name_len), query_len);
        if (!query_str.empty()) {
            query_str[query_len-1] = '\0';
        }
    } else {
        query_str = "//node";
    }
    
    // Test pugi::xpath_variable_set::add
    pugi::xpath_variable_set var_set;
    pugi::xpath_value_type types[] = {
        pugi::xpath_type_node_set,
        pugi::xpath_type_number,
        pugi::xpath_type_string,
        pugi::xpath_type_boolean
    };
    
    for (int i = 0; i < 4; ++i) {
        pugi::xpath_variable* var = var_set.add(var_name.c_str(), types[i]);
        if (var) {
            // Test pugi::xpath_variable::type
            pugi::xpath_value_type var_type = var->type();
            
            // Test pugi::xpath_variable::get_number
            double num = var->get_number();
            (void)num; // Use result to avoid unused variable warning
            
            // Test pugi::xpath_variable::get_node_set
            const pugi::xpath_node_set& node_set = var->get_node_set();
            (void)node_set; // Use result
            
            // Test pugi::xpath_variable_set::get
            pugi::xpath_variable* retrieved_var = var_set.get(var_name.c_str());
            (void)retrieved_var;
            
            // Test const version of get
            const pugi::xpath_variable_set& const_var_set = var_set;
            const pugi::xpath_variable* const_var = const_var_set.get(var_name.c_str());
            (void)const_var;
        }
    }
    
    // Test pugi::xpath_query::return_type
    try {
        pugi::xpath_query query(query_str.c_str());
        pugi::xpath_value_type return_type = query.return_type();
        (void)return_type;
    } catch (...) {
        // Ignore exceptions from invalid queries
    }
    
    // Test with empty variable set
    pugi::xpath_variable_set empty_set;
    pugi::xpath_variable* non_existent = empty_set.get("non_existent");
    (void)non_existent;
    
    // Test copy constructors
    pugi::xpath_variable_set copied_set(var_set);
    pugi::xpath_node_set empty_node_set;
    pugi::xpath_node_set copied_node_set(empty_node_set);
    
    return 0;
}