// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get document root and traverse nodes
    pugi::xml_node root = doc.root();
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        // Test parent() function
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test type() function
        pugi::xml_node_type node_type = node.type();
        (void)node_type;
        
        // Test first_child() function
        pugi::xml_node first_child = node.first_child();
        (void)first_child;
        
        // Test last_child() function
        pugi::xml_node last_child = node.last_child();
        (void)last_child;
        
        // Test offset_debug() function
        ptrdiff_t offset = node.offset_debug();
        (void)offset;
        
        // Test append_child() function with various node types
        // Use fuzzer data to determine which node type to create
        if (Size > 0) {
            uint8_t type_selector = Data[0] % 9; // 9 possible xml_node_type values
            pugi::xml_node_type new_type = static_cast<pugi::xml_node_type>(type_selector);
            pugi::xml_node new_child = node.append_child(new_type);
            (void)new_child;
        }
        
        // Also test append_child() with default parameter
        pugi::xml_node default_child = node.append_child();
        (void)default_child;
        
        // Test on empty node
        static pugi::xml_node empty_node;
        pugi::xml_node empty_parent = empty_node.parent();
        (void)empty_parent;
        
        pugi::xml_node_type empty_type = empty_node.type();
        (void)empty_type;
        
        pugi::xml_node empty_first_child = empty_node.first_child();
        (void)empty_first_child;
        
        pugi::xml_node empty_last_child = empty_node.last_child();
        (void)empty_last_child;
        
        ptrdiff_t empty_offset = empty_node.offset_debug();
        (void)empty_offset;
        
        pugi::xml_node empty_appended = empty_node.append_child();
        (void)empty_appended;
    }
    
    return 0;
}