// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static class test_walker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test the begin callback
        (void)node;
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required override for traversal
        (void)node;
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test the end callback
        (void)node;
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (!result) {
        // Even if parsing fails, test functions with empty nodes
        pugi::xml_node empty;
        (void)empty.traverse(g_walker);
        (void)empty.next_sibling();
        (void)empty.root();
        (void)empty.parent();
        return 0;
    }
    
    // Test traverse with custom walker
    pugi::xml_node root = doc.root();
    (void)root.traverse(g_walker);
    
    // Test navigation functions on various nodes
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        // Test next_sibling
        pugi::xml_node next = node.next_sibling();
        (void)next;
        
        // Test root
        pugi::xml_node doc_root = node.root();
        (void)doc_root;
        
        // Test parent
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Recursively test child nodes
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            (void)child.next_sibling();
            (void)child.root();
            (void)child.parent();
        }
    }
    
    // Test with string_view for next_sibling (if available in this context)
    // Note: next_sibling() without parameters is the target, but we can also
    // test next_sibling(name) if we want to explore more states
    if (Size > 0) {
        // Use first few bytes as a potential node name
        size_t name_len = Size > 100 ? 100 : Size;
        std::string_view name(reinterpret_cast<const char*>(Data), name_len);
        
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            pugi::xml_node next_named = node.next_sibling(name.data());
            (void)next_named;
        }
    }
    
    return 0;
}