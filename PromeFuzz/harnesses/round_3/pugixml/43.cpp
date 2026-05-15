// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Ensure we have at least one byte for null termination if needed
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string for load_string
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    
    // Initialize XML document
    pugi::xml_document doc;
    
    // 1. Load document from string
    pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
    
    // If parsing failed, we can still continue with empty document
    // but some operations might return empty nodes
    
    // 2. Get a child node with a specific name
    // We'll use a fixed name for simplicity, but could also extract from input
    const char* child_name = "node";
    pugi::xml_node child_node = doc.child(child_name);
    
    // 3. Get first child of the found node
    pugi::xml_node first_child = child_node.first_child();
    
    // 4. Get next sibling of first child
    pugi::xml_node next_sibling1 = first_child.next_sibling();
    
    // 5. Get next sibling again (chain traversal)
    pugi::xml_node next_sibling2 = next_sibling1.next_sibling();
    
    // 6. Get last child of the original child node
    pugi::xml_node last_child1 = child_node.last_child();
    
    // 7. Get type of last child
    pugi::xml_node_type type1 = last_child1.type();
    
    // 8. Get value of last child
    const char* value1 = last_child1.value();
    
    // 9. Get type again (redundant but specified)
    pugi::xml_node_type type2 = last_child1.type();
    
    // 10. Get value again (redundant but specified)
    const char* value2 = last_child1.value();
    
    // 11. Get last child of last_child1
    pugi::xml_node last_child2 = last_child1.last_child();
    
    // 12. Get first child of last_child2
    pugi::xml_node first_child2 = last_child2.first_child();
    
    // 13. Get first child of first_child2 (chain)
    pugi::xml_node first_child3 = first_child2.first_child();
    
    // All operations complete, automatic cleanup happens when objects go out of scope
    return 0;
}