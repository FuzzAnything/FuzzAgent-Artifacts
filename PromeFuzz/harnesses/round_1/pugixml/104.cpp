// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
#include <string_view>

class FuzzWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test the begin callback
        if (!node.empty()) {
            node.root();
            node.parent();
            node.next_sibling();
        }
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test API functions during traversal
        if (!node.empty()) {
            node.root();
            node.parent();
            node.next_sibling();
        }
        return true; // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test the end callback
        if (!node.empty()) {
            node.root();
            node.parent();
            node.next_sibling();
        }
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    static pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, 
        pugi::parse_default | pugi::parse_fragment, 
        pugi::encoding_auto);
    
    // Test API functions on the document root
    pugi::xml_node root = doc.root();
    pugi::xml_node doc_root = doc.document_element();
    
    // Test root() function
    if (!root.empty()) {
        pugi::xml_node test_root = root.root();
    }
    
    // Test parent() function
    if (!doc_root.empty()) {
        pugi::xml_node parent = doc_root.parent();
        
        // Test on child nodes if they exist
        for (pugi::xml_node child = doc_root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            pugi::xml_node child_root = child.root();
            
            // Test next_sibling() function
            pugi::xml_node next_sib = child.next_sibling();
            
            // Test with string_view parameter for overloaded version
            if (child.name()) {
                std::string_view name_sv(reinterpret_cast<const char*>(Data), 
                    Size > 100 ? 100 : Size);
                pugi::xml_node next_named = child.next_sibling(name_sv.data());
            }
        }
    }
    
    // Test traverse() function with our walker
    if (!doc_root.empty()) {
        FuzzWalker walker;
        doc_root.traverse(walker);
    }
    
    // Test on empty nodes
    pugi::xml_node empty_node;
    pugi::xml_node empty_root = empty_node.root();
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_next = empty_node.next_sibling();
    
    // Clear the document for next iteration
    doc.reset();
    
    return 0;
}