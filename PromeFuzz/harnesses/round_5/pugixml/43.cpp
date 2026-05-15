// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
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
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML from memory buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // Prepare string names for child/attribute lookups
    // Use first few bytes of input as name strings (ensure null termination)
    std::string child_name;
    std::string attr_name;
    
    if (Size > 0) {
        size_t name_len = std::min(Size, (size_t)50);
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (child_name.size() > 0) {
            child_name[child_name.size() - 1] = '\0';
        }
        
        if (Size > 1) {
            size_t attr_len = std::min(Size - 1, (size_t)50);
            attr_name.assign(reinterpret_cast<const char*>(Data + 1), attr_len);
            if (attr_name.size() > 0) {
                attr_name[attr_name.size() - 1] = '\0';
            }
        }
    }
    
    // 1. First child() call
    pugi::xml_node child1 = root.child(child_name.c_str());
    
    // 2. attribute() call on child1
    pugi::xml_attribute attr = child1.attribute(attr_name.c_str());
    
    // 3. Second child() call on child1
    pugi::xml_node child2 = child1.child(child_name.c_str());
    
    // Prepare XPath query
    // Use a simple XPath expression from input data
    std::string xpath_expr = "//*";
    if (Size > 10) {
        size_t expr_len = std::min(Size - 10, (size_t)100);
        xpath_expr.assign(reinterpret_cast<const char*>(Data + 10), expr_len);
        // Ensure null termination and basic validity
        if (xpath_expr.size() > 0) {
            xpath_expr[xpath_expr.size() - 1] = '\0';
        }
    }
    
    try {
        pugi::xpath_query query(xpath_expr.c_str());
        
        // Create xpath_node context from child2
        pugi::xpath_node context(child2);
        
        // 4. evaluate_boolean()
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result; // Suppress unused warning
        
        // 5. evaluate_number()
        double num_result = query.evaluate_number(context);
        (void)num_result; // Suppress unused warning
        
        // 6. First evaluate_string() overload
        pugi::string_t str_result = query.evaluate_string(context);
        (void)str_result; // Suppress unused warning
        
        // 7. Second evaluate_string() overload with buffer
        // Use pugi::char_t which is the correct character type for pugixml
        pugi::char_t buffer[256];
        size_t str_len = query.evaluate_string(buffer, sizeof(buffer)/sizeof(pugi::char_t), context);
        (void)str_len; // Suppress unused warning
        
        // 8. evaluate_node_set()
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        (void)node_set; // Suppress unused warning
        
        // 9. evaluate_node()
        pugi::xpath_node node_result = query.evaluate_node(context);
        (void)node_result; // Suppress unused warning
        
    } catch (const std::exception& e) {
        // Handle standard exceptions (including xpath_exception if derived from std::exception)
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}