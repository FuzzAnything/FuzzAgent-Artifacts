// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
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
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>

static std::string create_temp_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
    return "./dummy_file";
}

static pugi::xml_document load_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try loading from buffer first
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // If buffer loading fails, try loading from file
    if (!result) {
        std::string filename = create_temp_file(data, size);
        result = doc.load_file(filename.c_str());
    }
    
    return doc;
}

static pugi::xpath_query create_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) return pugi::xpath_query("*");
    
    // Create null-terminated string from input data
    std::string query_str(reinterpret_cast<const char*>(data), size);
    if (query_str.empty()) return pugi::xpath_query("*");
    
    return pugi::xpath_query(query_str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    try {
        // Load XML document
        pugi::xml_document doc = load_document(Data, Size);
        pugi::xml_node root = doc.root();
        
        // Create XPath query
        pugi::xpath_query query = create_xpath_query(Data, Size);
        
        // Create variable set
        pugi::xpath_variable_set vars;
        
        // 1. First select_node with char* overload
        pugi::xpath_node result1 = root.select_node("*", &vars);
        
        // 2. Second select_node with xpath_query overload
        pugi::xpath_node result2 = root.select_node(query);
        
        // 3. First node() call
        pugi::xml_node node1 = result1.node();
        
        // 4. Second node() call
        pugi::xml_node node2 = result2.node();
        
        // 5. Third select_node with char* overload
        pugi::xpath_node result3 = node1.select_node("*", &vars);
        
        // 6. Fourth select_node with xpath_query overload
        pugi::xpath_node result4 = node2.select_node(query);
        
        // 7. Fifth select_node with char* overload
        pugi::xpath_node result5 = root.select_node("*", &vars);
        
        // 8. Third node() call
        pugi::xml_node node3 = result3.node();
        
        // 9. Fourth node() call
        pugi::xml_node node4 = result4.node();
        
        // Try to diversify the calls with different nodes
        if (node1) {
            node1.select_node("*", &vars);
        }
        
        if (node2) {
            node2.select_node(query);
        }
        
        if (node3) {
            node3.select_node("*", &vars);
        }
        
        if (node4) {
            node4.select_node(query);
        }
        
        // Try with different queries
        std::string alt_query_str = "//*";
        pugi::xpath_query alt_query(alt_query_str.c_str());
        
        root.select_node(alt_query);
        
        // Try with empty variable set
        pugi::xpath_variable_set empty_vars;
        root.select_node("*", &empty_vars);
        
        // Try with null variables (default parameter)
        root.select_node("*");
        
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }
    
    return 0;
}