// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Return true to continue traversal, false to cancel
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Default implementation returns true to continue traversal
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Return true on success
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    // Early exit for empty input
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, 
        pugi::parse_default | pugi::parse_ws_pcdata | pugi::parse_escapes);
    
    // Test API functions on various nodes
    
    // 1. Test on document element (root of the document)
    pugi::xml_node root = doc.document_element();
    if (root) {
        // Test parent() - should return empty node or document node
        pugi::xml_node parent = root.parent();
        
        // Test next_sibling() - iterate through siblings
        pugi::xml_node sibling = root.next_sibling();
        while (sibling) {
            sibling = sibling.next_sibling();
        }
        
        // Test root() - should return the same as document_element() or document node
        pugi::xml_node doc_root = root.root();
        
        // Test traverse() with custom walker
        root.traverse(g_walker);
    }
    
    // 2. Test on first child of root (if exists)
    if (root) {
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            pugi::xml_node child_parent = first_child.parent();
            pugi::xml_node next_sib = first_child.next_sibling();
            pugi::xml_node child_root = first_child.root();
            
            // Test traverse on child node
            first_child.traverse(g_walker);
        }
    }
    
    // 3. Test on an empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_next = empty_node.next_sibling();
    pugi::xml_node empty_root = empty_node.root();
    
    // 4. Test on document node itself
    pugi::xml_node doc_node = static_cast<pugi::xml_node>(doc);
    pugi::xml_node doc_node_parent = doc_node.parent();
    pugi::xml_node doc_node_next = doc_node.next_sibling();
    pugi::xml_node doc_node_root = doc_node.root();
    
    // 5. Test with string_view parameter for next_sibling(name)
    if (root) {
        std::string_view name_sv(reinterpret_cast<const char*>(Data), 
                                 Size > 100 ? 100 : Size);
        pugi::xml_node named_sibling = root.next_sibling(name_sv.data());
    }
    
    // 6. Test on all nodes in the document
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        pugi::xml_node node_parent = node.parent();
        pugi::xml_node node_next = node.next_sibling();
        pugi::xml_node node_root = node.root();
        
        // Test traversal on each node
        node.traverse(g_walker);
        
        // Also test children of each node
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            pugi::xml_node child_next = child.next_sibling();
            pugi::xml_node child_root = child.root();
        }
    }
    
    // 7. Test with a node constructed from internal pointer (as shown in constructor)
    // This is a more advanced test - we'll create a node from nullptr (empty node)
    pugi::xml_node_struct* node_struct = nullptr;
    pugi::xml_node constructed_node(node_struct);
    pugi::xml_node constructed_parent = constructed_node.parent();
    pugi::xml_node constructed_next = constructed_node.next_sibling();
    pugi::xml_node constructed_root = constructed_node.root();
    
    // 8. Test the walker's begin and end methods directly
    if (root) {
        g_walker.begin(root);
        g_walker.end(root);
    }
    
    // 9. Test with different node types if they exist
    pugi::xml_node node = doc.first_child();
    while (node) {
        // Test based on node type
        switch (node.type()) {
            case pugi::node_element:
            case pugi::node_pcdata:
            case pugi::node_cdata:
            case pugi::node_comment:
            case pugi::node_pi:
            case pugi::node_declaration:
            case pugi::node_doctype:
                // Test all functions on this node
                pugi::xml_node test_parent = node.parent();
                pugi::xml_node test_next = node.next_sibling();
                pugi::xml_node test_root = node.root();
                break;
        }
        node = node.next_sibling();
    }
    
    return 0;
}