// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
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
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for potential file-based parsing
    write_dummy_file(Data, Size);
    
    // Parse XML from memory buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, try loading from dummy file
    if (!result) {
        doc.load_file("./dummy_file");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // First xml_node::child call
    std::string child_name1;
    if (Size > 0) {
        child_name1 = std::string(reinterpret_cast<const char*>(Data), 
                                 std::min(Size, static_cast<size_t>(100)));
        // Ensure null termination
        child_name1.push_back('\0');
    } else {
        child_name1 = "child";
    }
    pugi::xml_node child1 = root.child(child_name1.c_str());
    
    // xml_node::attribute call
    std::string attr_name;
    if (Size > 100) {
        attr_name = std::string(reinterpret_cast<const char*>(Data + 100), 
                               std::min(Size - 100, static_cast<size_t>(100)));
        attr_name.push_back('\0');
    } else {
        attr_name = "attr";
    }
    pugi::xml_attribute attr = child1.attribute(attr_name.c_str());
    
    // Second xml_node::child call
    std::string child_name2;
    if (Size > 200) {
        child_name2 = std::string(reinterpret_cast<const char*>(Data + 200), 
                                 std::min(Size - 200, static_cast<size_t>(100)));
        child_name2.push_back('\0');
    } else {
        child_name2 = "child2";
    }
    pugi::xml_node child2 = root.child(child_name2.c_str());
    
    // Create xpath_query
    std::string xpath_expr;
    if (Size > 300) {
        xpath_expr = std::string(reinterpret_cast<const char*>(Data + 300), 
                                std::min(Size - 300, static_cast<size_t>(200)));
        // Ensure null termination for xpath expression
        xpath_expr.push_back('\0');
    } else {
        xpath_expr = "//node";
    }
    
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_expr.c_str());
    } catch (...) {
        // If xpath query construction fails, use a simple valid query
        query = pugi::xpath_query("//*");
    }
    
    // Create context node for xpath evaluation
    pugi::xpath_node context(child2);
    
    // Evaluate functions in specified order
    try {
        // evaluate_boolean
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result;
        
        // evaluate_number
        double num_result = query.evaluate_number(context);
        (void)num_result;
        
        // First evaluate_string overload (returns string)
        pugi::string_t str_result = query.evaluate_string(context);
        (void)str_result;
        
        // Second evaluate_string overload (writes to buffer)
        char buffer[256];
        size_t str_len = query.evaluate_string(buffer, sizeof(buffer), context);
        (void)str_len;
        
        // evaluate_node_set
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        (void)node_set;
        
        // evaluate_node
        pugi::xpath_node single_node = query.evaluate_node(context);
        (void)single_node;
        
    } catch (...) {
        // Catch all exceptions to prevent crashes during fuzzing
    }
    
    return 0;
}