// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:13011:36 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
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

static pugi::xml_document create_dummy_doc() {
    pugi::xml_document doc;
    const char* xml_content = "<root><elem attr=\"value\">text</elem></root>";
    doc.load_string(xml_content);
    return doc;
}

static pugi::xpath_node_set create_dummy_node_set() {
    static pugi::xml_document doc = create_dummy_doc();
    pugi::xml_node node = doc.child("root");
    pugi::xml_attribute attr = node.first_attribute();
    pugi::xpath_node xnode(attr, node);
    
    // Create array of xpath_node and use iterator constructor
    pugi::xpath_node nodes_array[] = {xnode};
    pugi::xpath_node_set nodes(nodes_array, nodes_array + 1);
    return nodes;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Initialize a variable set and create a variable
    pugi::xpath_variable_set vars;
    
    // Use first byte to determine initial variable type
    if (Size == 0) return 0;
    uint8_t type_selector = Data[0];
    
    // Create variable with initial type based on fuzz input
    pugi::xpath_variable* var = nullptr;
    switch (type_selector % 4) {
        case 0:
            var = vars.add("test_var", pugi::xpath_type_boolean);
            if (var) var->set(false);
            break;
        case 1:
            var = vars.add("test_var", pugi::xpath_type_number);
            if (var) var->set(0.0);
            break;
        case 2:
            var = vars.add("test_var", pugi::xpath_type_string);
            if (var && Size > 1) {
                // Use up to 100 bytes for string, ensure null termination
                size_t str_len = (Size - 1) > 100 ? 100 : (Size - 1);
                char* str = new char[str_len + 1];
                memcpy(str, Data + 1, str_len);
                str[str_len] = '\0';
                var->set(str);
                delete[] str;
            } else if (var) {
                var->set("");
            }
            break;
        case 3:
            var = vars.add("test_var", pugi::xpath_type_node_set);
            if (var) {
                pugi::xpath_node_set nodes = create_dummy_node_set();
                var->set(nodes);
            }
            break;
    }
    
    if (!var) return 0;
    
    // Execute the sequence of API calls as specified
    try {
        // 1. pugi::xpath_variable::get_string
        const pugi::char_t* str_val = var->get_string();
        (void)str_val;
        
        // 2. pugi::xpath_variable::get_node_set
        const pugi::xpath_node_set& nodes1 = var->get_node_set();
        (void)nodes1;
        
        // 3-6. pugi::xpath_variable::set (all 4 overloads)
        // Use fuzz data to determine values, with bounds checking
        size_t data_index = 1;
        
        // bool set
        if (data_index < Size) {
            bool bool_val = Data[data_index] & 0x01;
            var->set(bool_val);
            data_index++;
        }
        
        // xpath_node_set set
        pugi::xpath_node_set dummy_nodes = create_dummy_node_set();
        var->set(dummy_nodes);
        
        // string set
        if (data_index < Size) {
            size_t remaining = Size - data_index;
            size_t str_len = remaining > 50 ? 50 : remaining;
            char* temp_str = new char[str_len + 1];
            memcpy(temp_str, Data + data_index, str_len);
            temp_str[str_len] = '\0';
            var->set(temp_str);
            delete[] temp_str;
            data_index += str_len;
        }
        
        // double set
        if (data_index + sizeof(double) <= Size) {
            double double_val;
            memcpy(&double_val, Data + data_index, sizeof(double));
            var->set(double_val);
            data_index += sizeof(double);
        } else if (data_index < Size) {
            // Use remaining bytes as double representation
            double double_val = 0.0;
            size_t copy_len = Size - data_index;
            if (copy_len > sizeof(double)) copy_len = sizeof(double);
            memcpy(&double_val, Data + data_index, copy_len);
            var->set(double_val);
        }
        
        // 7. pugi::xpath_variable::get_boolean
        bool bool_val = var->get_boolean();
        (void)bool_val;
        
        // 8. pugi::xpath_variable::get_number
        double num_val = var->get_number();
        (void)num_val;
        
        // 9. pugi::xpath_variable::get_string
        const pugi::char_t* str_val2 = var->get_string();
        (void)str_val2;
        
        // 10. pugi::xpath_variable::get_node_set
        const pugi::xpath_node_set& nodes2 = var->get_node_set();
        (void)nodes2;
        
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }
    
    return 0;
}