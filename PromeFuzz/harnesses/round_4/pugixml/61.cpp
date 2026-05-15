// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test functions on various nodes in the document
    pugi::xml_node root = doc.document_element();
    
    // Test parent() function
    if (root) {
        pugi::xml_node parent = root.parent();
        (void)parent; // Use result to avoid unused variable warning
    }
    
    // Test type() function on root and children
    if (root) {
        pugi::xml_node_type root_type = root.type();
        (void)root_type;
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node_type child_type = child.type();
            (void)child_type;
        }
    }
    
    // Test append_child() and prepend_child() functions
    // Create a new document to test building XML structures
    pugi::xml_document test_doc;
    pugi::xml_node test_root = test_doc.append_child(pugi::node_element);
    
    if (test_root) {
        // Append various node types
        pugi::xml_node appended_element = test_root.append_child(pugi::node_element);
        pugi::xml_node appended_comment = test_root.append_child(pugi::node_comment);
        pugi::xml_node appended_text = test_root.append_child(pugi::node_pcdata);
        
        // Prepend various node types
        pugi::xml_node prepended_element = test_root.prepend_child(pugi::node_element);
        pugi::xml_node prepended_declaration = test_root.prepend_child(pugi::node_declaration);
        
        // Test on the appended/prepended nodes themselves
        if (appended_element) {
            pugi::xml_node_type appended_type = appended_element.type();
            (void)appended_type;
        }
    }
    
    // Test offset_debug() function
    if (root) {
        ptrdiff_t root_offset = root.offset_debug();
        (void)root_offset;
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            ptrdiff_t child_offset = child.offset_debug();
            (void)child_offset;
        }
    }
    
    // Test last_child() function
    if (root) {
        pugi::xml_node last_child = root.last_child();
        (void)last_child;
        
        // Test last_child() on the last child itself (if it exists)
        if (last_child) {
            pugi::xml_node last_child_of_last = last_child.last_child();
            (void)last_child_of_last;
        }
    }
    
    // Test parent() on various nodes
    if (root) {
        // Test parent on root
        pugi::xml_node root_parent = root.parent();
        (void)root_parent;
        
        // Test parent on children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            (void)child_parent;
            
            // Test parent on grandchildren
            for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                pugi::xml_node grandchild_parent = grandchild.parent();
                (void)grandchild_parent;
            }
        }
    }
    
    // Test type() on all possible node types in the test document
    pugi::xml_document type_test_doc;
    type_test_doc.append_child(pugi::node_document);
    type_test_doc.append_child(pugi::node_element);
    type_test_doc.append_child(pugi::node_pcdata);
    type_test_doc.append_child(pugi::node_cdata);
    type_test_doc.append_child(pugi::node_comment);
    type_test_doc.append_child(pugi::node_pi);
    type_test_doc.append_child(pugi::node_declaration);
    type_test_doc.append_child(pugi::node_doctype);
    
    for (pugi::xml_node node = type_test_doc.first_child(); node; node = node.next_sibling()) {
        pugi::xml_node_type node_type = node.type();
        (void)node_type;
    }
    
    return 0;
}