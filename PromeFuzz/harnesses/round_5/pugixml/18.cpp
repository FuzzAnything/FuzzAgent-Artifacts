// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_utf8);
    
    // If parsing failed, we can still test with empty document
    if (!result) {
        // Create minimal valid XML document for testing
        static const char* minimal_xml = "<root/>";
        doc.load_string(minimal_xml);
    }
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // Create XPath query from part of the input
    // Use first 256 bytes or less for the XPath query
    size_t xpath_size = Size > 256 ? 256 : Size;
    char xpath_query[257];
    memcpy(xpath_query, Data, xpath_size);
    xpath_query[xpath_size] = '\0'; // Ensure null termination
    
    // Create xpath_variable_set (optional, can be null)
    pugi::xpath_variable_set variables;
    
    // Call select_nodes on the root node
    pugi::xpath_node_set node_set = root.select_nodes(xpath_query, &variables);
    
    // Now call the functions in the specified order
    // 1. size()
    size_t set_size = node_set.size();
    
    // 2. type()
    pugi::xpath_node_set::type_t set_type = node_set.type();
    
    // 3. empty()
    bool is_empty = node_set.empty();
    
    // 4. first()
    pugi::xpath_node first_node = node_set.first();
    
    // 5. end()
    pugi::xpath_node_set::const_iterator end_iter = node_set.end();
    
    // Additional: iterate through the node set to explore more states
    for (pugi::xpath_node_set::const_iterator it = node_set.begin(); it != end_iter; ++it) {
        // Access node properties to explore more code paths
        pugi::xml_node node = it->node();
        pugi::xml_attribute attr = it->attribute();
        
        // Call some basic operations on nodes
        if (node) {
            const char* name = node.name();
            const char* value = node.value();
            pugi::xml_node child = node.first_child();
            pugi::xml_node next_sibling = node.next_sibling();
        }
        
        if (attr) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
        }
    }
    
    // Test with null query pointer (should handle gracefully)
    try {
        pugi::xpath_node_set null_set = root.select_nodes(nullptr, nullptr);
        size_t null_size = null_set.size();
        bool null_empty = null_set.empty();
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with empty query
    pugi::xpath_node_set empty_set = root.select_nodes("", nullptr);
    size_t empty_size = empty_set.size();
    bool empty_is_empty = empty_set.empty();
    
    return 0;
}