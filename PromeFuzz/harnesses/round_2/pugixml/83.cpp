// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

static void test_node_functions(pugi::xml_node node) {
    // Test offset_debug
    ptrdiff_t offset = node.offset_debug();
    (void)offset;
    
    // Test first_child
    pugi::xml_node first = node.first_child();
    (void)first;
    
    // Test type
    pugi::xml_node_type node_type = node.type();
    (void)node_type;
    
    // Test last_child
    pugi::xml_node last = node.last_child();
    (void)last;
    
    // Test parent
    pugi::xml_node parent = node.parent();
    (void)parent;
}

static void test_append_child(pugi::xml_node node) {
    // Test append_child with different node types
    pugi::xml_node child1 = node.append_child(pugi::node_element);
    pugi::xml_node child2 = node.append_child(pugi::node_pcdata);
    pugi::xml_node child3 = node.append_child(pugi::node_comment);
    pugi::xml_node child4 = node.append_child(pugi::node_cdata);
    pugi::xml_node child5 = node.append_child(pugi::node_pi);
    pugi::xml_node child6 = node.append_child(pugi::node_declaration);
    pugi::xml_node child7 = node.append_child(pugi::node_doctype);
    
    // Test default parameter
    pugi::xml_node child8 = node.append_child();
    
    (void)child1; (void)child2; (void)child3; (void)child4;
    (void)child5; (void)child6; (void)child7; (void)child8;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    // Create a document and parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Test functions on document node
    pugi::xml_node root = doc;
    test_node_functions(root);
    
    // Test append_child on document node
    test_append_child(root);
    
    // Test functions on all nodes in the parsed tree
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        test_node_functions(node);
        test_append_child(node);
        
        // Recursively test all children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            test_node_functions(child);
            test_append_child(child);
        }
    }
    
    // Also test with an empty node
    pugi::xml_node empty_node;
    test_node_functions(empty_node);
    
    // Test with a newly created document
    pugi::xml_document new_doc;
    pugi::xml_node new_root = new_doc.append_child(pugi::node_element);
    test_node_functions(new_root);
    test_append_child(new_root);
    
    // Test with different node types
    pugi::xml_node text_node = new_root.append_child(pugi::node_pcdata);
    test_node_functions(text_node);
    
    pugi::xml_node comment_node = new_root.append_child(pugi::node_comment);
    test_node_functions(comment_node);
    
    pugi::xml_node cdata_node = new_root.append_child(pugi::node_cdata);
    test_node_functions(cdata_node);
    
    // Test parent-child relationships
    pugi::xml_node child_node = new_root.append_child(pugi::node_element);
    test_node_functions(child_node);
    test_node_functions(child_node.parent());
    
    return 0;
}