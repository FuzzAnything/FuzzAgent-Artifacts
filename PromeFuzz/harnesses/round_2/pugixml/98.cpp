// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document to work with
    pugi::xml_document doc;
    const char* xml_content = "<root><node attr=\"value\">text</node><node>text2</node></root>";
    
    if (!doc.load_string(xml_content)) {
        return 0;
    }
    
    // Create xpath_node_set using XPath query
    pugi::xpath_node_set node_set;
    
    // Use fuzzer input to determine which XPath query to use
    if (Size > 0) {
        // Create XPath query from fuzzer input (limited to safe length)
        char xpath_query[256];
        size_t query_len = Size < 255 ? Size : 255;
        memcpy(xpath_query, Data, query_len);
        xpath_query[query_len] = '\0';
        
        try {
            node_set = doc.select_nodes(xpath_query);
        } catch (...) {
            // Ignore XPath parsing errors during fuzzing
        }
    } else {
        // Default query if no input
        node_set = doc.select_nodes("//node");
    }
    
    // Test all target API functions
    // 1. size()
    size_t set_size = node_set.size();
    
    // 2. first()
    pugi::xpath_node first_node = node_set.first();
    
    // 3. empty()
    bool is_empty = node_set.empty();
    
    // 4. end()
    pugi::xpath_node_set::const_iterator end_iter = node_set.end();
    
    // 5. begin()
    pugi::xpath_node_set::const_iterator begin_iter = node_set.begin();
    
    // 6. type()
    pugi::xpath_node_set::type_t set_type = node_set.type();
    
    // Additional exploration: iterate through the set if not empty
    if (!is_empty) {
        for (pugi::xpath_node_set::const_iterator it = begin_iter; it != end_iter; ++it) {
            // Access node to explore more code paths
            pugi::xml_node node = it->node();
            pugi::xml_attribute attr = it->attribute();
            
            // Check if nodes are valid
            bool node_valid = node;
            bool attr_valid = attr;
        }
    }
    
    // Test copy constructor
    pugi::xpath_node_set copied_set(node_set);
    
    // Test the copied set as well
    size_t copied_size = copied_set.size();
    bool copied_empty = copied_set.empty();
    pugi::xpath_node_set::type_t copied_type = copied_set.type();
    
    // Create empty xpath_node and xml_attribute objects
    pugi::xpath_node empty_xpath_node;
    pugi::xml_attribute empty_attr;
    pugi::xml_node empty_node;
    
    // Test with empty node set (created via default constructor)
    pugi::xpath_node_set empty_set;
    size_t empty_set_size = empty_set.size();
    bool empty_set_empty = empty_set.empty();
    pugi::xpath_node empty_set_first = empty_set.first();
    
    return 0;
}