// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_variable::get_boolean at pugixml.cpp:12950:36 in pugixml.hpp
// pugi::xpath_variable::get_number at pugixml.cpp:12955:38 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::get_node_set at pugixml.cpp:12966:53 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12974:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12982:36 in pugixml.hpp
// pugi::xpath_variable::set at pugixml.cpp:12990:36 in pugixml.hpp
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

static void dummy_cleanup() {
    // No specific cleanup needed for this fuzzing driver
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Use a small portion of data to avoid excessive memory usage
    size_t data_len = Size > 1024 ? 1024 : Size;
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, data_len, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node (or create dummy if parsing failed)
    pugi::xml_node root = doc.root();
    if (!root) {
        // Create a minimal XML structure if parsing failed
        const char* dummy_xml = "<root><child>text</child></root>";
        doc.load_string(dummy_xml);
        root = doc.root();
    }
    
    // Create XPath variable set and add variables
    pugi::xpath_variable_set vars;
    
    // Add variables of different types
    pugi::xpath_variable* var_node_set = vars.add("var1", pugi::xpath_type_node_set);
    pugi::xpath_variable* var_number = vars.add("var2", pugi::xpath_type_number);
    pugi::xpath_variable* var_string = vars.add("var3", pugi::xpath_type_string);
    pugi::xpath_variable* var_boolean = vars.add("var4", pugi::xpath_type_boolean);
    
    // Create a dummy node set for the node set variable
    pugi::xpath_node_set dummy_node_set;
    if (var_node_set) {
        // Manually set up a node set (this is hacky since set() doesn't accept node sets)
        // We'll just use the variable as-is for get_node_set()
    }
    
    // 1. pugi::xpath_variable::get_node_set
    if (var_node_set) {
        const pugi::xpath_node_set& node_set = var_node_set->get_node_set();
        (void)node_set; // Use result to avoid unused variable warning
    }
    
    // 2-4. pugi::xpath_variable::set (all 3 overloads)
    // We'll use different variables for different types to avoid type mismatch errors
    
    // Use first byte for boolean
    if (var_boolean) {
        bool bool_val = (data_len > 0) ? (Data[0] & 1) : false;
        var_boolean->set(bool_val);
    }
    
    // Use second and third bytes for number (simple conversion)
    if (var_number) {
        double num_val = 0.0;
        if (data_len >= 3) {
            // Safe to access Data[1] and Data[2] since we have at least 3 bytes
            num_val = static_cast<double>(Data[1]) + static_cast<double>(Data[2]) / 256.0;
        } else if (data_len == 2) {
            // Only have Data[1], use it alone
            num_val = static_cast<double>(Data[1]);
        } else if (data_len == 1) {
            // Use Data[0] if that's all we have
            num_val = static_cast<double>(Data[0]);
        }
        // If data_len == 0, num_val remains 0.0
        var_number->set(num_val);
    }
    
    // Use a portion of data for string
    if (var_string && data_len > 0) {
        // Create null-terminated string from data
        size_t str_len = (data_len > 128) ? 128 : data_len;
        char* str_buf = static_cast<char*>(malloc(str_len + 1));
        if (str_buf) {
            // Copy only the available data
            memcpy(str_buf, Data, str_len);
            str_buf[str_len] = '\0';
            var_string->set(str_buf);
            free(str_buf);
        }
    }
    
    // 5. pugi::xml_node::select_nodes
    // Create a simple XPath query
    const char* xpath_query = "//*";
    pugi::xpath_node_set selected_nodes = root.select_nodes(xpath_query, &vars);
    (void)selected_nodes; // Use result to avoid unused variable warning
    
    // 6. pugi::xpath_variable::get_boolean
    if (var_boolean) {
        bool bool_result = var_boolean->get_boolean();
        (void)bool_result;
    }
    
    // 7. pugi::xpath_variable::get_number
    if (var_number) {
        double num_result = var_number->get_number();
        (void)num_result;
    }
    
    // 8. pugi::xpath_variable::get_string
    if (var_string) {
        const pugi::char_t* str_result = var_string->get_string();
        (void)str_result;
    }
    
    // 9. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    (void)first_child;
    
    dummy_cleanup();
    return 0;
}