// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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
#include <stdint.h>
#include <string>
#include <fstream>

static pugi::xml_document create_document_from_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    std::string xml_content(reinterpret_cast<const char*>(data), size);
    doc.load_string(xml_content.c_str());
    return doc;
}

static std::string create_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) return "//*";
    return std::string(reinterpret_cast<const char*>(data), std::min(size, static_cast<size_t>(100)));
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* Data, size_t Size) {
    static pugi::xml_document doc;
    static pugi::xpath_node_set node_set;
    
    // Reset state
    doc.reset();
    node_set = pugi::xpath_node_set();
    
    // Create XML document from fuzz data
    doc = create_document_from_data(Data, Size);
    
    // Create XPath query from fuzz data
    std::string query = create_xpath_query(Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Evaluate XPath query to get node set
    try {
        node_set = root.select_nodes(query.c_str());
    } catch (...) {
        // Ignore XPath evaluation errors
    }
    
    // Call target functions in specified order
    // 1. pugi::xpath_node_set::size
    size_t set_size = node_set.size();
    (void)set_size; // Prevent unused variable warning
    
    // 2. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t set_type = node_set.type();
    (void)set_type; // Prevent unused variable warning
    
    // 3. pugi::xpath_node_set::empty
    bool is_empty = node_set.empty();
    (void)is_empty; // Prevent unused variable warning
    
    // 4. pugi::xpath_node_set::first
    pugi::xpath_node first_node = node_set.first();
    (void)first_node; // Prevent unused variable warning
    
    // 5. pugi::xpath_node_set::end
    pugi::xpath_node_set::const_iterator end_iter = node_set.end();
    (void)end_iter; // Prevent unused variable warning
    
    // 6. pugi::xml_node::select_nodes
    // Call again with potentially different parameters
    pugi::xpath_variable_set vars;
    try {
        pugi::xpath_node_set another_set = root.select_nodes(query.c_str(), &vars);
        (void)another_set; // Prevent unused variable warning
    } catch (...) {
        // Ignore XPath evaluation errors
    }
    
    // Additional exploration: try with a dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
        
        pugi::xml_document file_doc;
        file_doc.load_file("./dummy_file");
        pugi::xml_node file_root = file_doc.root();
        
        try {
            pugi::xpath_node_set file_set = file_root.select_nodes("//node");
            (void)file_set; // Prevent unused variable warning
        } catch (...) {
            // Ignore errors
        }
    }
    
    return 0;
}