// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13193:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13175:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential XML parsing
    write_dummy_file(Data, Size);
    
    try {
        // Create xpath_variable_set
        pugi::xpath_variable_set vars;
        
        // Add variables first (since set only works on existing variables)
        // We need to add variables with appropriate types before setting them
        vars.add("var_nodeset", pugi::xpath_type_node_set);
        vars.add("var_bool", pugi::xpath_type_boolean);
        vars.add("var_string", pugi::xpath_type_string);
        vars.add("var_number", pugi::xpath_type_number);
        
        // Create dummy xpath_node_set for the first set call
        pugi::xml_document doc;
        pugi::xml_node node = doc.append_child("root");
        pugi::xpath_node xnode(node);
        pugi::xpath_node_set nodeset(&xnode, &xnode + 1);
        
        // Call set functions in order with fuzz data
        // 1. set with xpath_node_set
        vars.set("var_nodeset", nodeset);
        
        // 2. set with bool - use first byte as boolean
        bool bool_val = Size > 0 ? (Data[0] & 1) : false;
        vars.set("var_bool", bool_val);
        
        // 3. set with string - create null-terminated string from data
        std::string str_data;
        if (Size > 0) {
            size_t str_len = Size > 100 ? 100 : Size; // Limit string length
            str_data.assign(reinterpret_cast<const char*>(Data), str_len);
            // Ensure null termination
            str_data.push_back('\0');
            vars.set("var_string", str_data.c_str());
        } else {
            vars.set("var_string", "");
        }
        
        // 4. set with double - interpret bytes as double if enough data
        double double_val = 0.0;
        if (Size >= sizeof(double)) {
            memcpy(&double_val, Data, sizeof(double));
        } else if (Size > 0) {
            // Use first byte as double if not enough data
            double_val = static_cast<double>(Data[0]);
        }
        vars.set("var_number", double_val);
        
        // Create XML document and node for select_nodes
        pugi::xml_document doc2;
        const char* xml_content = "<root><a>test</a><b>value</b></root>";
        pugi::xml_parse_result result = doc2.load_string(xml_content);
        
        if (result) {
            pugi::xml_node root = doc2.root();
            
            // Create XPath query from fuzz data
            std::string xpath_query;
            if (Size > 0) {
                size_t query_len = Size > 50 ? 50 : Size;
                xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
                // Ensure null termination
                xpath_query.push_back('\0');
            } else {
                xpath_query = "//*";
            }
            
            // 5. Call select_nodes
            pugi::xpath_node_set selected = root.select_nodes(xpath_query.c_str(), &vars);
            
            // 6. First get call
            pugi::xpath_variable* var1 = vars.get("var_string");
            
            // 7. Second get call  
            pugi::xpath_variable* var2 = vars.get("var_number");
            
            // Use const version as shown in hint
            const pugi::xpath_variable_set& vars_const = vars;
            const pugi::xpath_variable* var_const = vars_const.get("var_bool");
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}