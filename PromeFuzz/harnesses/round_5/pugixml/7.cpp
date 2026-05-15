// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static pugi::xpath_value_type get_xpath_type_from_data(const uint8_t* data, size_t size) {
    if (size == 0) return pugi::xpath_type_boolean;
    uint8_t val = data[0] % 5;
    switch (val) {
        case 0: return pugi::xpath_type_node_set;
        case 1: return pugi::xpath_type_number;
        case 2: return pugi::xpath_type_string;
        case 3: return pugi::xpath_type_boolean;
        default: return pugi::xpath_type_none;
    }
}

static const char* get_variable_name_from_data(const uint8_t* data, size_t size) {
    static char name[32];
    if (size == 0) return "var";
    
    size_t name_len = (size < 31) ? size : 31;
    for (size_t i = 0; i < name_len; i++) {
        char c = data[i] % 62;
        if (c < 10) name[i] = '0' + c;
        else if (c < 36) name[i] = 'a' + (c - 10);
        else if (c < 62) name[i] = 'A' + (c - 36);
        else name[i] = '_';
    }
    name[name_len] = '\0';
    return name;
}

static std::string get_xpath_query_from_data(const uint8_t* data, size_t size) {
    if (size == 0) return "//node";
    
    std::string query;
    for (size_t i = 0; i < size; i++) {
        char c = data[i];
        if (c >= 32 && c <= 126) {
            query.push_back(c);
        } else {
            query.push_back(' ');
        }
        if (query.length() >= 256) break;
    }
    return query;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split data for different purposes
    size_t split1 = Size / 3;
    size_t split2 = 2 * Size / 3;
    
    const uint8_t* xml_data = Data;
    size_t xml_size = split1;
    
    const uint8_t* var_data = Data + split1;
    size_t var_size = (split2 > split1) ? (split2 - split1) : 1;
    
    const uint8_t* xpath_data = Data + split2;
    size_t xpath_size = Size - split2;
    
    // Step 1: Create xpath_variable_set and add a variable
    pugi::xpath_variable_set vars;
    const char* var_name = get_variable_name_from_data(var_data, var_size);
    pugi::xpath_value_type var_type = get_xpath_type_from_data(var_data, var_size);
    
    pugi::xpath_variable* var = vars.add(var_name, var_type);
    (void)var; // Variable handle not used further but add was called
    
    // Step 2: Load XML document from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_data, xml_size);
    
    // Only proceed if parsing was somewhat successful or we want to test error paths
    if (result.status != pugi::status_ok && result.status != pugi::status_bad_start_element &&
        result.status != pugi::status_bad_attribute && result.status != pugi::status_bad_end_element) {
        // For severe errors like out of memory, return early
        return 0;
    }
    
    // Step 3-7: Create xpath_query and evaluate it in different ways
    std::string xpath_query_str = get_xpath_query_from_data(xpath_data, xpath_size);
    
    try {
        pugi::xpath_query query(xpath_query_str.c_str(), &vars);
        
        // Create a context node (use document root)
        pugi::xml_node root = doc.root();
        pugi::xpath_node context(root);
        
        // Step 3: Evaluate as boolean
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result;
        
        // Step 4: Evaluate as number
        double num_result = query.evaluate_number(context);
        (void)num_result;
        
        // Step 5: Evaluate as string
        pugi::string_t str_result = query.evaluate_string(context);
        (void)str_result;
        
        // Step 6: Evaluate as node
        pugi::xpath_node node_result = query.evaluate_node(context);
        (void)node_result;
        
        // Step 7: Evaluate as node set
        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
        (void)node_set_result;
        
    } catch (const std::bad_alloc&) {
        // Out of memory - just return
    } catch (const std::exception&) {
        // Expected for invalid XPath queries and other errors
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}