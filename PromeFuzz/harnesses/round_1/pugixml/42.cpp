// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    // Ensure we have at least one byte for null termination
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzzer input
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    
    // Load XML document from string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    // If parsing failed, we can still proceed with empty document
    // Get root node (document element)
    pugi::xml_node root = doc.document_element();
    
    // Call child() with a fixed name - using "root" as a common element name
    pugi::xml_node child_node = root.child("root");
    
    // Call first_child() on the child node
    pugi::xml_node first_child = child_node.first_child();
    
    // Call next_sibling() twice on the first child
    pugi::xml_node next_sibling1 = first_child.next_sibling();
    pugi::xml_node next_sibling2 = next_sibling1.next_sibling();
    
    // Call last_child() on the second sibling
    pugi::xml_node last_child = next_sibling2.last_child();
    
    // Call type() on the last child
    pugi::xml_node_type node_type1 = last_child.type();
    
    // Call value() on the last child
    const char* node_value1 = last_child.value();
    
    // Call type() again on the last child
    pugi::xml_node_type node_type2 = last_child.type();
    
    // Call value() again on the last child
    const char* node_value2 = last_child.value();
    
    // Call last_child() again on the last child
    pugi::xml_node last_child2 = last_child.last_child();
    
    // Call first_child() on the last child
    pugi::xml_node first_child2 = last_child.first_child();
    
    // Call first_child() again on the last child
    pugi::xml_node first_child3 = last_child.first_child();
    
    // Use the results to avoid optimization
    static volatile pugi::xml_node_type dummy_type = node_type1;
    static volatile const char* dummy_value = node_value1;
    dummy_type = node_type2;
    dummy_value = node_value2;
    
    return 0;
}