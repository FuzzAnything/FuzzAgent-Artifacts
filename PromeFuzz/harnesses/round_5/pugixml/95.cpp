// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Test functions on document node
    pugi::xml_node doc_node = doc;
    pugi::xml_node doc_parent = doc_node.parent();
    pugi::xml_node_type doc_type = doc_node.type();
    pugi::xml_node doc_first_child = doc_node.first_child();
    pugi::xml_node doc_last_child = doc_node.last_child();
    ptrdiff_t doc_offset = doc_node.offset_debug();
    
    // Test functions on root element if it exists
    pugi::xml_node root = doc.document_element();
    if (root) {
        pugi::xml_node root_parent = root.parent();
        pugi::xml_node_type root_type = root.type();
        pugi::xml_node root_first_child = root.first_child();
        pugi::xml_node root_last_child = root.last_child();
        ptrdiff_t root_offset = root.offset_debug();
        
        // Traverse some children
        for (pugi::xml_node child = root_first_child; child; child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            pugi::xml_node_type child_type = child.type();
            pugi::xml_node child_first_child = child.first_child();
            pugi::xml_node child_last_child = child.last_child();
            ptrdiff_t child_offset = child.offset_debug();
            
            // Test append_child on child nodes
            if (Size > 0) {
                // Use first byte to determine node type (mod 9 for 9 node types)
                pugi::xml_node_type new_type = static_cast<pugi::xml_node_type>((Data[0] % 9) + 1);
                pugi::xml_node appended = child.append_child(new_type);
                
                // Test functions on the newly appended node
                if (appended) {
                    pugi::xml_node appended_parent = appended.parent();
                    pugi::xml_node_type appended_type = appended.type();
                    pugi::xml_node appended_first_child = appended.first_child();
                    pugi::xml_node appended_last_child = appended.last_child();
                    ptrdiff_t appended_offset = appended.offset_debug();
                }
            }
        }
    }
    
    // Test append_child on document node
    if (Size > 1) {
        // Use second byte to determine node type
        pugi::xml_node_type doc_new_type = static_cast<pugi::xml_node_type>((Data[1 % Size] % 9) + 1);
        pugi::xml_node doc_appended = doc_node.append_child(doc_new_type);
        
        if (doc_appended) {
            pugi::xml_node doc_appended_parent = doc_appended.parent();
            pugi::xml_node_type doc_appended_type = doc_appended.type();
            pugi::xml_node doc_appended_first_child = doc_appended.first_child();
            pugi::xml_node doc_appended_last_child = doc_appended.last_child();
            ptrdiff_t doc_appended_offset = doc_appended.offset_debug();
        }
    }
    
    // Test empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node_type empty_type = empty_node.type();
    pugi::xml_node empty_first_child = empty_node.first_child();
    pugi::xml_node empty_last_child = empty_node.last_child();
    ptrdiff_t empty_offset = empty_node.offset_debug();
    
    // Test append_child on empty node (should return empty node)
    pugi::xml_node empty_appended = empty_node.append_child(pugi::node_element);
    if (!empty_appended) {
        // Expected behavior - empty node returned
    }
    
    return 0;
}